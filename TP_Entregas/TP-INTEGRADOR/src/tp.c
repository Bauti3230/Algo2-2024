#include "tp.h"
#include "abb.h"
#include "lista.h"
#include "split.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MODO_LECTURA "r"
#define DELIMITADOR ','
#define TAMANIO_INICIAL 1
#define POSICION_NOMBRE 0
#define POSICION_FUERZA 1
#define POSICION_DESTREZA 2
#define POSICION_INTELIGENCIA 3
#define CANTIDAD_ESPERADA 3
#define SIN_ARCHIVO NULL
#define CARACTER_NULO '\0'
#define FUERZA 'F'
#define DESTREZA 'D'
#define INTELIGENCIA 'I'

typedef struct datos_jugador {
	struct pokemon_info *pokemon;
	bool seleccionado;
} datos_jugador_t;

struct tp {
	abb_t *roster_pokemon;
	lista_t *pista_1;
	lista_t *pista_2;
	datos_jugador_t *jugador1;
	datos_jugador_t *jugador2;
};

int comparador_nombres(void *dato_1, void *dato_2)
{
	struct pokemon_info *poke_1 = (struct pokemon_info *)dato_1;
	struct pokemon_info *poke_2 = (struct pokemon_info *)dato_2;

	return strcmp(poke_1->nombre, poke_2->nombre);
}

char *strdup2(char *str)
{
	size_t len = strlen(str) + 1;
	char *dst = malloc(len * sizeof(char));
	if (!dst)
		return NULL;
	strcpy(dst, str);
	return dst;
}

void to_lower(char *str)
{
	int i = 0;
	while (str[i] != CARACTER_NULO) {
		str[i] = (char)tolower(str[i]);
		i++;
	}
}

struct pokemon_info *crear_pokemon(char **linea_de_archivo)
{
	struct pokemon_info *nuevo_pokemon =
		malloc(sizeof(struct pokemon_info));
	if (!nuevo_pokemon)
		return NULL;

	for (int i = 0; linea_de_archivo[i]; i++) {
		switch (i) {
		case POSICION_NOMBRE:
			to_lower(linea_de_archivo[i]);
			linea_de_archivo[i][strcspn(linea_de_archivo[i], "\n")] =
				'\0';
			nuevo_pokemon->nombre = strdup2(linea_de_archivo[i]);
			if (!nuevo_pokemon->nombre) {
				free(nuevo_pokemon);
				return NULL;
			}
			break;
		case POSICION_FUERZA:
			nuevo_pokemon->fuerza = atoi(linea_de_archivo[i]);
			break;
		case POSICION_DESTREZA:
			nuevo_pokemon->destreza = atoi(linea_de_archivo[i]);
			break;
		case POSICION_INTELIGENCIA:
			nuevo_pokemon->inteligencia = atoi(linea_de_archivo[i]);
			break;
		}
	}
	return nuevo_pokemon;
}

void *finalizacion_abrupta(TP *tp, FILE *archivo)
{
	if (tp->pista_1)
		free(tp->pista_1);

	if (tp->pista_2)
		free(tp->pista_2);

	if (tp->roster_pokemon)
		free(tp->roster_pokemon);

	if (tp->jugador1)
		free(tp->jugador1);

	if (tp->jugador2)
		free(tp->jugador2);

	if (archivo)
		fclose(archivo);

	return NULL;
}

void split_destruir(char **split)
{
	int i = 0;
	while (split[i] != NULL) {
		free(split[i]);
		i++;
	}
	free(split);
}

void pokemon_destruir(void *elemento)
{
	struct pokemon_info *pokemon = (struct pokemon_info *)elemento;

	free(pokemon->nombre);
	free(pokemon);
}

bool tiene_el_formato_correcto(char *linea_de_archivo)
{
	int contador = 0;
	while (*linea_de_archivo) {
		if (*linea_de_archivo == DELIMITADOR)
			contador++;
		linea_de_archivo++;
	}
	return contador == CANTIDAD_ESPERADA;
}

TP *tp_crear(const char *nombre_archivo)
{
	FILE *archivo;
	char linea[250];

	archivo = fopen(nombre_archivo, MODO_LECTURA);
	if (!archivo)
		return NULL;

	TP *nuevo_tp = calloc(1, sizeof(TP));
	if (!nuevo_tp)
		return finalizacion_abrupta(nuevo_tp, archivo);

	nuevo_tp->roster_pokemon = abb_crear(comparador_nombres);
	if (!nuevo_tp->roster_pokemon)
		return finalizacion_abrupta(nuevo_tp, archivo);

	nuevo_tp->pista_1 = lista_crear();
	if (!nuevo_tp->pista_1)
		return finalizacion_abrupta(nuevo_tp, archivo);

	nuevo_tp->pista_2 = lista_crear();
	if (!nuevo_tp->pista_2)
		return finalizacion_abrupta(nuevo_tp, archivo);

	nuevo_tp->jugador1 = calloc(1, sizeof(datos_jugador_t));
	if (!nuevo_tp->jugador1)
		return finalizacion_abrupta(nuevo_tp, archivo);

	nuevo_tp->jugador2 = calloc(1, sizeof(datos_jugador_t));
	if (!nuevo_tp->jugador2)
		return finalizacion_abrupta(nuevo_tp, archivo);

	while (fgets(linea, sizeof(linea), archivo)) {
		if (!tiene_el_formato_correcto(linea)) {
			tp_destruir(nuevo_tp);
			fclose(archivo);
			return NULL;
		}
		linea[strcspn(linea, "\n")] = CARACTER_NULO;
		char **linea_separada = split(linea, DELIMITADOR);

		struct pokemon_info *pokemon = crear_pokemon(linea_separada);
		abb_insertar(nuevo_tp->roster_pokemon, pokemon);
		split_destruir(linea_separada);
	}
	fclose(archivo);

	return nuevo_tp;
}

int tp_cantidad_pokemon(TP *tp)
{
	if (!tp)
		return 0;

	return (int)abb_tamanio(tp->roster_pokemon);
}

const struct pokemon_info *tp_buscar_pokemon(TP *tp, const char *nombre)
{
	if (!tp || !nombre)
		return NULL;

	char *aux = strdup2((char *)nombre);
	if (!aux)
		return NULL;

	to_lower(aux);
	struct pokemon_info pokemon_aux;
	pokemon_aux.nombre = aux;
	pokemon_aux.fuerza = 0;
	pokemon_aux.destreza = 0;
	pokemon_aux.inteligencia = 0;

	struct pokemon_info *pokemon_buscado =
		abb_buscar(tp->roster_pokemon, &pokemon_aux);
	free(aux);

	return pokemon_buscado;
}

bool esta_disponible(struct pokemon_info *pokemon1,
		     struct pokemon_info *pokemon2, char *nombre_pokemon)
{
	if (!pokemon1 && !pokemon2)
		return true;

	if (!pokemon1)
		return strcmp(pokemon2->nombre, nombre_pokemon) != 0;

	if (!pokemon2)
		return strcmp(pokemon1->nombre, nombre_pokemon) != 0;

	return strcmp(pokemon1->nombre, nombre_pokemon) != 0 &&
	       strcmp(pokemon2->nombre, nombre_pokemon) != 0;
}

char *crear_string_nombres(struct pokemon_info **array_pokemones,
			   size_t cantidad_pokemones, TP *tp)
{
	size_t tamanio = TAMANIO_INICIAL;
	char *resultado = malloc(tamanio * sizeof(char));
	if (!resultado)
		return NULL;

	resultado[0] = CARACTER_NULO;

	for (size_t i = 0; i < cantidad_pokemones; i++) {
		if (esta_disponible(tp->jugador1->pokemon,
				    tp->jugador2->pokemon,
				    array_pokemones[i]->nombre)) {
			struct pokemon_info *pokemon = array_pokemones[i];
			tamanio += strlen(pokemon->nombre) + 1;
			resultado = realloc(resultado, tamanio * sizeof(char));
			if (!resultado)
				return NULL;
			strcat(resultado, pokemon->nombre);
			strcat(resultado, ",");
		}
	}

	if (cantidad_pokemones > 0) {
		resultado[tamanio - 2] = '\0';
	}

	return resultado;
}

char *tp_nombres_disponibles(TP *tp)
{
	if (!tp)
		return NULL;

	int tamanio = tp_cantidad_pokemon(tp);

	void *todos_los_nombres[tamanio];
	size_t cantidad_elementos = abb_recorrer(tp->roster_pokemon, INORDEN,
						 todos_los_nombres,
						 (size_t)tamanio);

	struct pokemon_info **array_pokemones =
		(struct pokemon_info **)todos_los_nombres;

	return crear_string_nombres(array_pokemones, cantidad_elementos, tp);
}

struct pokemon_info *pokemon_elegido(datos_jugador_t *jugador,
				     const struct pokemon_info *pokemon)
{
	jugador->pokemon = (struct pokemon_info *)pokemon;
	jugador->seleccionado = true;

	return jugador->pokemon;
}

bool tp_seleccionar_pokemon(TP *tp, enum TP_JUGADOR jugador, const char *nombre)
{
	if (!tp || !nombre)
		return false;

	const struct pokemon_info *pokemon_seleccionado =
		tp_buscar_pokemon(tp, nombre);
	if (!pokemon_seleccionado)
		return false;

	if (!esta_disponible(tp->jugador1->pokemon, tp->jugador2->pokemon,
			     pokemon_seleccionado->nombre))
		return false;

	if (jugador == JUGADOR_1) {
		return pokemon_elegido(tp->jugador1, pokemon_seleccionado) !=
		       NULL;
	} else if (jugador == JUGADOR_2) {
		return pokemon_elegido(tp->jugador2, pokemon_seleccionado) !=
		       NULL;
	}
	return false;
}

const struct pokemon_info *tp_pokemon_seleccionado(TP *tp,
						   enum TP_JUGADOR jugador)
{
	if (!tp)
		return NULL;

	if (jugador == JUGADOR_1) {
		return tp->jugador1->pokemon;
	} else if (jugador == JUGADOR_2) {
		return tp->jugador2->pokemon;
	}
	return NULL;
}

void agregar_obstaculo_segun_tipo(lista_t *pista, int *obstaculo,
				  unsigned posicion)
{
	if (*obstaculo == OBSTACULO_FUERZA) {
		lista_insertar_en_posicion(pista, obstaculo, posicion);
	} else if (*obstaculo == OBSTACULO_DESTREZA) {
		lista_insertar_en_posicion(pista, obstaculo, posicion);
	} else if (*obstaculo == OBSTACULO_INTELIGENCIA) {
		lista_insertar_en_posicion(pista, obstaculo, posicion);
	}
}

unsigned tp_agregar_obstaculo(TP *tp, enum TP_JUGADOR jugador,
			      enum TP_OBSTACULO obstaculo, unsigned posicion)
{
	if (!tp)
		return 0;

	int *obstaculo_ptr = malloc(sizeof(int));
	if (!obstaculo_ptr)
		return 0;

	*obstaculo_ptr = obstaculo;

	if (jugador == JUGADOR_1) {
		agregar_obstaculo_segun_tipo(tp->pista_1, obstaculo_ptr,
					     posicion);
		return (unsigned)lista_tamanio(tp->pista_1);
	} else if (jugador == JUGADOR_2) {
		agregar_obstaculo_segun_tipo(tp->pista_2, obstaculo_ptr,
					     posicion);
		return (unsigned)lista_tamanio(tp->pista_2);
	}
	return 0;
}

unsigned obstaculos_restantes_en_pista(lista_t *pista, unsigned posicion)
{
	if (lista_tamanio(pista) < posicion)
		return 0;

	int *obstaculo_ptr = (int *)lista_quitar_de_posicion(pista, posicion);
	free(obstaculo_ptr);

	return (unsigned)lista_tamanio(pista);
}

unsigned tp_quitar_obstaculo(TP *tp, enum TP_JUGADOR jugador, unsigned posicion)
{
	if (!tp)
		return 0;

	if (jugador == JUGADOR_1) {
		return obstaculos_restantes_en_pista(tp->pista_1, posicion);
	} else if (jugador == JUGADOR_2) {
		return obstaculos_restantes_en_pista(tp->pista_2, posicion);
	}
	return 0;
}

char *string_obstaculos(lista_t *pista)
{
	if (lista_vacia(pista))
		return NULL;

	size_t cantidad_obstaculos = lista_tamanio(pista);
	char *str_obstaculos = malloc((cantidad_obstaculos + 1) * sizeof(char));
	if (!str_obstaculos)
		return NULL;

	int i = 0;
	lista_iterador_t *iterador_externo = lista_iterador_crear(pista);

	for (; lista_iterador_tiene_siguiente(iterador_externo);
	     lista_iterador_avanzar(iterador_externo)) {
		int *obstaculo_ptr =
			lista_iterador_elemento_actual(iterador_externo);

		if (*obstaculo_ptr == OBSTACULO_FUERZA) {
			str_obstaculos[i] = FUERZA;
		} else if (*obstaculo_ptr == OBSTACULO_DESTREZA) {
			str_obstaculos[i] = DESTREZA;
		} else if (*obstaculo_ptr == OBSTACULO_INTELIGENCIA) {
			str_obstaculos[i] = INTELIGENCIA;
		}
		i++;
	}
	str_obstaculos[i] = CARACTER_NULO;

	lista_iterador_destruir(iterador_externo);
	return str_obstaculos;
}

char *tp_obstaculos_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp)
		return NULL;

	if (jugador == JUGADOR_1) {
		return string_obstaculos(tp->pista_1);
	} else if (jugador == JUGADOR_2) {
		return string_obstaculos(tp->pista_2);
	}
	return NULL;
}

void tp_limpiar_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp)
		return;

	if (jugador == JUGADOR_1) {
		while (!lista_vacia(tp->pista_1)) {
			int *obstaculo_ptr = (int *)lista_quitar(tp->pista_1);
			free(obstaculo_ptr);
		}
	} else if (jugador == JUGADOR_2) {
		while (!lista_vacia(tp->pista_2)) {
			int *obstaculo_ptr = (int *)lista_quitar(tp->pista_2);
			free(obstaculo_ptr);
		}
	}
	return;
}

unsigned tiempo_total_en_pista(datos_jugador_t *jugador, lista_t *pista)
{
	if (!jugador->seleccionado || lista_vacia(pista))
		return 0;

	unsigned puntaje = 0;
	int n = 0;
	char anterior = CARACTER_NULO;

	char *str_obstaculos = string_obstaculos(pista);
	int cantidad_obstaculos = (int)strlen(str_obstaculos);

	for (int i = 0; i < cantidad_obstaculos; i++) {
		if (str_obstaculos[i] != anterior)
			n = 0;

		int calculo = 0;
		if (str_obstaculos[i] == FUERZA) {
			calculo = 10 - n - jugador->pokemon->fuerza;
		} else if (str_obstaculos[i] == DESTREZA) {
			calculo = 10 - n - jugador->pokemon->destreza;
		} else if (str_obstaculos[i] == INTELIGENCIA) {
			calculo = 10 - n - jugador->pokemon->inteligencia;
		}

		if (calculo > 0)
			puntaje += (unsigned)calculo;

		anterior = str_obstaculos[i];
		n++;
	}
	free(str_obstaculos);
	return puntaje;
}

unsigned tp_calcular_tiempo_pista(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp)
		return 0;

	if (jugador == JUGADOR_1) {
		return tiempo_total_en_pista(tp->jugador1, tp->pista_1);
	} else if (jugador == JUGADOR_2) {
		return tiempo_total_en_pista(tp->jugador2, tp->pista_2);
	}
	return 0;
}

char *intToString(int num)
{
	int tamanio = snprintf(NULL, 0, "%d", num);
	char *str = malloc((unsigned)(tamanio + 1) * sizeof(char));
	if (!str)
		return NULL;

	snprintf(str, (size_t)tamanio + 1, "%d", num);
	return str;
}

char *string_tiempos_por_obstaculos(datos_jugador_t *jugador, lista_t *pista)
{
	if (!jugador->seleccionado || lista_vacia(pista))
		return NULL;

	int tamanio = TAMANIO_INICIAL;
	int n = 0;
	char anterior = CARACTER_NULO;
	char *resultado = malloc(sizeof(char));

	resultado[0] = CARACTER_NULO;

	char *str_obstaculos = string_obstaculos(pista);
	int cantidad_obstaculos = (int)strlen(str_obstaculos);
	for (int i = 0; i < cantidad_obstaculos; i++) {
		if (str_obstaculos[i] != anterior)
			n = 0;

		int calculo = 0;
		if (str_obstaculos[i] == FUERZA) {
			calculo = 10 - n - jugador->pokemon->fuerza;
		} else if (str_obstaculos[i] == DESTREZA) {
			calculo = 10 - n - jugador->pokemon->destreza;
		} else if (str_obstaculos[i] == INTELIGENCIA) {
			calculo = 10 - n - jugador->pokemon->inteligencia;
		}

		if (calculo < 0)
			calculo = 0;

		char *buffer = intToString(calculo);

		int nuevo_tamanio =
			(int)strlen(resultado) + (int)strlen(buffer) + 2;
		if (nuevo_tamanio > tamanio) {
			tamanio = nuevo_tamanio;
			resultado = realloc(resultado,
					    (unsigned)tamanio * sizeof(char));
		}

		if (i > 0)
			strcat(resultado, ",");
		strcat(resultado, buffer);

		n++;
		anterior = str_obstaculos[i];

		free(buffer);
	}
	free(str_obstaculos);
	return resultado;
}

char *tp_tiempo_por_obstaculo(TP *tp, enum TP_JUGADOR jugador)
{
	if (!tp)
		return NULL;

	if (jugador == JUGADOR_1) {
		return string_tiempos_por_obstaculos(tp->jugador1, tp->pista_1);
	} else if (jugador == JUGADOR_2) {
		return string_tiempos_por_obstaculos(tp->jugador2, tp->pista_2);
	}
	return NULL;
}

void tp_destruir(TP *tp)
{
	if (!tp)
		return;

	abb_destruir_todo(tp->roster_pokemon, pokemon_destruir);
	if (lista_vacia(tp->pista_1)) {
		free(tp->pista_1);
	} else if (!lista_vacia(tp->pista_1)) {
		lista_destruir_todo(tp->pista_1, free);
	}

	if (lista_vacia(tp->pista_2)) {
		free(tp->pista_2);
	} else if (!lista_vacia(tp->pista_2)) {
		lista_destruir_todo(tp->pista_2, free);
	}
	free(tp->jugador1);
	free(tp->jugador2);
	free(tp);
}