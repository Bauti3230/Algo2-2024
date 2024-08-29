/*

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

int main() {
    int **vector = malloc(sizeof(int *));
    *vector = malloc(sizeof(int));

    vector[0][0] = 8;

    free(*vector);
    free(vector);

    return 0;
}
