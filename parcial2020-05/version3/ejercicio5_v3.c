/*a. Escriba una función recursiva es_primo, sin usar while / for / do (puede usar una función auxiliar), que indique si un
número es primo o no.*/
#include <stdio.h>
#include <stdbool.h>

bool es_primo_aux(int numero, int divisor)
{
	if(divisor == 1)
		return true;

	if(numero / divisor % 2 == 0)
		return false;

	return es_primo_aux(numero,divisor-1);
}


bool es_primo (int n)
{
	return	es_primo_aux(n,n-1); 
}

//b. Escriba de la misma forma la función primeros_n_primos, que devuelva un vector reservado con malloc / calloc / realloc
//con los primeros n numeros primos.


int main()
{
	bool la_verdad = es_primo(0);

	if(la_verdad){
		printf("Siiii es primo");
	}
	else {
		printf("Noo es primo :(");
	}


}
