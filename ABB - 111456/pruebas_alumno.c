#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"

#define COMPARADOR_NULO NULL

int comparador(void *a, void *b)
{
	if (a == NULL && b == NULL)
		return 0; // Ambos son NULL, se consideran iguales
	if (a == NULL)
		return -1; // a es NULL, se considera menor que b
	if (b == NULL)
		return 1; // b es NULL, se considera mayor que a

	int *a_p = (int *)a;
	int *b_p = (int *)b;

	return *a_p - *b_p;
}

void imprimirABB(nodo_abb_t *nodo)
{
	if (nodo != NULL) {
		imprimirABB(nodo->izquierda);
		printf("%d ", *(int *)nodo->elemento);
		imprimirABB(nodo->derecha);
	}
}

void abb_crear_crea_un_arbol_nuevo_valido()
{
	abb_t *arbol = abb_crear(COMPARADOR_NULO);

	pa2m_afirmar(
		!arbol,
		"se prueba crear un arbol con un comparador nulo y el arbol el nulo");
	pa2m_afirmar(abb_vacio(arbol),
		     "un abrol crear con un comparador nulo esta vacio");
	pa2m_afirmar(abb_tamanio(arbol) == 0,
		     "un arbol creador con un comparador nulo tiene tamanio 0");

	arbol = abb_crear(comparador);

	pa2m_afirmar(arbol != NULL,
		     "se prueba crear un arbol con un comparador no nulo");
	pa2m_afirmar(
		abb_vacio(arbol),
		"un abrol nuevo, creado con un comparador no nulo, esta vacio");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "un arbol nuevo tiene tamanio 0");
	abb_destruir(arbol);
}

void abb_insertar_inserta_un_nuevo_nodo_en_el_arbol()
{
	abb_t *arbol = abb_crear(COMPARADOR_NULO);
	int e1 = 1;
	void *e2 = NULL;
	int e3 = 2;
	int e4 = 0;

	pa2m_afirmar(abb_insertar(arbol, &e1) == NULL,
		     "se prueba insertar un elemento en un arbol nulo");
	pa2m_afirmar(abb_insertar(arbol, e2) == NULL,
		     "se prueba insertar un elemento nulo en un arbol nulo");

	arbol = abb_crear(comparador);
	size_t tamanio_inicial = abb_tamanio(arbol);
	arbol = abb_insertar(arbol, &e1);

	pa2m_afirmar(
		abb_tamanio(arbol) > tamanio_inicial,
		"se prueba insertar un elemento no nulo en un arbol, aumenta el tamanio del árbol");
	pa2m_afirmar(abb_tamanio(arbol) == 1, "el tamanio actual es 1");

	arbol = abb_insertar(arbol, &e3);
	pa2m_afirmar(
		abb_tamanio(arbol) > tamanio_inicial,
		"se prueba insertar un elemento no nulo en un arbol, aumenta el tamanio del árbol");
	pa2m_afirmar(abb_tamanio(arbol) == 2, "el tamanio actual es 2");

	arbol = abb_insertar(arbol, &e4);
	pa2m_afirmar(
		abb_tamanio(arbol) > tamanio_inicial,
		"se prueba insertar un elemento no nulo en un arbol, aumenta el tamanio del árbol");
	pa2m_afirmar(abb_tamanio(arbol) == 3, "el tamanio actual es 3");

	abb_destruir(arbol);
}

void abb_insertar_pruebas_con_null()
{
	abb_t *arbol = abb_crear(comparador);
	void *e1 = NULL;
	void *e2 = NULL;

	arbol = abb_insertar(arbol, e1);
	arbol = abb_insertar(arbol, e2);

	pa2m_afirmar(arbol->nodo_raiz->elemento == e1,
		     "se pruede insertar un elemento nulo");
	pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == e2,
		     "se pruede insertar un elemento nulo");

	abb_destruir(arbol);
}

void abb_quitar_elimina_el_nodo_hoja_y_devulve_el_elemento_del_nodo_quitado_o_null_en_caso_de_error()
{
	abb_t *arbol = abb_crear(COMPARADOR_NULO);
	int e1 = 1;
	int e2 = 2;
	int e3 = 0;
	int e4 = 5;
	int e6 = 7;
	int e7 = 6;
	int e8 = 8;
	int e5 = 10;

	pa2m_afirmar(
		abb_quitar(arbol, &e2) == NULL,
		"se prueba quitar un elemento de un arbol nulo, devuleve null");
	arbol = abb_crear(comparador);
	pa2m_afirmar(
		abb_quitar(arbol, &e4) == NULL,
		"se prueba quitar un elemento de un arbol vacio, devuleve null");

	arbol = abb_insertar(arbol, &e4);
	arbol = abb_insertar(arbol, &e1);
	arbol = abb_insertar(arbol, &e2);
	arbol = abb_insertar(arbol, &e3);
	arbol = abb_insertar(arbol, &e6);
	arbol = abb_insertar(arbol, &e7);
	arbol = abb_insertar(arbol, &e8);

	pa2m_afirmar(
		abb_quitar(arbol, &e5) == NULL,
		"se prueba quitar un elemento que no existe en la lista, devuleve null");
	pa2m_afirmar(
		abb_quitar(arbol, &e3) == &e3,
		"se prueba quitar un nodo hoja y devuleve el elemento eliminado (devuleve 0)");
	pa2m_afirmar(
		abb_tamanio(arbol) == 6,
		"al eliminar un elemento del árbol reduce el tamanio del árbol");

	abb_destruir(arbol);
}

void abb_quitar_elimina_el_nodo_padre_y_devuelve_el_elemento_eliminado_o_null()
{
	abb_t *arbol = abb_crear(COMPARADOR_NULO);
	int e1 = 8;
	int e2 = 6;
	int e3 = 3;
	int e4 = 5;
	int e5 = 12;
	int e6 = 10;
	int e7 = 11;
	int e8 = 17;
	int e9 = 14;
	int e10 = 18;
	int e11 = 100;

	pa2m_afirmar(
		abb_quitar(arbol, &e5) == NULL,
		"se prueba quitar un elemento de un arbol nulo, devuleve null");
	arbol = abb_crear(comparador);
	pa2m_afirmar(
		abb_quitar(arbol, &e4) == NULL,
		"se prueba quitar un elemento de un arbol vacio, devuleve null");

	arbol = abb_insertar(arbol, &e1);
	arbol = abb_insertar(arbol, &e2);
	arbol = abb_insertar(arbol, &e3);
	arbol = abb_insertar(arbol, &e4);
	arbol = abb_insertar(arbol, &e5);
	arbol = abb_insertar(arbol, &e6);
	arbol = abb_insertar(arbol, &e7);
	arbol = abb_insertar(arbol, &e8);
	arbol = abb_insertar(arbol, &e9);
	arbol = abb_insertar(arbol, &e10);

	pa2m_afirmar(
		abb_quitar(arbol, &e11) == NULL,
		"se prueba quitar un elemento que no existe en la lista, devuleve null");

	pa2m_afirmar(abb_quitar(arbol, &e10) == &e10,
		     "se prueba quitar un nodo hoja");
	pa2m_afirmar(abb_buscar(arbol, &e10) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 9,
		     "ahora el arbol tiene 8 elementos");

	pa2m_afirmar(abb_quitar(arbol, &e9) == &e9,
		     "se prueba quitar un nodo hoja");
	pa2m_afirmar(abb_buscar(arbol, &e9) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 8,
		     "ahora el arbol tiene 7 elementos");

	pa2m_afirmar(abb_quitar(arbol, &e3) == &e3,
		     "se prueba quitar un nodo padre, con un solo hijo");
	pa2m_afirmar(abb_buscar(arbol, &e3) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 7,
		     "ahora el arbol tiene 6 elementos");

	pa2m_afirmar(abb_quitar(arbol, &e2) == &e2,
		     "se prueba quitar un nodo padre, con un solo hijo");
	pa2m_afirmar(abb_buscar(arbol, &e2) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 6,
		     "ahora el arbol tiene 5 elementos");

	pa2m_afirmar(abb_quitar(arbol, &e5) == &e5,
		     "se prueba quitar un nodo padre, con dos hijo");
	pa2m_afirmar(abb_buscar(arbol, &e5) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 5,
		     "ahora el arbol tiene 5 elementos");
	pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &e7,
		     "el nodo de la derecha es el esperado");

	pa2m_afirmar(abb_quitar(arbol, &e1) == &e1,
		     "se prueba quitar la raiz, con dos hijo");
	pa2m_afirmar(abb_buscar(arbol, &e1) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 4,
		     "ahora el arbol tiene 4 elementos");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &e4,
		     "el nodo raiz, es el esperado");

	pa2m_afirmar(abb_quitar(arbol, &e4) == &e4,
		     "se prueba quitar un nodo raiz, con un solo hijo");
	pa2m_afirmar(abb_buscar(arbol, &e4) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 3,
		     "ahora el arbol tiene 3 elementos");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &e7,
		     "el nodo raiz, es el esperado");

	pa2m_afirmar(abb_quitar(arbol, &e7) == &e7,
		     "se prueba quitar un nodo raiz, con dos hijos");
	pa2m_afirmar(abb_buscar(arbol, &e7) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 2,
		     "ahora el arbol tiene 2 elementos");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &e6,
		     "el nodo raiz, es el esperado");

	pa2m_afirmar(abb_quitar(arbol, &e6) == &e6,
		     "se prueba quitar un nodo raiz, con un solo hijo");
	pa2m_afirmar(abb_buscar(arbol, &e6) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_tamanio(arbol) == 1,
		     "ahora el arbol tiene 1 elementos");
	pa2m_afirmar(arbol->nodo_raiz->elemento == &e8,
		     "el nodo raiz, es el esperado");

	pa2m_afirmar(abb_quitar(arbol, &e8) == &e8,
		     "se prueba quitar un nodo raiz, sin hijos");
	pa2m_afirmar(abb_buscar(arbol, &e8) == NULL,
		     "el elemento quitar ya no existe");
	pa2m_afirmar(abb_vacio(arbol), "ahora el arbol tiene 0 elementos");
	pa2m_afirmar(arbol->nodo_raiz == NULL, "el nodo raiz, es el esperado");

	imprimirABB(arbol->nodo_raiz);

	abb_destruir(arbol);
}

void abb_buscar_devuelve_el_elemento_o_null_en_caso_de_no_encontrarlo()
{
	int e1 = 1;
	int e2 = 2;
	int e3 = 0;
	int e4 = 5;
	int e5 = 10;
	abb_t *arbol = abb_crear(COMPARADOR_NULO);

	pa2m_afirmar(abb_buscar(arbol, &e1) == NULL,
		     "se prueba buscar en un arbol nulo, devulve null");
	arbol = abb_crear(comparador);
	pa2m_afirmar(abb_buscar(arbol, &e3) == NULL,
		     "se prueba buscar en un arbol vacio, devulve null");

	arbol = abb_insertar(arbol, &e1);
	arbol = abb_insertar(arbol, &e2);
	arbol = abb_insertar(arbol, &e3);
	arbol = abb_insertar(arbol, &e4);

	pa2m_afirmar(
		abb_buscar(arbol, &e1) == &e1,
		"se prueba buscar un elemento que esta en el arbol y que se devuelva correctamente (es 1)");
	pa2m_afirmar(
		abb_buscar(arbol, &e2) == &e2,
		"se prueba buscar un elemento que esta en el arbol y que se devuelva correctamente (es 2)");
	pa2m_afirmar(
		abb_buscar(arbol, &e3) == &e3,
		"se prueba buscar un elemento que esta en el arbol y que se devuelva correctamente (es 0)");
	pa2m_afirmar(
		abb_buscar(arbol, &e4) == &e4,
		"se prueba buscar un elemento que esta en el arbol y que se devuelva correctamente (es 5)");
	pa2m_afirmar(
		abb_buscar(arbol, &e5) == NULL,
		"se prueba buscar un elemento que no esta en el arbol y que se devuelva null");

	abb_destruir(arbol);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	pa2m_nuevo_grupo("Pruebas de creacion arbol");
	abb_crear_crea_un_arbol_nuevo_valido();

	pa2m_nuevo_grupo("Pruebas de incersion");
	abb_insertar_inserta_un_nuevo_nodo_en_el_arbol();
	abb_insertar_pruebas_con_null();

	pa2m_nuevo_grupo("Pruebas de eliminacion");
	abb_quitar_elimina_el_nodo_hoja_y_devulve_el_elemento_del_nodo_quitado_o_null_en_caso_de_error();
	abb_quitar_elimina_el_nodo_padre_y_devuelve_el_elemento_eliminado_o_null();

	pa2m_nuevo_grupo("Pruebas de busqueda");
	abb_buscar_devuelve_el_elemento_o_null_en_caso_de_no_encontrarlo();

	return pa2m_mostrar_reporte();
}
