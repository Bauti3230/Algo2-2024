#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GRAN_PREGUNTA 100

int main(){
	int tamanio = 100;
	char* texto = malloc(tamanio*sizeof(char));
	int tope = 0;

	while(1){
		scanf("%s",texto+tope);
		tope = strlen(texto);

		if(tope >= tamanio/2) {
			tamanio *= 2;
			char* texto_aux = malloc(tamanio*sizeof(char));
			strcpy(texto_aux,texto);
			texto = texto_aux;
		}

		printf("Hasta ahora ingresaste esto:\n\n%s\n-------------------------------------------------------------\n\n",texto);
	}
	return 0;
}