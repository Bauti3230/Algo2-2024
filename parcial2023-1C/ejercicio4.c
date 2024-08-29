#include <stdlib.h>

typedef struct verde
{
	int campo1;
	azul* campo2;
}verde_t;

typedef struct azul
{
	void* campo1[2];
}

typedef struct rosa
{
	int campo1;
	char** campo2;
}

int int main(int argc, char const *argv[])
{
	verde_t* v1 = calloc(1,sizeof(verde_t));

	if(!v1)
		return 1;





	return 0;
}