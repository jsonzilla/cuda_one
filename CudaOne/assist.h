#ifndef ASSIST_H_
#define ASSIST_H_

#include <stdio.h>

FILE* OpenFile(
	const char* const fn_p, 
	const char* const open_mode_p, 
	const int if_silent)
{
	FILE* f_p = nullptr;

	if (fn_p == nullptr) {
		printf("Null file name pointer.");
		exit(-1);
	}
	if (!if_silent) {
		fprintf(stdout, "Opening the file %s... ", fn_p);
	}

	f_p = fopen(fn_p, open_mode_p);
	if (f_p == nullptr) {
		if (!if_silent) {
			fprintf(stdout, "failed.\n");
		}
		else {
			fprintf(stdout, "\nOpening the file %s ... failed.\n\n", fn_p);
		}
		exit(-1);
	}

	if (!if_silent) fprintf(stdout, "succedd.\n");

	return f_p; 
}

int GenMatrixFile(
	const char* const matrix_fn_p, 
	const unsigned int M_WIDTH, 
	const unsigned int M_HEIGHT,
	const int if_silent)
{
	const unsigned int M_SIZE = M_WIDTH * M_HEIGHT;
	FILE* matrix_fp = OpenFile(matrix_fn_p, "wb", if_silent);
	unsigned int* matrix = (unsigned int*)malloc(M_SIZE * sizeof(unsigned int));

	if (!if_silent) fprintf(stdout, "  ");
	for (unsigned int i = 0; i < M_HEIGHT; i++) {
		for (unsigned int j = 0; j < M_WIDTH; j++) {
			matrix[i * M_WIDTH + j] = i + j + 1;
			if (!if_silent) fprintf(stdout, "%u ", matrix[i * M_WIDTH + j]);
		}
		if (!if_silent) fprintf(stdout, "\n  ");
	}
	if (!if_silent) fprintf(stdout, "\n");

	fwrite(matrix, 1, M_SIZE * sizeof(unsigned int), matrix_fp);

	fclose(matrix_fp);
	free(matrix); 
	matrix = nullptr;

	return 1;
}

unsigned int* ReadMatrixFile(
	const char* const matrix_fn_p, 
	const unsigned int M_WIDTH, 
	const unsigned int M_HEIGHT,
	const int if_silent)
{
	const unsigned int M_SIZE = M_WIDTH * M_HEIGHT;
	FILE* matrix_fp = OpenFile(matrix_fn_p, "rb", if_silent);
	unsigned int* matrix = (unsigned int*)malloc(M_SIZE * sizeof(unsigned int));
	fread(matrix, 1, M_SIZE * sizeof(unsigned int), matrix_fp);

	if (!if_silent) {
		fprintf(stdout, "Read contents of matrix:\n");
		fprintf(stdout, " ");
		for (unsigned int i = 0; i < M_HEIGHT; i++) {
			for (unsigned int j = 0; j < M_WIDTH; j++) {
				fprintf(stdout, "%u ", matrix[i * M_WIDTH + j]);
			}
			if (!if_silent) fprintf(stdout, "\n  ");
		}
		if (!if_silent) fprintf(stdout, "\n");
	}

	fclose(matrix_fp);

	return matrix;
}

int WriteMatrixFile(
	const char* const matrix_fn_p,
	const unsigned int* const matrix,
	const unsigned int M_WIDTH,
	const unsigned int M_HEIGHT,
	const int if_silent)
{
	const unsigned int M_SIZE = M_WIDTH * M_HEIGHT;
	FILE* matrix_fp = OpenFile(matrix_fn_p, "wb", if_silent);
	fwrite(matrix, 1, M_SIZE * sizeof(unsigned int), matrix_fp);

	if (!if_silent) {
		fprintf(stdout, "Written contents of matrix:\n");
		for (unsigned int i = 0; i < M_HEIGHT; i++) {
			for (unsigned int j = 0; j < M_WIDTH; j++) {
				fprintf(stdout, "%u ", matrix[i * M_WIDTH + j]);
			}
			if (!if_silent) fprintf(stdout, "\n  ");
		}
		if (!if_silent) fprintf(stdout, "\n");
	}

	fclose(matrix_fp);

	return 1;
}

float CheckSum(
	const float* matrix,
	const int width,
	const int height)
{
	float s1 = 0;
	float s2;

	for (int i = 0; i < width; i++) {
		s2 = 0;
		for (int j = 0; j < height; j++) {
			s2 += (const float)matrix[i * width + j];
		}
		s1 += s2;
	}

	return s1;
}

#endif

