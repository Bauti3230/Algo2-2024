#include "cola.h"
#include "lista.h"

#define INICIO_COLA 0

cola_t *cola_crear()
{
	cola_t *nueva_cola = (cola_t *)lista_crear();

	if (!nueva_cola)
		return NULL;

	return nueva_cola;
}

cola_t *cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola)
		return NULL;

	return (cola_t *)lista_insertar_en_posicion((lista_t *)cola, elemento,
						    INICIO_COLA);
}

void *cola_desencolar(cola_t *cola)
{
	if (!cola)
		return NULL;

	return lista_quitar((lista_t *)cola);
}

void *cola_frente(cola_t *cola)
{
	if (!cola)
		return NULL;

	return lista_ultimo((lista_t *)cola);
}

size_t cola_tamanio(cola_t *cola)
{
	if (!cola)
		return 0;

	return lista_tamanio((lista_t *)cola);
}

bool cola_vacia(cola_t *cola)
{
	return lista_vacia((lista_t *)cola);
}

void cola_destruir(cola_t *cola)
{
	lista_destruir((lista_t *)cola);
}
