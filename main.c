#include <stdio.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void printdeb(unsigned char i);
void sudoku();
void calcularPosibles(int **ejercicio, int ***numPosibles, int** cantNumPOsibles, int fila, int columna);
void validarCombinacion(int cominacion[16][16]);
int main()
{
	printf("n thread\tt sec\n");
	for (unsigned char i = 0; i <= 16; i++)
	{
		
		omp_set_num_threads(i);
		clock_t start = clock();
		sudoku();
		double tiempoExe = (((double)clock() - start) / CLOCKS_PER_SEC);
		printf("%d\t%f\n",i,tiempoExe);
		
		//printf("%d\n", i % 4);

	}
	

	return 0;
}

void sudoku()
{
	const int ejercicio[16][16] = {
									{0,12,9,0,7,8,0,14,0,0,0,5,4,0,0,0},
									{7,0,0,0,3,0,0,0,15,0,0,14,6,0,0,0},
									{0,0,0,2,0,9,0,0,0,0,10,0,0,0,0,0},
									{0,6,0,0,0,0,5,0,0,0,0,12,0,3,11,0},
									{0,0,0,0,0,2,4,0,0,13,0,0,0,0,0,11},
									{3,0,11,0,0,0,8,0,0,0,1,0,0,0,16,9},
									{0,0,0,0,0,11,6,0,0,15,0,0,10,0,3,0},
									{0,2,6,0,9,12,0,0,7,0,0,0,5,0,0,8},
									{0,0,13,9,8,3,0,0,2,0,0,0,0,0,0,0},
									{0,0,0,0,0,0,0,0,0,4,7,0,13,10,15,0},
									{0,0,0,14,0,16,0,0,6,0,0,0,9,0,12,7},
									{0,8,3,6,12,0,9,13,0,5,0,10,0,0,0,2},
									{12,0,0,0,0,14,0,6,0,3,0,0,11,15,0,0},
									{0,0,0,0,2,15,0,0,11,0,6,4,3,0,0,0},
									{0,0,0,0,0,10,0,0,0,0,5,16,0,14,4,12},
									{0,0,0,0,0,4,11,12,14,0,0,8,7,0,5,0}
								   };
	int*** numPosibles;
	numPosibles = (int***)calloc(16, sizeof(int**));
	int** cantNumPOsibles;
	cantNumPOsibles = (int**)calloc(16, sizeof(int*));
	//shared(numPosibles,cantNumPOsibles)
	#pragma omp parallel 
	{
		#pragma omp for
		for (int i = 0; i < 16; i++)
		{
			numPosibles[i] = (int**)calloc(16, sizeof(int*));

			#pragma omp for
			for (int j = 0; j < 16; j++)
			{
				numPosibles[i][j] = (int*)calloc(16, sizeof(int));

				#pragma omp for	
				for (int k = 0; k < 16; k++)
				{
					numPosibles[i][j][k] = 0;
				}
			}
		}

		#pragma omp for
		for (int i = 0; i < 16; i++) {
			cantNumPOsibles[i] = (int*)calloc(16, sizeof(int));
			#pragma omp for
			for (int j = 0; j < 16; j++) {
				cantNumPOsibles[i][j] = 0;
			}
		}
	}

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < 16; i++)
		{
			#pragma omp for
			for (int j = 0; j < 16; j++)
			{
				calcularPosibles(ejercicio, numPosibles, cantNumPOsibles, i, j);
			}
		}
	}

	long cantCombinaciones = 1;

	#pragma omp parallel
	{	
		#pragma omp for reduction(*:cantCombinaciones)
		for (int i = 0; i < 16; i++)
		{
			#pragma omp for reduction(*:cantCombinaciones)
			for (int j = 0; j < 16; j++)
			{
				cantCombinaciones *= cantNumPOsibles[i][j];
			}
		}
	}

	const int** cnp = cantNumPOsibles;
	const int*** np = numPosibles;
	#pragma omp parallel private(combinacion)
	{
		#pragma omp for
		for (long i = 0; i < cantCombinaciones; i++)
		{
			int combinacion[16][16] = {
										{np[0][0][i%cnp[0][0]] ,12,9,0,7,8,0,14,0,0,0,5,4,0,0,0},
										{np[0][0][i%cnp[0][0]] ,0,0,0,3,0,0,0,15,0,0,14,6,0,0,0},
										{np[0][0][i%cnp[0][0]] ,0,0,2,0,9,0,0,0,0,10,0,0,0,0,0},
										{np[0][0][i%cnp[0][0]],6,0,0,0,0,5,0,0,0,0,12,0,3,11,0},
										{np[0][0][i%cnp[0][0]],0,0,0,0,2,4,0,0,13,0,0,0,0,0,11},
										{np[0][0][i%cnp[0][0]],0,11,0,0,0,8,0,0,0,1,0,0,0,16,9},
										{np[0][0][i%cnp[0][0]],0,0,0,0,11,6,0,0,15,0,0,10,0,3,0},
										{np[0][0][i%cnp[0][0]],2,6,0,9,12,0,0,7,0,0,0,5,0,0,8},
										{np[0][0][i%cnp[0][0]],0,13,9,8,3,0,0,2,0,0,0,0,0,0,0},
										{np[0][0][i%cnp[0][0]],0,0,0,0,0,0,0,0,4,7,0,13,10,15,0},
										{np[0][0][i%cnp[0][0]],0,0,14,0,16,0,0,6,0,0,0,9,0,12,7},
										{np[0][0][i%cnp[0][0]],8,3,6,12,0,9,13,0,5,0,10,0,0,0,2},
										{np[0][0][i%cnp[0][0],0,0,0,0,14,0,6,0,3,0,0,11,15,0,0},
										{np[0][0][i%cnp[0][0],0,0,0,2,15,0,0,11,0,6,4,3,0,0,0},
										{np[0][0][i%cnp[0][0],0,0,0,0,10,0,0,0,0,5,16,0,14,4,12},
										{np[0][0][i%cnp[0][0],0,0,0,0,4,11,12,14,0,0,8,7,0,5,0}
			};
		}

		
	}
	

}

void calcularPosibles(int** ejercicio, int*** numPosibles, int** cantNumPOsibles, int fila, int columna)
{
}

void validarCombinacion(int cominacion[16][16])
{
}

void printdeb(unsigned char i)
{
#pragma omp parallel
	{
		printf("hola %d\n", i);
	}
	return;
}