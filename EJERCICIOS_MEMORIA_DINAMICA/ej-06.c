/* 

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

const int MAX_NOMBRE = 10;
const int MAX_TIPO = 5;

typedef struct {
    int id;
    int nivel;
    char* nombre;
    char* tipo;
} pokemon_t;


int main() {
    pokemon_t* poke = malloc(sizeof(pokemon_t));
    poke->nombre = malloc(sizeof(char) * MAX_NOMBRE);
    poke->tipo = malloc(sizeof(char) * MAX_TIPO);

    free(poke->nombre);
    free(poke->tipo);
    free(poke);

    return 0;
}
