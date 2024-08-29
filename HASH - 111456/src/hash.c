#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hash.h"

#define PAR_VACIO NULL
#define MISMA_CLAVE 0
#define VACIO 0
#define FACTOR_CARGA_MAXIMA 0.7
#define CONSTATE_DE_AJUSTE 2
#define CAPACIDAD_MINIMA 3

typedef struct par {
	char *key;
	void *value;
	bool borrado;
} par_t;

struct hash {
	par_t *tabla;
	size_t cantidad;
	size_t capacidad;
};

size_t funcion_hash(const char *clave)
{
	size_t hash = 5381;
	int c;

	while ((c = *clave++))
		hash = ((hash << 5) + hash) + (size_t)c;

	return hash;
}

hash_t *rehash(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
	if (!nuevo_hash)
		return NULL;

	for (size_t i = 0; i < hash->capacidad; i++) {
		hash_insertar(nuevo_hash, hash->tabla[i].key,
			      hash->tabla[i].value, NULL);
	}

	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;

	hash_destruir(nuevo_hash);
	return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < CAPACIDAD_MINIMA)
		capacidad = CAPACIDAD_MINIMA;

	hash_t *hash = malloc(sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->capacidad = capacidad;
	hash->cantidad = VACIO;

	hash->tabla = calloc(capacidad, sizeof(par_t));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

	return hash;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	if (((float)hash->cantidad / (float)hash->capacidad) >=
	    FACTOR_CARGA_MAXIMA)
		if (!rehash(hash))
			return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	size_t posicion_borrada = 0;
	bool fue_borrado = false;

	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;

		if (hash->tabla[posicion_actual].key == NULL) {
			if (hash->tabla[posicion_actual].borrado &&
			    !fue_borrado) {
				posicion_borrada = posicion_actual;
				fue_borrado = true;
			} else {
				if (fue_borrado)
					posicion_actual = posicion_borrada;

				hash->tabla[posicion_actual].key = malloc(
					(strlen(clave) + 1) * sizeof(char));
				strcpy(hash->tabla[posicion_actual].key, clave);
				if (!hash->tabla[posicion_actual].key)
					return NULL;

				hash->tabla[posicion_actual].value = elemento;
				if (anterior)
					*anterior = NULL;

				hash->cantidad++;
				return hash;
			}
		} else if (strcmp(hash->tabla[posicion_actual].key, clave) ==
			   MISMA_CLAVE) {
			if (anterior)
				*anterior = hash->tabla[posicion_actual].value;
			hash->tabla[posicion_actual].value = elemento;
			return hash;
		}
	}
	return NULL;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion_actual = funcion_hash(clave) % hash->capacidad;

	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion = (posicion_actual + i) % hash->capacidad;

		if (hash->tabla[posicion].key == NULL) {
			if (!hash->tabla[posicion].borrado)
				return NULL;
		} else if (strcmp(hash->tabla[posicion].key, clave) ==
			   MISMA_CLAVE) {
			free(hash->tabla[posicion].key);
			hash->tabla[posicion].key = NULL;
			hash->tabla[posicion].borrado = true;

			hash->cantidad--;
			return hash->tabla[posicion].value;
		}
	}
	return NULL;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		if (hash->tabla[posicion_actual].key == NULL) {
			if (!hash->tabla[posicion_actual].borrado)
				return NULL;
		} else if (strcmp(hash->tabla[posicion_actual].key, clave) ==
			   MISMA_CLAVE) {
			return hash->tabla[posicion_actual].value;
		}
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!clave || !hash)
		return false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	for (size_t i = 0; i < hash->capacidad; i++) {
		size_t posicion_actual = (posicion + i) % hash->capacidad;
		if (hash->tabla[posicion_actual].key == NULL) {
			if (!hash->tabla[posicion_actual].borrado)
				return false;
		} else if (strcmp(hash->tabla[posicion_actual].key, clave) ==
			   MISMA_CLAVE) {
			return true;
		}
	}
	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;

	for (int i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].key)
			free(hash->tabla[i].key);
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	for (int i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].key) {
			if (destructor)
				destructor(hash->tabla[i].value);
		}
		free(hash->tabla[i].key);
	}
	free(hash->tabla);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	if (!hash || !f)
		return 0;

	size_t llamadas_a_funcion = 0;
	for (int i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].key) {
			llamadas_a_funcion++;
			if (!f(hash->tabla[i].key, hash->tabla[i].value, aux))
				return llamadas_a_funcion;
		}
	}
	return llamadas_a_funcion;
}
