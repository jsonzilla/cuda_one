#ifndef MATRIXMUL_H_
#define MATRIXMUL_H_

extern "C"
void computeGold(
	float* P,
	const float* M, 
	const float* N, 
	int Mh,
	int Mw, 
	int Nw);

#endif
