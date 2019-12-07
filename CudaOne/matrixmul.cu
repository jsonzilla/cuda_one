/* Matrix multiplication: C = A * B.
 * Host code.
 */

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <chrono>


#include <cuda_runtime.h>

// includes, kernels


#include "assist.h"

#include "matrixmul.h"

int main(int argc, char** argv) {
	bool if_quiet = false;
	unsigned int timer_compute = 0;
	char* matrix_id = NULL;
	char* input_fn = NULL;
	char* gold_fn = NULL;
	int Mw = 0;
	int Mh = 0;
	int Nw = 0;
	int Nh = 0;
	int Pw = 0;
	int Ph = 0;

	if (argc == 2) {
		matrix_id = strdup(argv[1]);
	}
	else {
		fprintf(stderr, "Error: Wrong input parameter numers.\n");
		fprintf(stderr, "Usage:\n"
			"$> ./lab1.1-matrixmul <8, 128, 512, 3072, 4092>\n"
			"Examples:\n "
			" $> ./bab1.1-matrixmul 128\n"
		);
		exit(1);
	}

	Mw = Mh = Mw = Nh = Pw = Ph = atoi(matrix_id);
	input_fn = (char*)malloc(30 * sizeof(char));
	gold_fn = (char*)malloc(30 * sizeof(char));
	sprintf(input_fn, "matrix_%s.bin", matrix_id);
	sprintf(gold_fn, "matrix_%s.gold", matrix_id);
	if (Pw * Ph > 15 * 15) {
		if_quiet = true; // dont show content
	}
	printf("Input matrix size: %d by %d\n", Mw, Mh);

	printf("Setup host side environment:\n");

	printf(" Allocate host memory for matrices M and N.\n");
	printf(" M: %d x %d\n", Mw, Mh);
	printf(" N: %d x %d\n", Nw, Nh);

	unsigned int size_M = Mw * Mh;
	unsigned int mem_size_M = sizeof(float) * size_M;
	float* hostM = (float*)malloc(mem_size_M);
	unsigned int size_N = Nw * Nh;
	unsigned int mem_size_N = sizeof(float) * size_N;
	float* hostN = (float*)malloc(mem_size_N);

	printf(" Allocate memory for the result on the host side.\n");
	unsigned int size_P = Pw * Ph;
	unsigned int mem_size_P = sizeof(float) * size_P;
	float* hostP = (float*)malloc(mem_size_P);

	printf(" Generate input matrix data for matrix M and N.\n");
	GenMatrixFile(input_fn, Pw, Ph, if_quiet);
	unsigned int* matrix = ReadMatrixFile(input_fn, Pw, Ph, true);

	for (int i = 0; i < Mw; i++) {
		for (int j = 0; j < Nw; j++) {
			hostM[i * Mw + j] = hostN[i * Mw + j] = (float)matrix[i * Mw + j];
		}
	}
	free(matrix);
	matrix = NULL;

	printf(" Copmuting matrix multiplication M x N:\n");
	if (Pw*Ph > 512*512) {
		printf(" (It takes time since matrix is larger than 512 by 512\n");
	}

	float* reference = (float*)malloc(mem_size_P);
	computeGold(reference, hostM, hostN, Mh, Mw, Nw);

	printf(" CPU Processing time : %f (ms)\n", 0.0);
	printf(" Matrix data checksum : %g\n", CheckSum(reference, Mw, Nw));

	if (!if_quiet) {
		printf(" matrix data contens:\n");
		printf(" ");
	}

	matrix = (unsigned int*)malloc(Pw * Ph * sizeof(unsigned int));
	for (int i = 0; i < Ph; i++) {
		for (int j = 0; j < Pw; j++) {
			matrix[i * Pw + j] = (unsigned int)reference[i * Pw + j];
			if (!if_quiet) printf("%u ", matrix[i * Pw + j]);
		}
		if (!if_quiet) printf("\n   ");
	}

	if (!if_quiet) printf("\n");

	WriteMatrixFile(gold_fn, matrix, Pw, Ph, 1);
	free(matrix);
	matrix = NULL;

	free(hostM);
	free(hostN);
	free(hostP);

	return 0;
}