#include "spline.h"

void preencherVetorH(double **matriz, int n, double *vetorH) {
	int i;

	for(i = 0; i < n-1; i++) {
		vetorH[i] = matriz[i+1][0] - matriz[i][0];
	}
}

void preencherVetorB(double **matriz, int n, double *vetorH, double *vetorB) {
	int i;

	for(i = 0; i < n-1; i++) {
		vetorB[i] = (matriz[i+1][1] - matriz[i][1]) / vetorH[i];
	}
}

void preencherVetorV(double **matriz, int n, double *vetorH, double *vetorV) {
	int i;

	for(i = 1; i < n-1; i++) {
		vetorV[i-1] = 2 * (vetorH[i-1] + vetorH[i]);
	}
}

void preencherVetorU(double **matriz, int n, double *vetorB, double *vetorU) {
	int i;

	for(i = 1; i < n-1; i++) {
		vetorU[i-1] = 6 * (vetorB[i] - vetorB[i-1]);
	}
}

void preencherMatrizA(int n, double *vetorV, double *vetorH, double matrizA[n-2][n-2]) {
	int i, j;

	for(i = 0; i < n-2; i++) {
		for(j = 0; j < n-2; j++) {
			if(j == i)
				matrizA[i][j] = vetorV[i];
			else if(j == i + 1)
				matrizA[i][j] = vetorH[i];
			else if(j == i - 1)
				matrizA[i][j] = vetorH[j];
			else
				matrizA[i][j] = 0;
		}
	}
}

double LU_subtrairLinhas(int n, double matrizU[n-2][n-2], int i) {
	int j;
	double m, *vetor1, *vetor2;

	vetor1 = matrizU[i];
	vetor2 = matrizU[i-1];
	m = vetor1[i-1] / vetor2[i-1];
	for(j = 0; j < n-2; j++) {
		vetor1[j] -= vetor2[j]*m;
	}

	return m;
}

void fatoracaoLU(int n, double matrizA[n-2][n-2], double matrizL[n-2][n-2], double matrizU[n-2][n-2]) {
	int i, j;
	double m;

	/* Copia a matrizA para a matrizU */
	for(i = 0; i < n-2; i++) {
		for(j = 0; j < n-2; j++) {
			matrizU[i][j] = matrizA[i][j];
		}
	}

	/* Inicializa matriz L */
	for(i = 0; i < n-2; i++) {
		for(j = 0; j < n-2; j++) {
			if(i == j)
				matrizL[i][j] = 1;
			else
				matrizL[i][j] = 0;
		}
	}

	/* Zera o primeiro termo da linha i subtraindo linhas */
	for(i = 1; i < n-2; i++) {
		m = LU_subtrairLinhas(n, matrizU, i);

		/* Coloca o número multiplicador da linha na matriz L */
		matrizL[i][i-1] = m;
	}
}

double subprodutoLYU(int i, int n, double matrizL[n-2][n-2], double *vetorY) {
	double soma = 0;

	if(i > 0) {
		soma = matrizL[i][i-1]*vetorY[i-1];
	}
	return soma;
}

void sistemaLYU(int n, double matrizL[n-2][n-2], double *vetorY, double *vetorU) {
	int i;

	for(i = 0; i < n-2; i++) {

			vetorY[i] = vetorU[i] - subprodutoLYU(i, n, matrizL, vetorY);
	}
}

double subprodutoUZY(int n, int i,  double matrizU[n-2][n-2], double *vetorZ) {
	double soma = 0;

	if( i < (n-2)-1)  {
		soma = matrizU[i][i+1]*vetorZ[i+1];
	}

	return soma;
}

void sistemaUZY(int n, double matrizU[n-2][n-2], double *vetorZ, double *vetorY) {
	int i;

	for(i = (n-2)-1; i >= 0; i--) {
		vetorZ[i] = (vetorY[i] - subprodutoUZY(n, i, matrizU, vetorZ)) / matrizU[i][i];
	}
}

void spline(double **matriz, int n, double *vetorZ) {
	double matrizA[n-2][n-2];     							   // matrizes do sistema matrizA.matrizZ = vetorU
	double matrizL[n-2][n-2], matrizU[n-2][n-2];               // matrizes da fatoracao LU
	double vetorH[n-1], vetorB[n-1], vetorV[n-2], vetorU[n-2]; // vetores auxiliares
	double vetorY[n-2];										   // vetor do sistema matrizL.vetorY = vetorU
	
	/* Relacões */
	/*  hi = xi+1 − xi
		bi = (1/hi)*(yi+1 − yi)
		vi = 2 (hi−1 + hi) 
		ui = 6 (bi − bi−1)
	*/
	/*Preenche os vetores segundo as relacoes supracitadas */
	preencherVetorH(matriz, n, vetorH);
	preencherVetorB(matriz, n, vetorH, vetorB);
	preencherVetorV(matriz, n, vetorH, vetorV);
	preencherVetorU(matriz, n, vetorB, vetorU);

	/* Preence a matriz A do sistema A.Z = U */
	preencherMatrizA(n, vetorV, vetorH, matrizA);

	/* Fatora a matrizA e armazena os resultados na matrizL e matrizU */
	fatoracaoLU(n, matrizA, matrizL, matrizU);

	/* Calcula o sistema matrizL.vetorY = vetorU */
	sistemaLYU(n, matrizL, vetorY, vetorU);

	/* Calcula o sistema matrizU.vetorZ = vetorY */
	sistemaUZY(n, matrizU, vetorZ, vetorY);
}