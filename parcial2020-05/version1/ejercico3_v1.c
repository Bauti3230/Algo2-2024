#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pokemon
{
	char tipo[MAX_TIPO];
	char nombre[MAX_NOMBRE];
	size_t nivel;
}pokemon_t;

typedef struct entrnador_pokemon
{
	char nombre_entrenador[MAX_NOMBRE];
	char ciudad_natal[MAX_CIUDAD];
	char distincion[MAX_DISTINCION];
	size_t victorias;
	size_t derrotas;
	size_t pokeballs;
	size_t cantidad_capturada;
	size_t cantidad_en_equipo;
	pokemon_t** equipo;
	pokemon_t** computadora;
}entrnador_t;

pokemon_t* crear_nuevo_pokemon(char* nombre,size_t nivel,char* tipo)
{
	pokemon_t* nuevo_pokemon = calloc(1,sizeof(pokemon_t));

	if(!nuevo_pokemon)
		return NULL;

	strcpy(nuevo_pokemon->nombre,nombre);
	strcpy(nuevo_pokemon->tipo,tipo);
	nuevo_pokemon->nivel = nivel;

	return nuevo_pokemon;
}

//b)
void renombrar_pokeon(char* nuevo_nombre,pokemon_t** equipo_de_entrenador)
{
	int i = 0;
	bool continuar_iteracion = true
	while (continuar_iteracion) {
		if (strcmp(nuevo_nombre,equipo_de_entrenador[i]) == 0) {
			continuar_iteracion = false;
			strcpy(equipo_de_entrenador[i]->nombre,nuevo_nombre);
			return ;
		}
		else{
			i++;
			if(!equipo_de_entrenador[i]) {
				continuar_iteracion = false;
				return;
			}
		}
	}
}

//c
/*
* Creará un entrenador pokemon.
* Reservará toda la memoria necesaria e inicializará las variables que lo requieran.
* Devolverá la referencia al pokemon creado o NULL si no pudo crearse.
* Puede agregar parámetros si lo desea.
*/
entrenador_t* crear_entrenador(char nombre[MAX_NOMBRE], char ciudad_natal[MAX_CIUDAD]
	 size_t victorias, size_t derrotas, pokemon_t** equipo, pokemon_t** almacenados, size_t pokeballs) {
	entrenador_t* nuevo_entrenador = calloc(1,sizeof(entrenador_t));

	if(!nuevo_entrenador)
		return NULL;

	strcpy(nuevo_entrenador->nombre,nombre);
	strcpy(nuevo_entrenador->ciudad_natal,ciudad_natal);
	nuevo_entrenador->victorias = victorias;
	nuevo_entrenador->derrotas = derrotas;
}
