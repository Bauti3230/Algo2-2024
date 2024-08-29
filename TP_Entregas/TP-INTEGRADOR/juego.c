#include "src/tp.h"
#include "src/split.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define POSICION_NOMBRE 0
#define CANT_DIFICULTADES 4
#define FACIL 1
#define LONGITUD_FACIL 3
#define INTENTOS_MODO_FACIL 5
#define NORMAL 2
#define LONGITUD_NORMAL 5
#define INTENTOS_MODO_NORMAL 4
#define DIFICIL 3
#define LONGITUD_DIFICL 7
#define INTENTOS_MODO_DIFCIL 3
#define IMPOSIBLE 4
#define LONGITUD_IMPOSIBLE 9
#define INTENTOS_MODO_IMPOSIBLE 2
#define MODO_LECTURA "r"
#define DELIMITADOR ','
#define AGREGAR 1
#define QUITAR 2
#define SALIR -1

typedef struct {
	int *opciones_dificultad;
} Menu;

Menu *menu_crear()
{
	Menu *menu = malloc(sizeof(Menu));
	if (!menu)
		return NULL;

	menu->opciones_dificultad = malloc(CANT_DIFICULTADES * sizeof(int));
	if (!menu->opciones_dificultad) {
		free(menu);
		return NULL;
	}

	for (int i = 0; i < CANT_DIFICULTADES; i++)
		menu->opciones_dificultad[i] = i + 1;

	return menu;
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

char *strdup3(char *str)
{
	size_t len = strlen(str) + 1;
	char *dst = malloc(len * sizeof(char));
	if (!dst)
		return NULL;
	strcpy(dst, str);
	return dst;
}

char *pokemon_aleatorio(int num_linea)
{
	FILE *archivo;
	char linea[250];

	archivo = fopen("ejemplo/pokemones.txt", MODO_LECTURA);
	if (!archivo)
		return NULL;

	int i = 0;
	while (fgets(linea, sizeof(linea), archivo) && i < num_linea)
		i++;

	fclose(archivo);

	char **linea_separada = split(linea, DELIMITADOR);
	char *nombre_pokemon = strdup3(linea_separada[POSICION_NOMBRE]);
	destruir_split(linea_separada);

	return nombre_pokemon;
}

void menu_destruir(Menu *menu)
{
	if (!menu)
		return;

	free(menu->opciones_dificultad);
	free(menu);
}

void seleccionar_dificultad(Menu *menu, int *dificultad)
{
	printf("Elegi tu dificultad (Ej : 1 para elegir Facil)\n");
	printf("1) Facil \n");
	printf("2) Normal \n");
	printf("3) Dificl \n");
	printf("4) Imposible \n");
	if (scanf("%d", dificultad) == -1)
		return;
	while (*dificultad < FACIL || *dificultad > IMPOSIBLE) {
		printf("Epa, me parece que te equivocaste\n");
		printf("Acordate que solo podes poner numeros que tengan asociada una dificultad\n");
		printf("\n");
		printf("Volve a intentar\n");
		printf("1) Facil \n");
		printf("2) Normal \n");
		printf("3) Dificl \n");
		printf("4) Imposible \n");
		if (scanf("%d", dificultad) == -1)
			return;
	}
}

void imprimir_pokemon(const struct pokemon_info *pokemon)
{
	if (!pokemon) {
		printf("Error\n");
		return;
	}

	printf("%s\n", pokemon->nombre);
	printf("%i\n", pokemon->fuerza);
	printf("%i\n", pokemon->destreza);
	printf("%i\n", pokemon->inteligencia);
}

void crear_pista_rival(TP *juego, int dificultad)
{
	int longitud_pista = 0;

	if (dificultad == FACIL) {
		longitud_pista = LONGITUD_FACIL;
	} else if (dificultad == NORMAL) {
		longitud_pista = LONGITUD_NORMAL;
	} else if (dificultad == DIFICIL) {
		longitud_pista = LONGITUD_DIFICL;
	} else if (dificultad == IMPOSIBLE) {
		longitud_pista = LONGITUD_IMPOSIBLE;
	}

	for (int i = 0; i < longitud_pista; i++) {
		enum TP_OBSTACULO obstaculo = rand() % 3;
		tp_agregar_obstaculo(juego, JUGADOR_2, obstaculo,
				     (unsigned int)i);
	}
}

void imprimir_pista(TP *juego, enum TP_JUGADOR jugador)
{
	char *obstaculos_pista = tp_obstaculos_pista(juego, jugador);
	if (!obstaculos_pista) {
		printf("La pista esta vacia no hay nada para imprimir");
		return;
	}

	for (int i = 0; i < strlen(obstaculos_pista); i++)
		printf("%c", obstaculos_pista[i]);

	free(obstaculos_pista);
}

int modificar_pista_jugador(TP *juego, enum TP_JUGADOR jugador)
{
	unsigned posicion = 0;
	int obstaculo = -1;
	int accion = 0;
	printf("Momento de crear tu pista\n");
	do {
		printf("Si queres Agregar un obstaculo a tu pista apreta 1 o Si que eliminar un obstaculo de tu pista apreta 2\n");
		printf("Para terminar de crear tu pista solo escribi -1\n");
		if (scanf("%d", &accion) == -2)
			return 1;
		if (accion == AGREGAR) {
			printf("Ingrese el obstáculo (0: FUERZA, 1: DESTREZA, 2: INTELIGENCIA): ");
			if (scanf("%d", &obstaculo) == -2)
				return 1;

			printf("Ingresa una posicion donde agrega tu obstaculo (Si pones un numero mayor al tamanio de tu pista va quedar al final)\n");
			if (scanf("%u", &posicion) == -2)
				return 1;
			if (obstaculo >= 0 && obstaculo <= 2) {
				tp_agregar_obstaculo(juego, jugador, obstaculo,
						     posicion);
			}
		} else if (accion == QUITAR) {
			printf("Ingresa un posicion donde quitar un obstaculo (Se pones un numero mayor al tamanio de tu pista no va a eliminar nada)\n");
			if (scanf("%u", &posicion) == -2)
				return 1;
			tp_quitar_obstaculo(juego, jugador, posicion);
		}
	} while (accion != -1);
	return 0;
}

int cantidad_intentos(int dificultad)
{
	if (dificultad == FACIL) {
		return INTENTOS_MODO_FACIL;
	} else if (dificultad == NORMAL) {
		return INTENTOS_MODO_NORMAL;
	} else if (dificultad == DIFICIL) {
		return INTENTOS_MODO_DIFCIL;
	} else if (dificultad == IMPOSIBLE) {
		return INTENTOS_MODO_IMPOSIBLE;
	}
	return 0;
}

int puntaje_final_carrer(TP *juego)
{
	int puntaje_jugador1 = (int)tp_calcular_tiempo_pista(juego, JUGADOR_1);
	int puntaje_jugador2 = (int)tp_calcular_tiempo_pista(juego, JUGADOR_2);

	if (puntaje_jugador1 == puntaje_jugador2)
		return 0;

	int ta = 0;
	int tb = 0;

	if (puntaje_jugador1 > puntaje_jugador2) {
		ta = puntaje_jugador1;
		tb = puntaje_jugador2;
	} else if (puntaje_jugador1 < puntaje_jugador2) {
		ta = puntaje_jugador2;
		tb = puntaje_jugador1;
	}

	int numerador = abs(ta - tb);
	int denominador = ta + tb;

	return numerador / denominador;
}

int main(int argc, char const *argv[])
{
	//Se recomienda pasar el archivo de texto con los pokemon como argumento al ejecutar el programa
	srand((unsigned int)time(NULL));

	//crear el juego
	TP *juego = tp_crear(argv[1]);
	if (!juego) {
		printf("Error al cargar los datos del Juego :(\n");
		return 1;
	}
	Menu *menu = menu_crear();
	if (!menu) {
		printf("Error al cargar el Menu :(\n");
		tp_destruir(juego);
		return 1;
	}

	//mostrar menues
	int dificultad = 0;
	printf("___Bienvenido a las carreras de Pokemones___\n");
	seleccionar_dificultad(menu, &dificultad);
	int cant_intentos = cantidad_intentos(dificultad);

	printf("Tu rival esta eligiendo a su pokemon >:)...\n");
	int num_linea_aleatorio = (rand() % tp_cantidad_pokemon(juego)) + 1;
	char *nombre_pokemon_jugador2 = pokemon_aleatorio(num_linea_aleatorio);

	tp_seleccionar_pokemon(juego, JUGADOR_2, nombre_pokemon_jugador2);
	free(nombre_pokemon_jugador2);

	const struct pokemon_info *pokemon_jugador2 =
		tp_pokemon_seleccionado(juego, JUGADOR_2);
	imprimir_pokemon(pokemon_jugador2);
	crear_pista_rival(juego, dificultad);
	imprimir_pista(juego, JUGADOR_2);

	char nombre_pokemon_jugador1[100];
	nombre_pokemon_jugador1[0] = 0;
	char *pokemons_disponibles = tp_nombres_disponibles(juego);
	printf("Ahora te toca a vos :)\n");
	printf("Elegi un pokemon de esta lista, no te preocupes por las mayusculas yo me ocupo ;)\n");
	do {
		for (int i = 0; i < strlen(pokemons_disponibles); i++) {
			if (pokemons_disponibles[i] == ',') {
				printf("\n");
			} else {
				printf("%c", pokemons_disponibles[i]);
			}
		}
		printf("\n");
		if (scanf("%s", nombre_pokemon_jugador1) == -1)
			return 1;
	} while (!tp_seleccionar_pokemon(juego, JUGADOR_1,
					 nombre_pokemon_jugador1));
	modificar_pista_jugador(juego, JUGADOR_1);
	//correr carrera
	if (puntaje_final_carrer(juego) == 0) {
		printf("Felicidades Ganaste");
		return 0;
	}
	printf("La diferencia de la carrera fue de %i\n",
	       puntaje_final_carrer(juego));
	printf("Se que lo podes hacer mejor\n");
	for (int i = 0; i < cant_intentos; i++) {
		printf("Si queres podes modfiicar tu pista\n");
		modificar_pista_jugador(juego, JUGADOR_1);

		if (puntaje_final_carrer(juego) == 0) {
			printf("Felicidades Ganaste");
			return 0;
		}

		printf("La diferencia de la carrera fue de %i\n",
		       puntaje_final_carrer(juego));
		printf("Se que lo podes hacer mejor\n");
	}
	menu_destruir(menu);
	tp_destruir(juego);
	return 0;
}
