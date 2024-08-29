/*
Dado el tipo recursivo nodo_t, y sin usar while / for / do, complete:
a. La función calcular_altura que cuente la cantidad de nodos existentes desde la raíz hasta la hoja mas alejada.
*/
#include <stdlib.h> // Incluimos stdlib para usar la función max()

typedef struct nodo {
    int fe;
    struct nodo* i;
    struct nodo* d;
} nodo_t;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int calcular_altura(nodo_t* raiz) {
    // Caso base: si el árbol está vacío, su altura es 0
    if (raiz == NULL) {
        return 0;
    } else {
        // Calculamos la altura de los subárboles izquierdo y derecho recursivamente
        int altura_izquierda = calcular_altura(raiz->i);
        int altura_derecha = calcular_altura(raiz->d);

        // La altura del árbol es la máxima altura de los subárboles más 1 (por el nodo actual)
        return 1 + max(altura_izquierda, altura_derecha);
    }
}

int completar_factor_de_equilibrio_recursivo(nodo_t* nodo) {
    // Caso base: si el nodo es NULL, su factor de equilibrio es 0
    if (nodo == NULL) {
        return 0;
    }

    // Calculamos la altura de los subárboles izquierdo y derecho recursivamente
    int altura_izquierda = completar_factor_de_equilibrio_recursivo(nodo->i);
    int altura_derecha = completar_factor_de_equilibrio_recursivo(nodo->d);

    // Actualizamos el factor de equilibrio del nodo como la diferencia de alturas de los subárboles
    nodo->fe = altura_derecha - altura_izquierda;

    // Devolvemos la altura del subárbol en el que estamos (máxima altura de los subárboles + 1)
    return 1 + ((altura_izquierda > altura_derecha) ? altura_izquierda : altura_derecha);
}