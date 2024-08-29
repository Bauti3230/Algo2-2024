#ifndef
#define

typedef struct abb
{
	int dato;
	struct abb *arbol_izquierda;
	struct abb *arbol_derecha;	
}abb_t;

abb_t *crear_abb();

abb_t *abb_inserar();

#endif