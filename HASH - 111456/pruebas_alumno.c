#include "pa2m.h"
#include "src/hash.h"

#define CAPACIDAD_INFERIOR_A_MINIMA 1
#define CAPACIDAD_MINIMA 3
#define CAPACIDAD_SUPERIOR_A_MINIMA 5

void hash_crear_me_devuelve_un_hash_valido()
{
	hash_t *hash_inferior = hash_crear(CAPACIDAD_INFERIOR_A_MINIMA);
	hash_t *hash_minimo = hash_crear(CAPACIDAD_INFERIOR_A_MINIMA);
	hash_t *hash_superior = hash_crear(CAPACIDAD_INFERIOR_A_MINIMA);

	pa2m_afirmar(
		hash_inferior != NULL,
		"Se preuba crear un hash con una capacidad menor a la capacidad minima");
	pa2m_afirmar(
		hash_minimo != NULL,
		"Se preuba crear un hash con una capacidad igual a la capacidad minima");
	pa2m_afirmar(
		hash_superior != NULL,
		"Se preuba crear un hash con una capacidad superior a la capacidad minima");

	hash_destruir(hash_inferior);
	hash_destruir(hash_minimo);
	hash_destruir(hash_superior);
}

void hash_insertar_inserta_un_nuevo_par_en_la_tabla()
{
	void *e1 = (void *)12;
	void *e2 = (void *)156;
	void *e3 = (void *)76;
	void *e4 = (void *)NULL;
	void *e5 = (void *)"holaa";

	char *clav1 = "Hola";
	char *clav2 = "mundo";
	char *clav3 = "como";
	char *clav4 = "estas";
	char *clav5 = "holaaa";
	char *clave_nula = NULL;

	hash_t *hash = NULL;

	pa2m_afirmar(
		!hash_insertar(hash, clave_nula, e3, NULL),
		"Se preuba insertar un elemento en una tabla nula con una clave nula");
	pa2m_afirmar(
		!hash_insertar(hash, clav1, e1, NULL),
		"Se preuba insertar un elemento en una tabla nula con una clave no nula");

	hash = hash_crear(1);
	pa2m_afirmar(
		!hash_insertar(hash, clave_nula, e3, NULL),
		"Se preuba insertar un elemento en una tabla con una clave nula");

	pa2m_afirmar(hash_insertar(hash, clav1, e1, NULL) != NULL,
		     "Se preuba insertar un elemento en una tabla de hash");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Luego de insetar hay 1 elemento en la tabla");

	pa2m_afirmar(hash_insertar(hash, clav2, e2, NULL) != NULL,
		     "Se preuba insertar un elemento en una tabla de hash");
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Luego de insetar hay 2 elemento en la tabla");

	pa2m_afirmar(hash_insertar(hash, clav3, e3, NULL) != NULL,
		     "Se prueba insertar un elemento en una tabla de hash");
	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Luego de insetar hay 3 elemento en la tabla");

	pa2m_afirmar(hash_insertar(hash, clav4, e4, NULL) != NULL,
		     "Se preuba insertar un elemento en una tabla de hash");
	pa2m_afirmar(hash_cantidad(hash) == 4,
		     "Luego de insetar hay 4 elemento en la tabla");

	pa2m_afirmar(hash_insertar(hash, clav5, e5, NULL) != NULL,
		     "Se preuba insertar un elemento en una tabla de hash");
	pa2m_afirmar(hash_cantidad(hash) == 5,
		     "Luego de insetar hay 5 elemento en la tabla");

	hash_destruir(hash);
}

void las_ultimas_pruebas_de_insercion()
{
	void *e1 = (void *)12;
	void *e2 = (void *)156;
	void *e3 = (void *)76;
	void *e4 = (void *)NULL;
	void *e5 = (void *)"holaa";
	void *valor_sustituto = "soy un char*";

	void *anterior1 = (void *)35;
	void *anterior2 = (void *)"Estoy a dieta";

	char *clav1 = "Hola";
	char *clav2 = "mundo";
	char *clav3 = "como";
	char *clav4 = "estas";
	char *clav5 = "holaaa";

	hash_t *hash = hash_crear(1);

	hash_insertar(hash, clav1, e1, &anterior1);
	hash_insertar(hash, clav2, e2, NULL);
	hash_insertar(hash, clav3, e3, NULL);
	hash_insertar(hash, clav4, e4, NULL);
	hash_insertar(hash, clav5, e5, NULL);

	hash_insertar(hash, clav3, valor_sustituto, &anterior2);
	pa2m_afirmar(
		hash_contiene(hash, clav3) /*&&
			hash_obtener(hash, clav3) == valor_sustituto*/
		,
		"Se prueba insertar un elemento con una clave ya registrada");
	pa2m_afirmar(hash_cantidad(hash) == 5, "La cantidad no aumenta");

	pa2m_afirmar(
		anterior1 == NULL,
		"El anterior se modifica correctamente cuando insertamos un nuvo elemento");
	pa2m_afirmar(
		anterior2 == e3,
		"El anterior se modifica correctamente cuando insertamos un elemento con una clave ya existente");
	clav5 = "modificada";
	pa2m_afirmar(
		!hash_obtener(hash, clav5) && !hash_contiene(hash, clav5),
		"Como se guarda una copia de la clave, si esta se modifica, no se puede encontrar");

	hash_destruir(hash);
}

void hash_quitar_elimina_y_devuelve_el_elemento_quitado()
{
	void *e1 = (void *)12;
	void *e2 = (void *)156;
	void *e3 = (void *)76;
	void *e4 = (void *)78;

	char *clav1 = "Hola";
	char *clav2 = "mundo";
	char *clav3 = "como";
	char *clav4 = "estas";
	char *clave_no_asociada = "am";
	char *clave_nula = NULL;

	hash_t *hash = NULL;

	pa2m_afirmar(!hash_quitar(hash, clave_nula),
		     "Se prueba quitar con una clave nula de un hash nulo");
	pa2m_afirmar(!hash_quitar(hash, clav1),
		     "Se prueba quitar con una clave valida de un hash nulo");

	hash = hash_crear(CAPACIDAD_SUPERIOR_A_MINIMA);

	hash_insertar(hash, clav1, e1, NULL);
	hash_insertar(hash, clav2, e2, NULL);
	hash_insertar(hash, clav3, e3, NULL);
	hash_insertar(hash, clav4, e4, NULL);

	pa2m_afirmar(
		hash_quitar(hash, clav1) == e1,
		"Se preuba quitar con una clave valida asociada a un elemento en un hash valido");
	pa2m_afirmar(hash_cantidad(hash) == 3,
		     "Luego de eliminar hay 3 elementos");
	pa2m_afirmar(
		!hash_obtener(hash, clav1) && !hash_contiene(hash, clav1),
		"Se prueba obtener un elemento que fue quitado de la tabla");
	pa2m_afirmar(
		hash_quitar(hash, clav2) == e2 && !hash_contiene(hash, clav2),
		"Se preuba quitar con una clave valida asociada a un elemento en un hash valido");
	pa2m_afirmar(hash_cantidad(hash) == 2,
		     "Luego de eliminar hay 2 elemento");
	pa2m_afirmar(
		!hash_obtener(hash, clav2),
		"Se prueba obtener un elemento que fue quitado de la tabla");
	pa2m_afirmar(
		hash_obtener(hash, clave_no_asociada) == NULL,
		"Se prueba quitar con una clave valida no asociada a un elemento en un hash valido");

	hash_quitar(hash, clav3);
	hash_quitar(hash, clav4);

	pa2m_afirmar(
		hash_cantidad(hash) == 0,
		"Se prueba eliminar los elementos restantes y la tabla queda vacia");

	hash_destruir(hash);
}

void hash_obtener_devuelve_el_elementos_en_caso_de_existir()
{
	void *e1 = (void *)12;
	void *e4 = (void *)1;

	char *clav1 = "Hola";
	char *clav4 = "estas";
	char *clave_no_asociada = "No estoy asociada";
	char *clave_nula = NULL;

	hash_t *hash = NULL;

	pa2m_afirmar(
		!hash_obtener(hash, clave_nula),
		"Se prueba obtener un elemento con una clave nula de hash nulo");
	pa2m_afirmar(
		!hash_obtener(hash, clav1),
		"Se prueba obtener un elemento que una clave valida de un hash nulo");

	hash = hash_crear(CAPACIDAD_SUPERIOR_A_MINIMA);

	hash_insertar(hash, clav1, e1, NULL);
	hash_insertar(hash, clav4, e4, NULL);

	pa2m_afirmar(hash_obtener(hash, clav1) == e1,
		     "Se prueba obtener un elemento asociado a un clave");
	pa2m_afirmar(hash_obtener(hash, clav4) == e4,
		     "Se prueba obtener un elemento asociado a un clave");
	pa2m_afirmar(!hash_obtener(hash, clave_no_asociada),
		     "Se prueba obtener un elemento no asociado");

	hash_destruir(hash);
}

void hash_contiene_devulve_true_si_existe_un_elemetno_asociado_a_la_clave()
{
	void *e1 = (void *)12;
	void *e4 = (void *)1;

	char *clav1 = "Hola";
	char *clav4 = "estas";
	char *clave_nula = NULL;

	hash_t *hash = NULL;

	pa2m_afirmar(
		!hash_contiene(hash, clave_nula),
		"Se prueba verificar si una clave nula tiene un elemento asociado en una tabla nula");
	pa2m_afirmar(
		!hash_contiene(hash, clav1),
		"Se prueba verificar si una clave no nula tiene un elemento asociado en una tabla nula");

	hash = hash_crear(CAPACIDAD_SUPERIOR_A_MINIMA);

	hash_insertar(hash, clav1, e1, NULL);
	hash_insertar(hash, clav4, e4, NULL);

	pa2m_afirmar(
		hash_contiene(hash, clav1),
		"Se prueba verificar si una clave tiene un elemento asociado en una tabla");
	pa2m_afirmar(
		hash_contiene(hash, clav4),
		"Se prueba verificar si una clave tiene un elemento asociado en una tabla");

	hash_destruir(hash);
}

void despues_de_eliminar_se_puede_obtener_todos_los_elementos_remanentes_en_la_tabla()
{
	char *clav1 = "Pikachu";
	char *clav2 = "Charmander";
	char *clav3 = "Squirtle";
	char *clav4 = "Bulbasaur";
	char *clav5 = "Jigglypuff";
	char *clav6 = "Eevee";
	char *clav7 = "Snorlax";
	char *clav8 = "Mewtwo";
	char *clav9 = "Gyarados";
	char *clav10 = "Gengar";

	void *e1 = (void *)42;
	double e2 = 3.14;
	void *e3 = (void *)'A';
	float e4 = 2;
	long e5 = 1234567890;
	short e6 = 32767;
	unsigned int e7 = 1000;
	unsigned long e8 = 9876543210;
	unsigned char e9 = 'B';
	int e10 = 1;

	hash_t *hash = hash_crear(3);

	hash_insertar(hash, clav1, e1, NULL);
	hash_insertar(hash, clav2, &e2, NULL);
	hash_insertar(hash, clav3, e3, NULL);
	hash_insertar(hash, clav4, &e4, NULL);
	hash_insertar(hash, clav5, &e5, NULL);
	hash_insertar(hash, clav6, &e6, NULL);
	hash_insertar(hash, clav7, &e7, NULL);
	hash_insertar(hash, clav8, &e8, NULL);
	hash_insertar(hash, clav9, &e9, NULL);
	hash_insertar(hash, clav10, &e10, NULL);

	hash_quitar(hash, clav3);
	hash_quitar(hash, clav6);
	hash_quitar(hash, clav9);
	hash_quitar(hash, clav8);
	hash_quitar(hash, clav1);

	pa2m_afirmar(hash_obtener(hash, clav2) == &e2 &&
			     hash_contiene(hash, clav2),
		     "Se prueba encontrar y obtener un elemento restante");
	pa2m_afirmar(hash_obtener(hash, clav4) == &e4 &&
			     hash_contiene(hash, clav4),
		     "Se prueba encontrar y obtener un elemento restante");
	pa2m_afirmar(hash_obtener(hash, clav5) == &e5 &&
			     hash_contiene(hash, clav5),
		     "Se prueba encontrar y obtener un elemento restante");
	pa2m_afirmar(hash_obtener(hash, clav7) == &e7 &&
			     hash_contiene(hash, clav7),
		     "Se prueba encontrar y obtener un elemento restante");
	pa2m_afirmar(hash_obtener(hash, clav10) == &e10 &&
			     hash_contiene(hash, clav10),
		     "Se prueba encontrar y obtener un elemento restante");

	hash_destruir(hash);
}

void proxima_clave(char *c)
{
	c[0]++;
	if (c[0] == 'z' + 1) {
		c[0] = '0';
		proxima_clave(c + 1);
	}
}

void inserto_y_elimino_muchos_elementos()
{
	char clave[4] = { '1', '1', '1', 0 };
	int cant_errores = 0;
	int cantidad = 50000;

	hash_t *hash = hash_crear(1);

	for (int i = 0; i < cantidad; i++) {
		void *anterior = NULL;
		int posicion = i;
		int *pt = &posicion;

		if (hash_insertar(hash, clave, (void *)pt, &anterior) == NULL)
			cant_errores++;

		if (anterior != NULL)
			pa2m_afirmar(
				0,
				"Se reemplazo una clave que no se debia reemplazar");

		proxima_clave(clave);
	}

	pa2m_afirmar(cant_errores == 0, "No hubieron errores ");
	if (cant_errores != 0)
		printf("cantidad de errores al insertar : %i\n", cant_errores);
	pa2m_afirmar(hash_cantidad(hash) == cantidad, "Tiene 50000 elementos");

	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	pa2m_nuevo_grupo("Pruebas de creacion Hash");
	hash_crear_me_devuelve_un_hash_valido();

	pa2m_nuevo_grupo("Pruebas de Insercion Hash");
	hash_insertar_inserta_un_nuevo_par_en_la_tabla();

	pa2m_nuevo_grupo("Mas Pruebas de Insercion");
	las_ultimas_pruebas_de_insercion();

	pa2m_nuevo_grupo("Pruebas de Eliminacion Hash");
	hash_quitar_elimina_y_devuelve_el_elemento_quitado();

	pa2m_nuevo_grupo("Pruebas de Contension Hash");
	hash_contiene_devulve_true_si_existe_un_elemetno_asociado_a_la_clave();

	pa2m_nuevo_grupo("Pruebas de Obtencion Hash");
	hash_obtener_devuelve_el_elementos_en_caso_de_existir();

	pa2m_nuevo_grupo("Pruebas de Obtencion Despues de Eliminacion Hash");
	despues_de_eliminar_se_puede_obtener_todos_los_elementos_remanentes_en_la_tabla();

	pa2m_nuevo_grupo("Hash con Muchos Elementos");
	inserto_y_elimino_muchos_elementos();

	return pa2m_mostrar_reporte();
}