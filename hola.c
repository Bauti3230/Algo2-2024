#include <stdio.h>

/*
void ejemplo1(){

	char nombre[2];

	nombre[0] = 66; //'B'
	nombre[1] = 'a';
	nombre[2] = 'u';
	nombre[3] = 't';
	nombre[4] = 'i';
	nombre[5] = 0;

	char i = 100;
	char j = 50;

	printf("%s\n", nombre);
}
*/

void ejemplo2()
{
	char nombre[6];//Vector

	nombre[0] = 66; //'B'
	nombre[1] = 'a';
	nombre[2] = 'u';
	nombre[3] = 't';
	nombre[4] = 'i';
	nombre[5] = 0;

	char* nombre2 = nombre;//Puntero, por logica no podria hacer esto pero el compilador toma como puntero a los arrays en el lado derecho
	//Para guardar una direccion de memoria en un procesador de 32bits necesito 4bytes

	printf("%s\n", nombre);
	printf("%s\n", nombre2);
}

void ejemplo3()
{
	char nombre[6];

	nombre[0] = 66; 
	nombre[1] = 'a';
	nombre[2] = 'u';
	nombre[3] = 't';
	nombre[4] = 'i';
	nombre[5] = 0;

	char* nombre2 = nombre;

	nombre2[1] = 'B';//Como este puntero apunta directamente a la direccionde memoria la modifica, por lo que modifica su valor tambien

	printf("%s\n", nombre);
	printf("%s\n", nombre2);
}

/*void ejemplo4()
{
	char nombre[6];

	nombre[0] = 66; //'B'
	nombre[1] = 'a';
	nombre[2] = 'u';
	nombre[3] = 't';
	nombre[4] = 'i';
	nombre[5] = 0;

	char* nombre2 = "Bauti";//En ese caso nombre2 apunta al str "Bauti"(un string literal)
	nombre2[2] = 'C';
}*/

void ejemplo5()
{
	char nombre[6];

	nombre[0] = 66;
	nombre[1] = 'a';
	nombre[2] = 'u';
	nombre[3] = 't';
	nombre[4] = 'i';
	nombre[5] = 0;

	char* nombre2 = nombre;
	char* nombre3 = &nombre2;

	printf("nombre --> %p\n", nombre);
	printf("nombre2 --> %p\n", nombre2);
	printf("nombre3 --> %p\n", nombre3);
}
/*
	char* nombre2 = nombre;
	char** nombre3 = nombre2;
	nombre3[2] = 'C';
*/

int main(){
	//ejemplo1();
	ejemplo2();
	ejemplo3();
	ejemplo5();
	return 0;
}