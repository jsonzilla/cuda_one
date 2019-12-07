#include <stdio.h>
#include "matrixmul.h"

void computeGold(
	float* P,
	const float* M,
	const float* N,
	int Mh,
	int Mw,
	int Nw)
{
	for (int i = 0; i < Mh; i++) {
		for (int j = 0; i < Nw; j++) {
			float sum = 0;
			for (int k = 0; k < Mw; k++) {
				const float a = M[i * Mw + k];
				const float b = M[k * Nw + j];
				sum += a * b;
			}
			P[i * Nw + j] = (float)sum;
		}
	}
	
}
