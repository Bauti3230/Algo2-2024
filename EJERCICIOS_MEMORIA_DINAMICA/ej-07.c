/*

    El código presentado pierde memoria. Libérela. Puede asumir que los alloc no fallan.

*/


#include <stdlib.h>

int main() {
    int *num = malloc(sizeof(int) * 3);
    num = realloc(num, sizeof(int));
    free(num);

    return 0;
}
