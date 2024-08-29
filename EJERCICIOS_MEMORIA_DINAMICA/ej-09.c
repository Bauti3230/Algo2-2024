/* 

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/

#include <stdlib.h>

const size_t MAX_LARGO = 5;


int main() {
    int** vector = malloc(MAX_LARGO * sizeof(int*));
    int i = 0;
   
    for(i = 0;i < MAX_LARGO; i++){
        vector[i] = malloc(sizeof(int));
    }

    for (int j = 0; j < MAX_LARGO; j++){
        int* numero = i+j;
        vector[i] = numero;
    }

    for(int j = 0; j < MAX_LARGO; j++){
        free(vector[j]);
    }
    free(vector);

    return 0;
}

