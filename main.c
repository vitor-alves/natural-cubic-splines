/*#######################################
#            Splines Cúbicas            #
#                                       #
#     Vitor Alves Mesquita da Silva     #
#             RA: 158498                #
#                                       #
#######################################*/

#include <stdio.h>
#include <stdlib.h>
#include "spline.h"

void freeMatriz(int n, double **matriz) {
	int i;

	for(i=0; i < n; i++) {
		free(matriz[i]);
	}
	free(matriz);
}

void salvarResultado(int n, double *vetorZ) {
	FILE *file;
	int i;

	file = fopen("splines.txt", "w");

	fprintf(file, "%.8e", 0.0);   // Z0 = 0
	fprintf(file, " %.8e\n", vetorZ[0]);
	for(i = 1; i <= (n-2)-1; i++) {
		fprintf(file, "%.8e %.8e\n", vetorZ[i-1], vetorZ[i]);
	}
	fprintf(file, "%.8e", vetorZ[(n-2)-1]);
	fprintf(file, " %.8e\n", 0.0);   // Zn = 0

	fclose(file);
}

int main() {
	FILE *file;
	int n, i;
	double x, y, **matriz;
	

	file = fopen("tabela.txt", "r");

	/* Lê o valor de n */
	fscanf(file, "%d", &n);

	double vetorZ[n-2];		// vetor do sistema matrizU.vetorZ = vetorY

	/* Aloca espaco para a matriz de pontos */
	matriz = malloc(sizeof(double*) * n);
	for(i = 0; i < n; i++) {
		matriz[i] = malloc(sizeof(double)*2);
	}

	/* Preenche a matriz de pontos (x, y) */
	for(i = 0; i < n; i++) {
		fscanf(file, "%lf %lf", &x, &y);
		matriz[i][0] = x;
		matriz[i][1] = y;
	}

	fclose(file);

	spline(matriz, n, vetorZ);

	freeMatriz(n, matriz);

	salvarResultado(n, vetorZ);
	
	return 0;
}