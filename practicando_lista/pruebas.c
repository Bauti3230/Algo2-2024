#include "lista.h"
#include "pa2m.h"

void lista_crear_crea_una_lista_no_nula()
{
	lista_t* lista = lista_crear();
	pa2m_afirmar(lista != NULL, "la lista creada no es nula");
	lista_destruir(lista);
}

void lista_crear_crea_una_lista_vacia()
{
	lista_t* lista = lista_crear();
	pa2m_afirmar(lista_tamanio(lista) == 0, "la lista creada tiene tamanio 0");
	lista_destruir(lista);
}

void lista_obtener_devuelve_null_si_la_lista_esta_vacia()
{
	lista_t* lista = lista_crear();
	pa2m_afirmar(lista_obtener(lista,0) == NULL, "Obtener de una lista recien creada me da null");
	lista_destruir(lista);
}

void lista_insertar_aumenta_el_tamanio_de_la_lista()
{
	void* e1;
	lista_t* lista = lista_crear();
	lista_insertar(lista, e1);
	pa2m_afirmar(lista_tamanio(lista) == 1, "Al insertar un elemento, la lista crece");
	lista_destruir(lista);	
}

void lista_obtener_recupera_los_elementos_insertados()
{
	void* e1 = (void*)1;
	lista_t* lista = lista_crear();
	lista_insertar(lista, e1);

	pa2m_afirmar(lista_obtener(lista,0) == e1, "Al obtener el primer elemento de la lista, obtengo el primer elemento insertado");

	lista_destruir(lista);	
}

int main(int argc, char const *argv[])
{
	pa2m_nuevo_grupo("Pruebas de creacion");
	lista_crear_crea_una_lista_no_nula();
	lista_crear_crea_una_lista_vacia();
	lista_obtener_devuelve_null_si_la_lista_esta_vacia();

	pa2m_nuevo_grupo("Pruebas de insercion");
	lista_insertar_aumenta_el_tamanio_de_la_lista();
	lista_obtener_recupera_los_elementos_insertados();

	return pa2m_mostrar_reporte();
}