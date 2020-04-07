#include <stdio.h>


int main()
{

#pragma omp parallel
	{
		printf("hola\n");
	}

	return 0;
}