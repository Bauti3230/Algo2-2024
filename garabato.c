int main(){


	void* a;
	char* b = a;
	char** c = a;

	b[0] = 'l';

	c[0][10] = 'm';

	a[1] = 2; // esto no lo pdriamos hacer ya que no sabemos de que tipo es el a) porque el void es la falta de tipo.

	return 0;
}