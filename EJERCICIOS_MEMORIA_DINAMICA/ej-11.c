/* 

    El código presentado pierde memoria. Libérela. Puede asumir que los alloc no fallan.

*/


#include <stdlib.h>

const int MAX_NOMBRE = 10;
const int MAX_TIPO = 5;
const int MAX_POKES_INICIAL = 3;
const int MAX_POKES_NUEVO = 6;

typedef struct {
    int id;
    int nivel;
    char* nombre;
    char* tipo;
} pokemon_t;


int main() {
    pokemon_t** pokes = malloc(sizeof(pokemon_t*) * MAX_POKES_INICIAL);

    for (int i = 0; i < MAX_POKES_INICIAL; i++) {
        pokemon_t* poke = malloc(sizeof(pokemon_t));
        poke->nombre = malloc(sizeof(char) * MAX_NOMBRE);
        poke->tipo = malloc(sizeof(char) * MAX_TIPO);

        pokes[i] = poke;
    }

    pokes = realloc(pokes, sizeof(pokemon_t*) * MAX_POKES_NUEVO);

    for (int i = MAX_POKES_INICIAL; i < MAX_POKES_NUEVO; i++) {
        pokemon_t* poke = malloc(sizeof(pokemon_t));
        poke->nombre = malloc(sizeof(char) * MAX_NOMBRE);
        poke->tipo = malloc(sizeof(char) * MAX_TIPO);

        pokes[i] = poke;
    }
    

    return 0;
}
