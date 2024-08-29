
int sumar_dos(int *n)
{
	return n+2;
}

int multiplicar_dos(int n)
{
	return n*2;
}

int multiplicar_tres(int n)
{
	return n*3;
}

int realizar_operacion(int n, int (*funcion)(int))
{
	return funcion(n);
}

int main(){

	int n = 0;

	int (*puntero)(int);

	puntero = sumar_dos;

	n = realizar_operacion(n, sumar_dos);
	sumar_dos(&n);

	printf("n = %i\n", n);

	return 0;
}