#include "pa2m.h"
#include "diccionario.h"

void insertarUnClaveMePermiteEncontrarla()
{
	char* clave1 = "Hola";
	char* clave2 = "mundo";
	char* clave3 = "algoritmos";

	void *valor1 = (void*)0x1234;
	void *valor2 = (void*)0xBEBECAFE;
	void *valor3 = (void*)0x9999999;	

	diccionario* d = diccionario_crear();

	diccionario_insertar(d, clave1, valor1);
	diccionario_insertar(d, clave2, valor2);
	diccionario_insertar(d, clave3, valor3);

	pa2m_afirmar(diccionario_buscar(d, clave1) == valor1, "Encuentro el valor1");
	pa2m_afirmar(diccionario_buscar(d, clave2) == valor2, "Encuentro el valor2");
	pa2m_afirmar(diccionario_buscar(d, clave3) == valor3, "Encuentro el valor3");

	diccionario_destruir(d);
}

void alInsertarUnElementoConClaveRepetidaSeSobreescribeElValor()
{
	char* clave1 = "Hola";
	char* clave2 = "Hola";

	void *valor1 = (void*)0x1234;
	void *valor2 = (void*)0xBEBECAFE;

	diccionario* d = diccionario_crear();

	diccionario_insertar(d, clave1, valor1);
	diccionario_insertar(d, clave2, valor2);

	//tamanio del diccionario = 1

	pa2m_afirmar(diccionario_buscar(d, clave1) == valor2, "Encuentro el segundi valor inserado con la calve");

	diccionario_destruir(d);

}

int main(int argc, char const *argv[])
{
	pa2m_nuevo_grupo("Pruebas de insercion y busqueda");
	insertarUnClaveMePermiteEncontrarla();

	pa2m_nuevo_grupo("Pruebas de insercion de repetidos");
	alInsertarUnElementoConClaveRepetidaSeSobreescribeElValor();


	return 0;
}