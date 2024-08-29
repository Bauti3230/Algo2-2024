/* 

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

typedef struct {
    int id;
    int nivel;
    char* nombre;
    char* tipo;
} pokemon_t;


int main() {
    pokemon_t* poke = malloc(sizeof(pokemon_t));
    free(poke);

    return 0;
}

