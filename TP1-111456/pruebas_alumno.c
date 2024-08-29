#include "pa2m.h"
#include "src/lista.h"
#include <stdlib.h>
/*void prueba_simple()
{
	int i = 14;
	int j = 16;
	pa2m_afirmar(i != j, "i=14 es diferente de j=16");
}*/

int funcion_comp_num(void *elemento, void *contexto)
{
	if (contexto == NULL) {
		return -1;
	}

	int *elemento_lista = (int *)elemento;
	int *elemento_buscado = (int *)contexto;

	if (*elemento_lista - *elemento_buscado == 0) {
		return 0;
	}

	return -1;
}

int funcion_comp_void(void *elemento, void *contexto)
{
	if (elemento == contexto)
		return 0;

	return -1;
}

void lista_crear_crea_una_lista_valida()
{
	lista_t *lista = lista_crear();
	pa2m_afirmar(lista != NULL, "la lista creada es distinta de null");
	pa2m_afirmar(lista_tamanio(lista) == 0,
		     "la lista creada esta vacia (su tamanio es 0)");
	lista_destruir(lista);
}

void lista_insertar_aumenta_el_tamanio_de_la_lista()
{
	void *e1 = (void *)4;
	void *e2 = (void *)6;
	lista_t *lista = lista_crear();
	lista = lista_insertar(lista, e1);
	pa2m_afirmar(lista_tamanio(lista) == 1,
		     "la lista tiene un elemento, entonces es de tamanio 1");
	lista = lista_insertar(lista, e2);
	pa2m_afirmar(lista_tamanio(lista) == 2,
		     "la lista tiene dos elemento, entonces es de tamanio 2");
	lista_destruir(lista);
}

void lista_insertar_inserta_el_elemento_al_final()
{
	int e1 = 5;
	int e2 = 4;
	lista_t *lista = NULL;
	pa2m_afirmar(lista_insertar(lista, &e1) == NULL,
		     "se prueba insertar al final en una lista nula");
	lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	pa2m_afirmar(
		lista_primero(lista) == lista_ultimo(lista),
		"se prueba insertar al final en una lista vacia y se verifica en la posicion correcta");
	lista = lista_insertar(lista, &e2);
	pa2m_afirmar(
		lista_ultimo(lista) == &e2,
		"se prueba insertar al final en una lista con elementos y se verifica que esta en la posicion correcta");
	lista = lista_insertar(lista, NULL);
	pa2m_afirmar(lista_ultimo(lista) == NULL,
		     "se prueba insertar al final un null");
	lista_destruir(lista);
}

void lista_insertar_en_posicion_inserta_el_elemento_en_la_posicion_indicada()
{
	int e1 = 5;
	int e2 = 4;
	int e3 = 9;
	lista_t *lista = NULL;
	pa2m_afirmar(
		lista_insertar_en_posicion(lista, &e1, 20) == NULL,
		"se prueba insertar en cualquier posicion en una lista nula");
	lista = lista_crear();
	lista = lista_insertar_en_posicion(lista, &e1, 100);
	pa2m_afirmar(
		lista_primero(lista) == lista_ultimo(lista),
		"se prueba insertar en cualquier posicion en una lista vacia y se verifica que este en la posicion correcta");
	lista = lista_insertar_en_posicion(lista, &e2, 0);
	pa2m_afirmar(
		lista_primero(lista) == &e2,
		"se prueba insertar en cualquier posicion en una lista con elementos y verifica que esta en la posicion correcta");
	lista = lista_insertar_en_posicion(lista, &e3, 200);
	pa2m_afirmar(
		lista_ultimo(lista) == &e3,
		"se prueba insertar en una posicion inexistente en la lista, y se verifica que esta en la posicion correcta");
	lista_destruir(lista);
}

void lista_quitar_elimina_y_devuelve_el_ultimo_elemento()
{
	int e1 = 5;
	int e2 = 4;
	lista_t *lista = NULL;
	pa2m_afirmar(lista_quitar(lista) == NULL,
		     "se prueba quitar de una lista nula");
	lista = lista_crear();
	pa2m_afirmar(lista_quitar(lista) == NULL,
		     "se prueba quitar de una lista vacia");
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	pa2m_afirmar(
		lista_quitar(lista) == &e2,
		"se prueba quitar el ultimo elemento de una lista con elementos");
	pa2m_afirmar(
		lista_ultimo(lista) == &e1,
		"se verifica que despues de eliminar el ultimo elemento ahora el ultimo elemento es el anterior");
	lista_destruir(lista);
}

void lista_quitar_en_posicon_elimina_y_devuelve_el_dato_en_la_posicion_pasada_por_parametro()
{
	int e1 = 90;
	int e2 = 100;
	int e3 = 300;
	lista_t *lista = NULL;
	pa2m_afirmar(lista_quitar_de_posicion(lista, 20) == NULL,
		     "se prueba quitar de cualquier posicon de una lista nula");
	lista = lista_crear();
	pa2m_afirmar(
		lista_quitar_de_posicion(lista, 10) == NULL,
		"se prueba quitar de cualquier posicon de una lista vacia");
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	pa2m_afirmar(
		lista_quitar_de_posicion(lista, 1) == &e2,
		"se prueba quitar un elemento de cualquier posicion de una lista con elementos");
	lista_destruir(lista);
}

void lista_elemento_en_posicion_devuelve_en_elemento_en_la_posicion_pasada_por_parametro()
{
	lista_t *lista = NULL;
	pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == NULL,
		     "se prueba obtener un elemento de una lista nula ");
	lista = lista_crear();
	pa2m_afirmar(lista_elemento_en_posicion(lista, 1) == NULL,
		     "se prueba obtener un elemento de una lista vacia");
	int e1 = 90;
	int e2 = 200;
	char e3 = 'a';
	char *e4 = "90";
	lista = lista_insertar(lista, &e1); //0
	lista = lista_insertar(lista, &e2); //1
	lista = lista_insertar(lista, &e3); //2
	lista = lista_insertar(lista, &e4); //3
	pa2m_afirmar(
		lista_elemento_en_posicion(lista, 20) == NULL,
		"se prueba obtener un elemento de una posicion inexistente");
	pa2m_afirmar(
		lista_elemento_en_posicion(lista, 3) == &e4,
		"se prueba obtener un elemento de una posicion existente de la lista");
	lista_destruir(lista);
}

void lista_buscar_elemento_devuelve_el_primer_elemento_igual_a_contexto_o_null_si_no_existe()
{
	lista_t *lista = NULL;
	int contexto = 0;
	int contexto2 = 19;
	pa2m_afirmar(lista_buscar_elemento(lista, NULL, &contexto) == NULL,
		     "se prueba buscar un elemento en una lista nula");
	lista = lista_crear();
	pa2m_afirmar(lista_buscar_elemento(lista, NULL, &contexto) == NULL,
		     "se prueba buscar un elemento en una lista vacia");
	int e1 = 0;
	int e2 = 12;
	int e3 = 4;
	int e4 = 0;
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	lista = lista_insertar(lista, &e1);
	pa2m_afirmar(
		lista_buscar_elemento(lista, funcion_comp_num, &contexto2) ==
			NULL,
		"se prueba buscar un elemento que no existente en la lista");
	pa2m_afirmar(
		lista_buscar_elemento(lista, funcion_comp_num, &contexto) ==
			&e1,
		"se prueba buscar un elemento que si existente en la lista");
	pa2m_afirmar(lista_buscar_elemento(lista, funcion_comp_num,
					   &contexto) != &e4,
		     "se prueba buscar un elemento repetido en la lista");
	lista_destruir(lista);
}

void lista_iterador_crear_duevulve_un_puntero_a_iterador_o_null()
{
	int e1 = 1;
	int e2 = 2;
	int e3 = 3;
	lista_t *lista = NULL;
	lista_iterador_t *iterador = NULL;
	pa2m_afirmar(lista_iterador_crear(lista) == NULL,
		     "se prueba crear un iterador externo con una lista nula");
	lista = lista_crear();
	iterador = lista_iterador_crear(lista);
	pa2m_afirmar(iterador != NULL,
		     "se prueba crear un iterador externo con una lista vacia");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
	lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	iterador = lista_iterador_crear(lista);
	pa2m_afirmar(
		iterador != NULL,
		"se prueba crear una iterador externo con una lista con elementos");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

void lista_iterador_avanzar_avanza_o_devuelve_false()
{
	lista_iterador_t *iterador = NULL;
	pa2m_afirmar(lista_iterador_avanzar(iterador) == false,
		     " se prueba avanzar un interador nulo");
	int e1 = 1;
	int e2 = 2;
	int e3 = 3;
	lista_t *lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	iterador = lista_iterador_crear(lista);
	pa2m_afirmar(
		lista_iterador_avanzar(iterador) == true,
		"se prueba avanzar un interador que todavia tiene elemento para iterar");
	pa2m_afirmar(
		lista_iterador_avanzar(iterador) == true,
		"se prueba avanzar un interador que todavia tiene elemento para iterar");
	pa2m_afirmar(
		lista_iterador_avanzar(iterador) == false,
		"se prueba avanzar un interador que ya no tiene elementos para iterar");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

void iterar_sobre_el_elemento_actual_de_la_lista()
{
	lista_iterador_t *iterador = NULL;
	pa2m_afirmar(
		lista_iterador_elemento_actual(iterador) == NULL,
		"se prueba iterar sobre el elemento actual de un iterador nulo");
	int e1 = 1;
	lista_t *lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	iterador = lista_iterador_crear(lista);
	int *elemento_actual = (int *)lista_iterador_elemento_actual(iterador);
	(*elemento_actual)++;
	int *primer_elemento_lista = (int *)lista_primero(lista);
	pa2m_afirmar(
		*elemento_actual == *primer_elemento_lista,
		"se prueba ieterar sobre el elemento actual de un iterador que todavia tiene elementos para iterar");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

void lista_iterador_tiene_siguiente_devuelve_true_si_tiene_o_false_si_no()
{
	lista_iterador_t *iterador = NULL;
	pa2m_afirmar(
		lista_iterador_tiene_siguiente(iterador) == false,
		"se prueba verificar si un iterador nulo tiene mas elementos para iterar");
	int e1 = 1;
	int e2 = 2;
	int e3 = 3;
	lista_t *lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	iterador = lista_iterador_crear(lista);
	pa2m_afirmar(
		lista_iterador_tiene_siguiente(iterador) == true,
		"se prueba verificar si un iterador que tiene mas elementos para iterar los tiene");
	lista_iterador_avanzar(iterador);
	lista_iterador_avanzar(iterador);
	lista_iterador_avanzar(iterador);
	pa2m_afirmar(
		lista_iterador_tiene_siguiente(iterador) == false,
		"se prueba verificar si un iterador que no tiene mas elementos para iterar los tiene");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}

void lista_iterador_elemento_actual_devuelve_null_o_el_elemento_actual()
{
	lista_iterador_t *iterador = NULL;
	pa2m_afirmar(
		lista_iterador_elemento_actual(iterador) == NULL,
		"se prueba obtener el elemento actual de un iterador nulo");
	int e1 = 1;
	int e2 = 2;
	int e3 = 3;
	lista_t *lista = lista_crear();
	lista = lista_insertar(lista, &e1);
	lista = lista_insertar(lista, &e2);
	lista = lista_insertar(lista, &e3);
	iterador = lista_iterador_crear(lista);
	pa2m_afirmar(
		lista_iterador_elemento_actual(iterador) == &e1,
		"se prueba obtener el elemento actual de un iterador que si tiene elementos para obtener");
	lista_iterador_avanzar(iterador);
	lista_iterador_avanzar(iterador);
	pa2m_afirmar(
		lista_iterador_elemento_actual(iterador) == &e3,
		"se prueba obtener el elemento actual de un iterador que no tiene mas elementos para obtener");
	lista_destruir(lista);
	lista_iterador_destruir(iterador);
}
int main()
{
	/*pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	prueba_simple();*/

	pa2m_nuevo_grupo("Pruebas de creacion lista");
	lista_crear_crea_una_lista_valida();

	pa2m_nuevo_grupo("Pruebas de insercion");
	lista_insertar_aumenta_el_tamanio_de_la_lista();
	lista_insertar_inserta_el_elemento_al_final();
	printf("\n");
	lista_insertar_en_posicion_inserta_el_elemento_en_la_posicion_indicada();

	pa2m_nuevo_grupo("Pruebas eliminacion");
	lista_quitar_elimina_y_devuelve_el_ultimo_elemento();
	printf("\n");
	lista_quitar_en_posicon_elimina_y_devuelve_el_dato_en_la_posicion_pasada_por_parametro();

	pa2m_nuevo_grupo("Pruebas obtencion");
	lista_elemento_en_posicion_devuelve_en_elemento_en_la_posicion_pasada_por_parametro();

	pa2m_nuevo_grupo("Pruebas busqueda");
	lista_buscar_elemento_devuelve_el_primer_elemento_igual_a_contexto_o_null_si_no_existe();

	pa2m_nuevo_grupo("Pruebas iterador externo");
	lista_iterador_crear_duevulve_un_puntero_a_iterador_o_null();
	lista_iterador_avanzar_avanza_o_devuelve_false();
	iterar_sobre_el_elemento_actual_de_la_lista();
	lista_iterador_tiene_siguiente_devuelve_true_si_tiene_o_false_si_no();
	lista_iterador_elemento_actual_devuelve_null_o_el_elemento_actual();
	return pa2m_mostrar_reporte();
}
