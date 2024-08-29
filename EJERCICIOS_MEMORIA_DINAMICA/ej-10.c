/* 

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

const int MAX_NOMBRE = 10;
const int MAX_TIPO = 5;
const int MAX_POKES = 3;

typedef struct {
    int id;
    int nivel;
    char* nombre;
    char* tipo;
} pokemon_t;


int main() {
    pokemon_t** pokes = malloc(sizeof(pokemon_t*) * MAX_POKES);

    for (int i = 0; i < MAX_POKES; i++) {
        pokemon_t* poke = malloc(sizeof(pokemon_t));
        pokes->nombre = malloc(sizeof(char) * MAX_NOMBRE);
        pokes->tipo = malloc(sizeof(char) * MAX_TIPO);

        pokes[i] = poke;
    }

    for(int j = 0; j < MAX_POKES; j++){
        free(pokes->nombre);
        free(pokes->tipo);
    }

    for(int i = 0; i < MAX_POKES; i++){
        free(pokes[i]);
    }

    free(pokes);

    

    return 0;
}
