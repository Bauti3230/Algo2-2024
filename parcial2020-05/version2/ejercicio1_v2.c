#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct azul
{
	int campo1;
	struct azul** campo2;
}azul;

int main()
{
	azul* v1 = calloc(1,sizeof(azul));

	if(!v1)
		return 0;

	azul** vector = calloc(4, sizeof(azul*));

	if(!vector) {
		free(v1);
		return 0;
	}

	bool hubo_error = false;

	for(int i=0;i<4;i++){
		vector[i] = calloc(1,sizeof(azul));
		if(!vector[i]) {
			hubo_error = true;
		} 
		else {
			vector[i]->campo1 = i+1;
			vector[i]->campo2 = NULL;
		}
	}

	if (hubo_error) {
		for (int i=0;i<4;i++) {
			if (vector[i]) {
				free(vector[i]);
			}
		}
		free(vector);
		free(v1);
	}

	v1->campo2 = vector;

    for (int i = 0; i < 4; i++) {
        free(vector[i]);
    }
    free(vector);
    free(v1);

	return 0;
}