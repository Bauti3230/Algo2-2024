#include "diccionario.h"
#include <stdlib.h>
#include <string.h>

struct par {
	char* k;
	char* v;
};

struct diccionario
{
	int cantidad;
	struct par* elementos;
};

diccionario* diccionario_crear()
{
	return calloc(1,sizeof(diccionario));
}

struct par* buscar_par(diccionario* d, char* clave)
{
	for(int i=0;i<d->cantidad;i++){
		if(strcmp(clave, d->elementos[i].k)==0)
			return d->elementos+i;
	}
	return NULL;
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

void* diccionario_buscar(diccionario* d, char* clave)
{
	struct par* par = buscar_par(d, clave);
	if(par)
		return par->v;
	return NULL;
}

void diccionario_destruir(diccionario* d)
{
	free(d->elementos);
	free(d);
}