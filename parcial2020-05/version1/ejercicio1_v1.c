#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	int campo1;
	void* campo2;
}Azul;

int main()
{
	Azul* v1 = calloc(1,sizeof(Azul));

	if(!v1)
		return 0;

	void** vector = calloc(5,sizeof(void*));

	char** v_verticales[5];

	char* caracter_suelto_en_el_heap = calloc(1,sizeof(char));
	char caracter_suelto_en_el_stack = 0;

	bool hubo_error = false;
	for(int i = 0; i < 5; i++){
		v_verticales[i] = calloc(i+1, sizeof(char*));
		if(!v_verticales[i])
			hubo_error = true;
	}

	if(hubo_error){
		for(int i = 0; i < 5; i++){
			if(v_verticales[i]){
				free(v_verticales[i]);
			}
		}
		free(vector);
		free(v1);
	}

	v1->campo2 = vector;

	for(int i = 0; i < 5; i++){
		vector[i] = v_verticales[i];
		for(int j = 0; j <i+1; j++){
			if(j%2 == 0)
				v_verticales[i][j] = &caracter_suelto_en_el_stack;
			else v_verticales[i][j] = caracter_suelto_en_el_heap;
		}
	}

	free(caracter_suelto_en_el_heap);
	for(int i = 0; i < 5;i++) {
		free(v_verticales[i]);
	}
	free(vector);
	free(v1);


	return 0;
}