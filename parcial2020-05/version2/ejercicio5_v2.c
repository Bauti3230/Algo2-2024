//a. La función contar_hojas que cuente la cantidad de nodos hoja de la estructura.
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo 
{
struct nodo* i;
struct nodo* d;
} nodo_t;

// Función recursiva para contar las hojas
int contar_hojas(nodo_t* nodo) {
    // Si el nodo es NULL, no hay hojas
    if (nodo == NULL)
        return 0;
    // Si es un nodo hoja, retornamos 1
    if (nodo->i == NULL && nodo->d == NULL)
        return 1;
    // Recursivamente contar las hojas en los subárboles izquierdo y derecho
    return contar_hojas(nodo->i) + contar_hojas(nodo->d);
}

// Función recursiva para contar nodos con dos hijos
int contar_nodos_con_dos_hijos(nodo_t* nodo) {
    // Si el nodo es NULL o es una hoja, no tiene dos hijos
    if (nodo == NULL || (nodo->i == NULL && nodo->d == NULL))
        return 0;
    // Si tiene dos hijos, sumamos 1 y continuamos recursivamente con los subárboles
    if (nodo->i != NULL && nodo->d != NULL)
        return 1 + contar_nodos_con_dos_hijos(nodo->i) + contar_nodos_con_dos_hijos(nodo->d);
    // Si tiene solo un hijo, continuamos recursivamente con el hijo que tiene
    return contar_nodos_con_dos_hijos(nodo->i) + contar_nodos_con_dos_hijos(nodo->d);
}

nodo_t* crear_nodo() {
    nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
    if (!nodo) {
        printf("Error: No se pudo asignar memoria para el nodo\n");
        exit(EXIT_FAILURE);
    }
    nodo->i = NULL;
    nodo->d = NULL;
    return nodo;
}

int main() {
    // Construimos un árbol de ejemplo
    nodo_t* raiz = crear_nodo();
    raiz->i = crear_nodo();
    raiz->d = crear_nodo();
    raiz->i->i = crear_nodo();
    raiz->i->d = crear_nodo();
    raiz->d->i = crear_nodo();
    raiz->d->d = crear_nodo();

    // Contamos la cantidad de hojas en el árbol y mostramos el resultado
    int cantidad_hojas = contar_nodos_con_dos_hijos(raiz);
    printf("Cantidad de hojas en el arbol: %d\n", cantidad_hojas);

    // Liberamos la memoria asignada para el árbol
    free(raiz->i->i);
    free(raiz->i->d);
    free(raiz->d->i);
    free(raiz->d->d);
    free(raiz->i);
    free(raiz->d);
    free(raiz);

    return 0;
}