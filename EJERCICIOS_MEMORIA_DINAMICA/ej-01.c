/*

    El código presentado pierde memoria. Libérela. Puede asumir que el malloc no falla.

*/


#include <stdlib.h>

int main() {
    int *num = malloc(sizeof(int));
    free(num);

    return 0;
}
