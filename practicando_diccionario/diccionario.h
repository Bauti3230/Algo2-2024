#ifndef _DICCIONARIO_H_
#define _DICCIONARIO_H_

typedef struct diccionario diccionario;

diccionario* diccionario_crear();

diccionario* diccionario_insertar(diccionario* d, char* clave, void* valor);

void* diccionario_buscar(diccionario* d, char* clave);

void diccionario_destruir(diccionario* d);

#endif // DICCIONARIO_H_