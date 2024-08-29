#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t tamanio;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *nodo_actual;
};

nodo_t *crear_nuevo_nodo(void *dato)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));

	if (nuevo_nodo == NULL)
		return NULL;

	nuevo_nodo->elemento = dato;
	nuevo_nodo->siguiente = NULL;

	return nuevo_nodo;
}

lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));

	if (lista == NULL)
		return NULL;

	lista->nodo_inicio = NULL;
	lista->nodo_final = NULL;
	lista->tamanio = 0;

	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL) {
		return NULL;
	}

	nodo_t *nuevo_nodo = crear_nuevo_nodo(elemento);

	if (nuevo_nodo == NULL) {
		return NULL;
	}

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_final = nuevo_nodo;
		lista->tamanio++;
		return lista;
	}

	lista->nodo_final->siguiente = nuevo_nodo;
	lista->nodo_final = nuevo_nodo;
	lista->tamanio++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL) {
		return NULL;
	}

	if (posicion >= lista->tamanio) {
		lista_insertar(lista, elemento);
		return lista;
	}

	nodo_t *nuevo_nodo = crear_nuevo_nodo(elemento);

	if (nuevo_nodo == NULL) {
		return NULL;
	}

	nodo_t *nodo_aux = lista->nodo_inicio;

	if (posicion == 0) {
		lista->nodo_inicio = nuevo_nodo;
		lista->nodo_inicio->siguiente = nodo_aux;
		lista->tamanio++;
		return lista;
	}

	size_t contador = 0;

	while (contador < posicion - 1) {
		nodo_aux = nodo_aux->siguiente;
		contador++;
	}

	nuevo_nodo->siguiente = nodo_aux->siguiente;
	nodo_aux->siguiente = nuevo_nodo;
	lista->tamanio++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista == NULL || lista_vacia(lista)) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_anterior = NULL;

	if (lista->nodo_inicio->siguiente == NULL) {
		void *dato = lista->nodo_inicio->elemento;
		free(lista->nodo_inicio);
		lista->tamanio--;
		return dato;
	}

	while (nodo_actual->siguiente != NULL) {
		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	void *dato = nodo_actual->elemento;

	lista->nodo_final = nodo_anterior;
	lista->nodo_final->siguiente = NULL;

	free(nodo_actual);
	lista->tamanio--;

	return dato;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL) {
		return NULL;
	}

	if (posicion >= lista->tamanio) {
		void *dato = lista_quitar(lista);
		return dato;
	}

	nodo_t *nodo_a_quitar;
	size_t contador = 0;

	if (posicion == 0) {
		nodo_a_quitar = lista->nodo_inicio;
		lista->nodo_inicio = lista->nodo_inicio->siguiente;
	} else {
		nodo_t *nodo_actual = lista->nodo_inicio;
		while (contador < posicion - 1) {
			nodo_actual = nodo_actual->siguiente;
			contador++;
		}
		nodo_a_quitar = nodo_actual->siguiente;
		nodo_actual->siguiente = nodo_a_quitar->siguiente;
	}

	void *dato = nodo_a_quitar->elemento;
	free(nodo_a_quitar);
	lista->tamanio--;

	return dato;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL) {
		return NULL;
	}

	if (posicion >= lista->tamanio) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	size_t contador = 0;

	while (contador < posicion) {
		nodo_actual = nodo_actual->siguiente;
		contador++;
	}

	return nodo_actual->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || lista_vacia(lista) || !comparador) {
		return NULL;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;

	while (nodo_actual) {
		if (comparador(nodo_actual->elemento, contexto) == 0) {
			return nodo_actual->elemento;
		}
		nodo_actual = nodo_actual->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista)) {
		return NULL;
	}

	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL || lista->tamanio == 0) {
		return true;
	}
	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista) {
		return 0;
	}

	return lista->tamanio;
}

void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;

	if (lista_vacia(lista)) {
		free(lista);
		return;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_aux;

	while (nodo_actual->siguiente != NULL) {
		nodo_aux = nodo_actual;
		nodo_actual = nodo_actual->siguiente;

		free(nodo_aux);
	}

	free(nodo_actual);

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista) {
		return;
	}

	nodo_t *nodo_actual = lista->nodo_inicio;
	nodo_t *nodo_aux;

	while (nodo_actual) {
		nodo_aux = nodo_actual;
		nodo_actual = nodo_actual->siguiente;

		if (funcion) {
			funcion(nodo_aux->elemento);
		}

		free(nodo_aux);
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *it = malloc(sizeof(lista_iterador_t));

	if (!it) {
		return NULL;
	}

	it->lista = lista;
	it->nodo_actual = lista->nodo_inicio;

	return it;
} //esta

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista)) {
		return false;
	}

	return iterador->nodo_actual != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista))
		return false;

	if (iterador->nodo_actual) {
		iterador->nodo_actual = iterador->nodo_actual->siguiente;
	}

	return lista_iterador_tiene_siguiente(iterador);
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->nodo_actual) {
		return NULL;
	}

	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador)
		return;
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion) {
		return 0;
	}

	if (lista_vacia(lista)) {
		return 0;
	}

	size_t elementos_iterados = 0;
	nodo_t *nodo_actual = lista->nodo_inicio;

	while (nodo_actual) {
		(elementos_iterados)++;

		if (funcion(nodo_actual->elemento, contexto) == false) {
			return elementos_iterados;
		}

		nodo_actual = nodo_actual->siguiente;
	}

	return elementos_iterados;
}