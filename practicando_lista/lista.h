#ifndef LISTA_H_
#define LISTA_H_

typedef struct lista lista_t;

lista_t* lista_crear();
lista_t* lista_insertar(lista_t*lista, void*);
void* lista_obtener(lista_t*lista,int posicion);
int lista_tamanio(lista_t* lista);
void lista_destruir(lista_t*);

#endif