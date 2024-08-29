#include "split.h"
#include <stdlib.h>
#include <string.h>

#define CARACTER_NULO '\0'

void liberar_memoria(char **vector_strings, int tope)
{
	for (int i = 0; i < tope; i++) {
		free(vector_strings[i]);
	}
	free(vector_strings);
}

void copiar_palabra(const char *string_origen, char *string_destino,
		    int posicion_copia, int cant_caracteres)
{
	strncpy(string_destino, string_origen + posicion_copia,
		(size_t)cant_caracteres);
	string_destino[cant_caracteres] = CARACTER_NULO;
}

char **split(const char *string, char separador)
{
	if (string == NULL) {
		return NULL;
	}

	int contador_palabras = 1;
	int caracter = 0;

	while (string[caracter] != CARACTER_NULO) {
		if (string[caracter] == separador) {
			contador_palabras++;
		}
		caracter++;
	}

	char **vector_strings =
		malloc((size_t)(contador_palabras + 1) * sizeof(char *));
	if (vector_strings == NULL) {
		return NULL;
	}

	int posicion = 0;
	int inicio_palabra = 0;
	caracter = 0;
	while (string[caracter] != CARACTER_NULO) {
		if (string[caracter] == separador) {
			int longitud_palabra = caracter - inicio_palabra;
			vector_strings[posicion] = malloc(
				(size_t)(longitud_palabra + 1) * sizeof(char));
			if (vector_strings[posicion] == NULL) {
				liberar_memoria(vector_strings, posicion);
				return NULL;
			}
			copiar_palabra(string, vector_strings[posicion],
				       inicio_palabra, longitud_palabra);
			posicion++;
			inicio_palabra = caracter + 1;
		}
		caracter++;
	}

	int longitud_palabra = caracter - inicio_palabra;
	vector_strings[posicion] =
		malloc((size_t)(longitud_palabra + 1) * sizeof(char));

	if (vector_strings[posicion] == NULL) {
		liberar_memoria(vector_strings, posicion);
		return NULL;
	}
	copiar_palabra(string, vector_strings[posicion], inicio_palabra,
		       longitud_palabra);
	posicion++;

	vector_strings[posicion] = NULL;

	return vector_strings;
}