#include "pa2m.h"
#include "src/tp.h"
#include "src/split.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ARCHIVO_INEXISTENTE "archivo_inexistente.txt"
#define ARCHIVO_EJEMPLO "ejemplo/pokemones.txt"
#define ARCHIVO_VACIO "ejemplo/archivo_vacio"
#define ARCHIVO_ERRONEO_PARCIAL "ejemplo/archivo_parcialmente_erroneo"
#define ARCHIVO_ERRONEO_TOTAL "ejemplo/archivo_totalmente_erroneo"
#define NOMBRE_INEXISTENTE "cortoplasista"
#define PIKACHU "Pikachu"
#define VANUSAUR "Venusaur"
#define MEOWTH "Meowth"
#define RATTATA "Rattata"
#define DRAGONITE "Dragonite"
#define CARETPIE "Caterpie"
#define MODO_LECTURA "r"
#define DELIMITADOR ','
#define CARACTER_NULO '\0'
#define POSICION_NOMBRE 0
#define NOMBRE_NULL NULL
#define SIN_FALLOS 0
#define VACIO 0
#define CANTIDAD_ESPERADA 25

void abajo(char *str)
{
	int i = 0;
	while (str[i] != CARACTER_NULO) {
		str[i] = (char)tolower(str[i]);
		i++;
	}
}

void destruir_split(char **split)
{
	int i = 0;
	while (split[i] != NULL) {
		free(split[i]);
		i++;
	}
	free(split);
}

void tp_crear_crea_e_inicializa()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(!tp, "Se prueba crear un tp con un archvio inexistente");

	tp = tp_crear(ARCHIVO_ERRONEO_PARCIAL);
	pa2m_afirmar(
		!tp,
		"Se prueba crear un tp con un archvio parcialmente erroneo");

	tp = tp_crear(ARCHIVO_ERRONEO_TOTAL);
	pa2m_afirmar(!tp,
		     "Se prueba crear un tp con un archvio totalmente erroneo");

	tp = tp_crear(ARCHIVO_EJEMPLO);
	pa2m_afirmar(tp != NULL, "Se prueba crear un tp con un archvio valido");

	tp_destruir(tp);
}

void tp_cantidad_pokemon_devuelve_la_cantidad_de_pokemones_en_juego()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(tp_cantidad_pokemon(tp) == VACIO,
		     "Un tp creado con un archvio inexistente esta vacio");

	tp = tp_crear(ARCHIVO_EJEMPLO);
	pa2m_afirmar(
		tp_cantidad_pokemon(tp) == CANTIDAD_ESPERADA,
		"Un tp creado con un archvio valido tiene la cantidad esperada");

	tp_destruir(tp);
}

void tp_buscar_pokemon_devuelve_el_pokemon_buscado_o_null()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(!tp_buscar_pokemon(tp, NULL),
		     "Se prueba buscar con un nombre nulo en un tp no valido");
	pa2m_afirmar(
		!tp_buscar_pokemon(tp, NOMBRE_INEXISTENTE),
		"Se prueba buscar con un nombre no incluido en un tp no valido");
	pa2m_afirmar(
		!tp_buscar_pokemon(tp, PIKACHU),
		"Se prueba buscar con un nombre incluido en un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	pa2m_afirmar(!tp_buscar_pokemon(tp, NULL),
		     "Se prueba buscar con un nombre nulo en un tp valido");
	pa2m_afirmar(
		!tp_buscar_pokemon(tp, NOMBRE_INEXISTENTE),
		"Se prueba buscar con un nombre no incluido en un tp valido");

	FILE *archivo;
	char linea[250];
	int contador_fallos_busqueda = 0;

	archivo = fopen(ARCHIVO_EJEMPLO, MODO_LECTURA);
	while (fgets(linea, sizeof(linea), archivo)) {
		linea[strcspn(linea, "\n")] = CARACTER_NULO;
		char **linea_separada = split(linea, DELIMITADOR);

		abajo(linea_separada[POSICION_NOMBRE]);
		linea_separada[POSICION_NOMBRE]
			      [strcspn(linea_separada[POSICION_NOMBRE], "\n")] =
				      '\0';

		if (!(tp_buscar_pokemon(tp, linea_separada[POSICION_NOMBRE])))
			contador_fallos_busqueda++;

		destruir_split(linea_separada);
	}
	fclose(archivo);

	pa2m_afirmar(contador_fallos_busqueda == SIN_FALLOS,
		     "Todos los pokemones estan almacenados");
	if (contador_fallos_busqueda != SIN_FALLOS)
		printf("Hubieron %i fallos\n", contador_fallos_busqueda);

	tp_destruir(tp);
}

void tp_nombres_disponibles_devuelve_un_str_con_todos_los_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(!tp_nombres_disponibles(tp),
		     "Se prueba obtener los nombres de un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);
	char *nombres_pokemones = tp_nombres_disponibles(tp);
	pa2m_afirmar(nombres_pokemones != NULL,
		     "Se prueba obtener los nombres de un tp valido");
	char **nombres_pokemones_split = split(nombres_pokemones, DELIMITADOR);

	int contador_fallos_orden = 0;
	for (int i = 0; i < tp_cantidad_pokemon(tp); i++) {
		for (int j = i + 1; j < tp_cantidad_pokemon(tp) - j; j++) {
			if (strcmp(nombres_pokemones_split[i],
				   nombres_pokemones_split[j]) > 0)
				contador_fallos_orden++;
		}
	}

	pa2m_afirmar(contador_fallos_orden == SIN_FALLOS,
		     "El string de nombres esta en orden alfabetico");
	if (contador_fallos_orden != SIN_FALLOS)
		printf("Hubieron %i fallos de orden\n", contador_fallos_orden);

	destruir_split(nombres_pokemones_split);
	free(nombres_pokemones);
	tp_destruir(tp);
}

void tp_seleccionar_pokemon_devuelve_true_si_se_pudo_seleccionar_un_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_2, NULL),
		"Se prueba seleccionar un pokemon de un tp no valido y con un nombre nulo");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_1, NOMBRE_INEXISTENTE),
		"Se prueba seleccionar un pokemon de un tp no valido y con un nombre no incluido");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_1, PIKACHU),
		"Se prueba seleccionar un pokemon de un tp no valido y con un nombre incluido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_2, NULL),
		"Se prueba seleccionar un pokemon de un tp valido y con un nombre nulo");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_2, NOMBRE_INEXISTENTE),
		"Se prueba seleccionar un pokemon de un tp valido y con un nombre no incluido");
	pa2m_afirmar(
		tp_seleccionar_pokemon(tp, JUGADOR_1, PIKACHU),
		"El jugador 1 prueba seleccionar un pokemon de un tp valido y con un nombre incluido");
	pa2m_afirmar(
		tp_seleccionar_pokemon(tp, JUGADOR_2, VANUSAUR),
		"El jugador 2 prueba seleccionar un pokemon de un tp valido y con un nombre incluido");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_1, VANUSAUR),
		"El jugador 1 prueba cambiar su pokemon por uno ya seleccionado");
	pa2m_afirmar(
		tp_seleccionar_pokemon(tp, JUGADOR_1, MEOWTH),
		"El jugador 1 prueba cambiar su pokemon por uno no seleccionado");
	pa2m_afirmar(
		!tp_seleccionar_pokemon(tp, JUGADOR_2, MEOWTH),
		"El jugador 2 prueba cambiar su pokemon por uno recien seleccionado");

	tp_destruir(tp);
}

bool es_el_pokemon_esperado(const struct pokemon_info *pokemon_1,
			    const struct pokemon_info *pokemon_2)
{
	if (strcmp(pokemon_1->nombre, pokemon_2->nombre) != 0)
		return false;
	if (pokemon_1->fuerza != pokemon_2->fuerza)
		return false;
	if (pokemon_1->destreza != pokemon_2->destreza)
		return false;
	if (pokemon_1->inteligencia != pokemon_2->inteligencia)
		return false;
	return true;
}

void tp_pokemon_seleccionado_devulve_el_pokemon_elegido_por_el_juador()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(
		!tp_pokemon_seleccionado(tp, JUGADOR_2),
		"Se prueba obtener la informacion de un pokemon de un tp no valido");
	pa2m_afirmar(
		!tp_pokemon_seleccionado(tp, JUGADOR_1),
		"Se prueba obtener la informacion pokemon de un tp no valido");
	pa2m_afirmar(
		!tp_pokemon_seleccionado(tp, JUGADOR_1),
		"Se prueba obtener la informacion de un pokemon de un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	pa2m_afirmar(
		!tp_pokemon_seleccionado(tp, JUGADOR_1),
		"Se prueba obtener la informacion de un pokemon de un jugador que no selecciono (JUGADOR_1)");
	pa2m_afirmar(
		!tp_pokemon_seleccionado(tp, JUGADOR_2),
		"Se prueba obtener la informacion de un pokemon de un jugador que no selecciono (JUGADOR_2)");

	tp_seleccionar_pokemon(tp, JUGADOR_1, MEOWTH);
	tp_seleccionar_pokemon(tp, JUGADOR_2, VANUSAUR);

	const struct pokemon_info *poke_jugador_1 =
		tp_pokemon_seleccionado(tp, JUGADOR_1);
	const struct pokemon_info *poke_jugador_2 =
		tp_pokemon_seleccionado(tp, JUGADOR_2);

	pa2m_afirmar(
		poke_jugador_1 != NULL,
		"Se prueba obtener la informacion de un pokemon de un jugador que selecciono (JUGADOR_1)");
	const struct pokemon_info *poke_esperado_1 =
		tp_buscar_pokemon(tp, MEOWTH);
	pa2m_afirmar(es_el_pokemon_esperado(poke_jugador_1, poke_esperado_1),
		     "El jugador 1 tiene el pokemon selecciono");

	pa2m_afirmar(
		poke_jugador_2 != NULL,
		"Se prueba obtener la informacion de un pokemon de un jugador que selecciono (JUGADOR_2)");
	const struct pokemon_info *poke_esperado_2 =
		tp_buscar_pokemon(tp, VANUSAUR);
	pa2m_afirmar(es_el_pokemon_esperado(poke_jugador_2, poke_esperado_2),
		     "El jugador 2 tiene el pokemon selecciono");

	const struct pokemon_info *primer_poke_seleccionado_1 = poke_jugador_1;
	tp_seleccionar_pokemon(tp, JUGADOR_1, VANUSAUR);
	poke_jugador_1 = tp_pokemon_seleccionado(tp, JUGADOR_1);
	poke_esperado_1 = tp_buscar_pokemon(tp, VANUSAUR);
	pa2m_afirmar(
		!es_el_pokemon_esperado(poke_jugador_1, poke_esperado_1) &&
			es_el_pokemon_esperado(primer_poke_seleccionado_1,
					       poke_jugador_1),
		"Se prueba seleccionar un pokemon ya seleccionado, no se puede y el jugador 1 mantiene su pokemon original");

	const struct pokemon_info *primer_poke_seleccionado_2 = poke_jugador_2;
	tp_seleccionar_pokemon(tp, JUGADOR_1, MEOWTH);
	poke_jugador_2 = tp_pokemon_seleccionado(tp, JUGADOR_2);
	poke_esperado_2 = tp_buscar_pokemon(tp, MEOWTH);
	pa2m_afirmar(
		!es_el_pokemon_esperado(poke_jugador_2, poke_esperado_2) &&
			es_el_pokemon_esperado(primer_poke_seleccionado_2,
					       poke_jugador_2),
		"Se prueba seleccionar un pokemon ya seleccionado, no se puede y el jugador 2 mantiene su pokemon original");

	tp_seleccionar_pokemon(tp, JUGADOR_1, RATTATA);
	poke_jugador_1 = tp_pokemon_seleccionado(tp, JUGADOR_1);
	pa2m_afirmar(!es_el_pokemon_esperado(poke_jugador_1,
					     primer_poke_seleccionado_1),
		     "Se prueba cambiar el pokemon del jugador 1");

	tp_seleccionar_pokemon(tp, JUGADOR_2, MEOWTH);
	poke_jugador_2 = tp_pokemon_seleccionado(tp, JUGADOR_2);
	pa2m_afirmar(
		!es_el_pokemon_esperado(poke_jugador_2,
					primer_poke_seleccionado_2),
		"Se prueba cambiar el pokemon del jugador 2 por una que antes estaba seleccionado");

	tp_destruir(tp);
}

void tp_agregar_obstaculo_devuleve_la_cantidad_de_obstaculos_en_la_pista()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA,
					  1000) == 0,
		     "Se prueba agregar un obstaculo a un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0) == 1,
		"Se prueba agregar un obstaculo de fuerza en la primera posicion y la cantidad de obstaculos es 1 (pista 1)");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0) == 1,
		"Se prueba agregar un obstaculo de fuerza en la primera posicion y la cantidad de obstaculos es 1 (pista 2)");

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1) == 2,
		"Se prueba agregar un obstaculo de destreza en la segunda posicion y la cantidad de obstaculos es 2 (pista 1)");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 1) == 2,
		"Se prueba agregar un obstaculo de destreza en la segunda posicion y la cantidad de obstaculos es 2 (pista 2)");

	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA,
				     2) == 3,
		"Se prueba agregar un obstaculo de inteligencia en la tercera posicion y la cantidad de obstaculos es 3 (pista 1)");
	pa2m_afirmar(
		tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA,
				     2) == 3,
		"Se prueba agregar un obstaculo de inteligencia en la tercera posicion y la cantidad de obstaculos es 3 (pista 2)");

	tp_destruir(tp);
}

void tp_quitar_obstaculo_devuelve_la_cantidad_de_obstaculos_restantes_en_la_pista()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(tp_quitar_obstaculo(tp, JUGADOR_1, 100) == 0,
		     "Se prueba quitar un obstaculo de un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 1);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 2);

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_1, 0) == 2,
		"Se prueba quitar un obstaculo de un tp y quedan 2 obstaculos (pista 1)");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 0) == 2,
		"Se prueba quitar un obstaculo de un tp y quedan 2 obstaculos (pista 2)");

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_1, 1) == 1,
		"Se prueba quitar un obstaculo de un tp y queda 1 obstaculo (pista 1)");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 1) == 1,
		"Se prueba quitar un obstaculo de un tp y queda 1 obstaculo (pista 2)");

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_1, 10) == 0,
		"Se prueba quitar un obstaculo de un tp en una posicion mayor al tamanio de la pista (pista 1)");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 10) == 0,
		"Se prueba quitar un obstaculo de un tp en una posucion mayor al tamanio de la pista (pista 2)");

	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_1, 0) == 0,
		"Se prueba quitar un obstaculo de un tp y la pista queda vacia (pista 1)");
	pa2m_afirmar(
		tp_quitar_obstaculo(tp, JUGADOR_2, 0) == 0,
		"Se prueba quitar un obstaculo de un tp y la pista queda vacia (pista 2)");

	tp_destruir(tp);
}

void tp_obstaculos_pista_devuleve_un_string_con_las_iniciales_de_los_obstaculos_en_pista()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(
		!tp_obstaculos_pista(tp, JUGADOR_1),
		"Se prueba conseguir los obstaculos,en la pista 1, de un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	pa2m_afirmar(
		!tp_obstaculos_pista(tp, JUGADOR_1),
		"Se prueba conseguir los obstaculos,en la pista 1, de un tp valido (la pista esta vacia)");
	pa2m_afirmar(
		!tp_obstaculos_pista(tp, JUGADOR_2),
		"Se prueba conseguir los obstaculos,en la pista 2, de un tp valido (la pista esta vacia)");

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 0);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 1);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 2);

	char *obstaculos_pista1 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		obstaculos_pista1 != NULL,
		"Se prueba conseguir los obstaculos,en la pista 1, de un tp valido");
	char *obstaculos_pista2 = tp_obstaculos_pista(tp, JUGADOR_2);
	pa2m_afirmar(
		obstaculos_pista2 != NULL,
		"Se prueba conseguir los obstaculos,en la pista 2, de un tp valido");

	free(obstaculos_pista1);
	free(obstaculos_pista2);

	tp_destruir(tp);
}

void tp_calcular_tiempo_pista_calcula_las_unidades_de_tiempo_que_tarda_en_recorrer_la_pista_el_pokemon()
{
	TP *tp = tp_crear(ARCHIVO_INEXISTENTE);
	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_1) == 0,
		"Se prueba calcular el tiempo en pista para un tp no valido");

	tp = tp_crear(ARCHIVO_EJEMPLO);

	pa2m_afirmar(
		tp_calcular_tiempo_pista(tp, JUGADOR_2) == 0,
		"Se prueba calcular el tiempo en pista para una pista vacia y un jugador sin pokemon");

	tp_seleccionar_pokemon(tp, JUGADOR_1, CARETPIE);
	tp_seleccionar_pokemon(tp, JUGADOR_2, DRAGONITE);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 7);

	unsigned tiempo_jugador1 = tp_calcular_tiempo_pista(tp, JUGADOR_1);
	pa2m_afirmar(
		tiempo_jugador1 == 36,
		"Se prueba calcular el tiempo en pista para el pokemon del jugador 1");

	unsigned tiempo_jugador2 = tp_calcular_tiempo_pista(tp, JUGADOR_2);
	pa2m_afirmar(
		tiempo_jugador2 == 6,
		"Se prueba calcular el tiempo en pista para el pokemon del jugador 1");

	tp_destruir(tp);
}

void tp_tiempo_por_obstaculo_devuelve_un_string_con_los_tiempor_que_tardo_el_pokemon_por_cada_obstaculo()
{
	TP *tp = tp_crear(ARCHIVO_EJEMPLO);

	tp_seleccionar_pokemon(tp, JUGADOR_1, CARETPIE);
	tp_seleccionar_pokemon(tp, JUGADOR_2, DRAGONITE);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 7);

	char *str_tiempos1 = tp_tiempo_por_obstaculo(tp, JUGADOR_1);
	pa2m_afirmar(str_tiempos1 != NULL,
		     "Se prueba conseguir los tiempos de un pokemon");

	char *str_tiempos2 = tp_tiempo_por_obstaculo(tp, JUGADOR_2);
	pa2m_afirmar(str_tiempos2 != NULL,
		     "Se prueba conseguir los tiempos de un pokemon");

	free(str_tiempos1);
	free(str_tiempos2);

	tp_destruir(tp);
}

void tp_limpiar_pista_vacia_la_pista_de_obstaculos()
{
	TP *tp = tp_crear(ARCHIVO_EJEMPLO);

	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_1, OBSTACULO_INTELIGENCIA, 7);

	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 0);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_FUERZA, 1);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 2);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 3);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 4);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_DESTREZA, 5);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 6);
	tp_agregar_obstaculo(tp, JUGADOR_2, OBSTACULO_INTELIGENCIA, 7);

	tp_limpiar_pista(tp, JUGADOR_1);
	char *obstaculos_pista1 = tp_obstaculos_pista(tp, JUGADOR_1);
	pa2m_afirmar(!obstaculos_pista1,
		     "No se puede obtener los obtaculos de una pista vacia");

	tp_limpiar_pista(tp, JUGADOR_2);
	char *obstaculos_pista2 = tp_obstaculos_pista(tp, JUGADOR_2);
	pa2m_afirmar(!obstaculos_pista2,
		     "No se puede obtener los obtaculos de una pista vacia");

	tp_destruir(tp);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	pa2m_nuevo_grupo("Pruebas de Creacion TP");
	tp_crear_crea_e_inicializa();

	pa2m_nuevo_grupo("Pruebas de Cantidad TP");
	tp_cantidad_pokemon_devuelve_la_cantidad_de_pokemones_en_juego();

	pa2m_nuevo_grupo("Pruebas de Busqueda TP");
	tp_buscar_pokemon_devuelve_el_pokemon_buscado_o_null();

	pa2m_nuevo_grupo("Pruebas de Nombres TP");
	tp_nombres_disponibles_devuelve_un_str_con_todos_los_pokemon();

	pa2m_nuevo_grupo("Pruebas de Seleccion TP");
	tp_seleccionar_pokemon_devuelve_true_si_se_pudo_seleccionar_un_pokemon();

	pa2m_nuevo_grupo("Pruebas de Pokemon Seleccionado TP");
	tp_pokemon_seleccionado_devulve_el_pokemon_elegido_por_el_juador();

	pa2m_nuevo_grupo("Pruebas de Agregar Obstaculo TP");
	tp_agregar_obstaculo_devuleve_la_cantidad_de_obstaculos_en_la_pista();

	pa2m_nuevo_grupo("Prueba de Quitar Obstaculo TP");
	tp_quitar_obstaculo_devuelve_la_cantidad_de_obstaculos_restantes_en_la_pista();

	pa2m_nuevo_grupo("Pruebas de Obstaculos en Pista TP");
	tp_obstaculos_pista_devuleve_un_string_con_las_iniciales_de_los_obstaculos_en_pista();

	pa2m_nuevo_grupo("Pruebas de Calcular Tiempo en Pista");
	tp_calcular_tiempo_pista_calcula_las_unidades_de_tiempo_que_tarda_en_recorrer_la_pista_el_pokemon();

	pa2m_nuevo_grupo("Pruebas de Tiempo por Obstaculo");
	tp_tiempo_por_obstaculo_devuelve_un_string_con_los_tiempor_que_tardo_el_pokemon_por_cada_obstaculo();

	pa2m_nuevo_grupo("Pruebas de Limpieza de Pista");
	tp_limpiar_pista_vacia_la_pista_de_obstaculos();

	return pa2m_mostrar_reporte();
}
