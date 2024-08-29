//a. Definir el TDA Gimnasio pokemon (strcut y primitivas)

#include <stdlib.h>
#include <stdio.h>

typedef struct entrnador_pokemon
{
	char nombre_entrenador[MAX_NOMBRE];
	pokemon_t** equipo;
}entrnador_t;

typedef struct pokemon
{
	char nombre_pokemon[MAX_NOMBRE];
	char tipo[MAX_TIPO];
	size_t poder;
}

typedef struct gimnasio_pokemon
{
	entrnador_t* lider_de_gimnasio;
	char especialidad[MAX_ESPECIALIAD];
	size_t entrenadores_derrotados;
	size_t medallas_entregadas;
}gimnasio_pokemon_t;

//b. Implementar una de las primitivas propuestas que utilice memoria dinámica.

entrnador_t* crear_nuevo_entrandro(char* nombre_entrenador, pokemon_t** equipo)
{
	entrnador_t* nuevo_entrenador = calloc(1,sizeof(entrenador_t));

	if(!nuevo_entrenador)
		return NULL;

	strcpy(nuevo_entrenador->nombre,nombre_entrenador);
	nombre_entrenador->equipo = calloc(1,sizeof(pokemon_t*));

	if(!nuevo_entrenador->equipo) {
		free(nuevo_entrenador);
		return NULL;
	}

	memcpy(nuevo_entrenador->equipo,equipo);

	return nuevo_entrenador;
}

