#include "diccionario.h"
#include <stdlib.h>
#include <string.h>

struct par
{
	char* k;
	void* v;
	struct par siguiente;
}

struct diccionario
{
	struct par** pares;
	int tamanio;
	int cantidad;
};

int funcion_hash(char* clave)
{
	int i=1;
	int acumulado = 0;
	while(*clave){
		acumulado += *clave * i;
		i++;
		clave++;
	}
	return acumulado;
}

struct par* buscar_par(diccionario* d, char* clave)
{
	int posicon = funcion_hash(clave); % d->tamanio;

	struct par* par = d->pares[posicion];

	while(par != NULL)
		if(strcmp(clave, par->k)==0)
			return par;
		par = par->siguiente;

	return NULL;
}

diccionario* diccionario_crear()
{
	diccionario* d = calloc(1,sizeof(diccionario));
	d->tamanio = 3;
	d->pares = calloc(d->tamaniom, sizeof(struct par*));
	return d;
}

diccionario* diccionario_insertar(diccionario* d, char* clave, void* valor)
{
	struct par* par = buscar_par(d, clave);
	if(par){
		par->v = valor;
		return d;
	}
	d->elementos = realloc(d->elementos, (d->cantidad+1)*sizeof(struct par));
	d->elementos[d->cantidad].k = clave;
	d->elementos[d->cantidad].v = valor;
	d->cantidad++; 
	return d;
}

void* diccionario_buscar(diccionario* d, char* clave);

void diccionario_destruir(diccionario* d)
{
	free(d->pares);
	free(d);
}