#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct azul
{
	int campo1;
	struct rojo** campo2;
}azul_t;

typedef struct rojo
{
	int campo1;
	struct rojo* campo2;
}rojo_t;

int main()
{
	azul_t* v1 = calloc(1,sizeof(azul_t));

	if(!v1)
		return 1;

	rojo_t** vector = calloc(4,sizeof(rojo_t*));

	if(!vector){
		free(v1);
		return 1;
	}

	rojo_t* siguiente = NULL;
	bool hubo_error = false;

	for(int i=3;i>=0;i--) {
		vector[i] = calloc(1,sizeof(rojo_t));
		if(!vector[i]) {
			hubo_error = true;
		}
		else{
			vector[i]->campo2 = siguiente;
			siguiente = vector[i];
		}
	}

	if(hubo_error) {
		for(int i=0;i<4;i++){
			if(vector[i]){
				free(vector[i]);
			}
		}
		free(vector);
		free(v1);
	}

	v1->campo2 = vector;

	for(int i=0;i<4;i++){
		free(vector[i]);
	}
	free(vector);
	free(v1);

	return 0;
}