#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define SIN_ELEMENTOS 0
#define SIN_FUNCION NULL
#define MISMO_ELEMENTO 0

nodo_abb_t *crear_nuevo_nodo_abb(void *elemento)
{
	nodo_abb_t *nuevo_nodo_abb = calloc(1, sizeof(nodo_abb_t));

	if (!nuevo_nodo_abb)
		return NULL;

	nuevo_nodo_abb->elemento = elemento;
	nuevo_nodo_abb->izquierda = NULL;
	nuevo_nodo_abb->derecha = NULL;

	return nuevo_nodo_abb;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;

	abb_t *nuevo_abb = malloc(sizeof(abb_t));

	if (!nuevo_abb)
		return NULL;

	nuevo_abb->nodo_raiz = NULL;
	nuevo_abb->comparador = comparador;
	nuevo_abb->tamanio = 0;

	return nuevo_abb;
}

nodo_abb_t *buscar_nodo(abb_comparador comparador, nodo_abb_t *nodo_actual,
			void *elemento)
{
	if (!comparador || !nodo_actual)
		return NULL;

	int comparacion = comparador(nodo_actual->elemento, elemento);

	if (comparacion == MISMO_ELEMENTO) {
		return nodo_actual;
	}

	if (comparacion < MISMO_ELEMENTO) {
		return buscar_nodo(comparador, nodo_actual->derecha, elemento);
	}
	return buscar_nodo(comparador, nodo_actual->izquierda, elemento);
}

bool se_pudo_insertar(nodo_abb_t *nodo_actual, abb_comparador comparador,
		      void *elemento)
{
	int comparacion = comparador(nodo_actual->elemento, elemento);

	if (comparacion < MISMO_ELEMENTO) {
		if (nodo_actual->derecha == NULL) {
			nodo_abb_t *nuevo_nodo = crear_nuevo_nodo_abb(elemento);
			if (!nuevo_nodo)
				return false;
			nodo_actual->derecha = nuevo_nodo;
			return true;
		} else {
			return se_pudo_insertar(nodo_actual->derecha,
						comparador, elemento);
		}
	} else {
		if (nodo_actual->izquierda == NULL) {
			nodo_abb_t *nuevo_nodo = crear_nuevo_nodo_abb(elemento);
			if (!nuevo_nodo)
				return false;
			nodo_actual->izquierda = nuevo_nodo;
			return true;
		} else {
			return se_pudo_insertar(nodo_actual->izquierda,
						comparador, elemento);
		}
		return false;
	}
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	if (abb_vacio(arbol)) {
		nodo_abb_t *nodo_a_insertar = crear_nuevo_nodo_abb(elemento);

		if (!nodo_a_insertar)
			return NULL;

		arbol->nodo_raiz = nodo_a_insertar;
		arbol->tamanio++;
		return arbol;
	}

	if (!se_pudo_insertar(arbol->nodo_raiz, arbol->comparador, elemento)) {
		return NULL;
	}

	arbol->tamanio++;
	return arbol;
}

bool abb_quitar_recursivo(abb_t *arbol, nodo_abb_t **nodo,
			  abb_comparador comparador, void *elemento)
{
	if (*nodo == NULL)
		return false;

	int comparacion = comparador((*nodo)->elemento, elemento);

	if (comparacion < MISMO_ELEMENTO) {
		return abb_quitar_recursivo(arbol, &(*nodo)->derecha,
					    comparador, elemento);
	} else if (comparacion > MISMO_ELEMENTO) {
		return abb_quitar_recursivo(arbol, &(*nodo)->izquierda,
					    comparador, elemento);
	} else {
		if ((*nodo)->izquierda == NULL) {
			nodo_abb_t *aux = *nodo;
			*nodo = (*nodo)->derecha;
			free(aux);
		} else if ((*nodo)->derecha == NULL) {
			nodo_abb_t *aux = *nodo;
			*nodo = (*nodo)->izquierda;
			free(aux);
		} else {
			nodo_abb_t *predecesor = (*nodo)->izquierda;

			while (predecesor->derecha != NULL) {
				predecesor = predecesor->derecha;
			}

			(*nodo)->elemento = predecesor->elemento;

			return abb_quitar_recursivo(arbol, &(*nodo)->izquierda,
						    comparador,
						    predecesor->elemento);
		}
		return true;
	}
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	if (!abb_quitar_recursivo(arbol, &arbol->nodo_raiz, arbol->comparador,
				  elemento))
		return NULL;
	arbol->tamanio--;
	return elemento;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol || abb_vacio(arbol))
		return NULL;

	nodo_abb_t *nodo_buscado =
		buscar_nodo(arbol->comparador, arbol->nodo_raiz, elemento);

	if (!nodo_buscado)
		return NULL;

	return nodo_buscado->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || abb_tamanio(arbol) == SIN_ELEMENTOS)
		return true;

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return SIN_ELEMENTOS;

	return arbol->tamanio;
}

void destruir_nodos(nodo_abb_t *nodo_actual, void (*destructor)(void *))
{
	if (!nodo_actual)
		return;

	destruir_nodos(nodo_actual->izquierda, destructor);
	destruir_nodos(nodo_actual->derecha, destructor);

	if (destructor)
		destructor(nodo_actual->elemento);

	free(nodo_actual);
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;

	destruir_nodos(arbol->nodo_raiz, SIN_FUNCION);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;

	destruir_nodos(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento_recursivo(nodo_abb_t *nodo,
				       abb_recorrido recorrido,
				       bool (*funcion)(void *, void *),
				       void *aux, bool *continuar)
{
	if (nodo == NULL || !continuar) {
		return 0;
	}
	size_t contador = 0;

	if (recorrido == INORDEN) {
		contador += abb_con_cada_elemento_recursivo(
			nodo->izquierda, recorrido, funcion, aux, continuar);

		if (*continuar) {
			*continuar = funcion(nodo->elemento, aux);
			contador++;
		}

		if (!*continuar) {
			return contador;
		}

		contador += abb_con_cada_elemento_recursivo(
			nodo->derecha, recorrido, funcion, aux, continuar);

	} else if (recorrido == PREORDEN) {
		if (*continuar) {
			*continuar = funcion(nodo->elemento, aux);
			contador++;
		}

		if (!*continuar) {
			return contador;
		}

		contador += abb_con_cada_elemento_recursivo(
			nodo->izquierda, recorrido, funcion, aux, continuar);

		contador += abb_con_cada_elemento_recursivo(
			nodo->derecha, recorrido, funcion, aux, continuar);
	} else if (recorrido == POSTORDEN) {
		contador += abb_con_cada_elemento_recursivo(
			nodo->izquierda, recorrido, funcion, aux, continuar);

		contador += abb_con_cada_elemento_recursivo(
			nodo->derecha, recorrido, funcion, aux, continuar);

		if (*continuar) {
			*continuar = funcion(nodo->elemento, aux);
			contador++;
		}

		if (!*continuar) {
			return contador;
		}
	}

	return contador;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (arbol == NULL || funcion == NULL)
		return 0;

	bool continuar = true;

	return abb_con_cada_elemento_recursivo(arbol->nodo_raiz, recorrido,
					       funcion, aux, &continuar);
}

size_t abb_recorrer_recursivo(nodo_abb_t *nodo, abb_recorrido recorrido,
			      void **array, size_t tamanio_array,
			      size_t *indice)
{
	if (nodo == NULL || *indice >= tamanio_array)
		return *indice;

	if (recorrido == INORDEN) {
		*indice = abb_recorrer_recursivo(nodo->izquierda, recorrido,
						 array, tamanio_array, indice);

		if (*indice < tamanio_array) {
			array[*indice] = nodo->elemento;
			(*indice)++;
		}

		*indice = abb_recorrer_recursivo(nodo->derecha, recorrido,
						 array, tamanio_array, indice);
	} else if (recorrido == PREORDEN) {
		if (*indice < tamanio_array) {
			array[*indice] = nodo->elemento;
			(*indice)++;
		}

		*indice = abb_recorrer_recursivo(nodo->izquierda, recorrido,
						 array, tamanio_array, indice);

		*indice = abb_recorrer_recursivo(nodo->derecha, recorrido,
						 array, tamanio_array, indice);

	} else if (recorrido == POSTORDEN) {
		*indice = abb_recorrer_recursivo(nodo->izquierda, recorrido,
						 array, tamanio_array, indice);

		*indice = abb_recorrer_recursivo(nodo->derecha, recorrido,
						 array, tamanio_array, indice);

		if (*indice < tamanio_array) {
			array[*indice] = nodo->elemento;
			(*indice)++;
		}
	}

	return *indice;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || array == NULL)
		return 0;

	size_t indice = 0;
	abb_recorrer_recursivo(arbol->nodo_raiz, recorrido, array,
			       tamanio_array, &indice);

	return indice;
}