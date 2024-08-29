/*

    --Ejercicio de parcial--

    Este código pierde memoria. Libérela. Puede asumir que los mallocs no fallan.
    
*/

#include <stdio.h>
#include <stdlib.h>

const size_t MAX_ANCHO = 5;
const size_t MAX_ALTO = 5;


int main() {
    int*** matriz = malloc(MAX_ALTO * sizeof(int**));
    for (int i = 0; i < MAX_ALTO; i++)
        matriz[i] = malloc(MAX_ANCHO * sizeof(int*));

    for (int i = 0; i < MAX_ALTO; i++){
        int* numero = malloc(sizeof(int));

        for (int j = 0; j < MAX_ANCHO; j++){
            *numero = i+j;
            matriz[i][j] = numero;
            *numero = i*j;
        }
    }
 
    for (int i = 0; i < MAX_ANCHO; i++){
        for (int j = 0; j < MAX_ALTO; j++){
            printf("%i\t", *(matriz[i][j]));
        }
        printf("\n");
    }

 return 0;
}