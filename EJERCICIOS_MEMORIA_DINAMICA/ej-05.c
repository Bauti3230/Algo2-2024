/*

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

int main() {
    int **vector = malloc(sizeof(int *) * 2);
    *(vector) = malloc(sizeof(int));
    *(vector+1) = malloc(sizeof(int));
    
    vector[0][0] = 8;
    vector[1][0] = 18;

    free(*vector);
    free(*(vector+1));
    free(vector);
    

    return 0;
}
