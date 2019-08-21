//#define S1_Baseline
//#define S2_Remove_if
//#define S3_Cycle_Partition
//#define S4_Manual_Unroll_TDL
//#define S5_Unroll_TDL
//#define S6_Unroll_MAC
//#define S7_ARRAY_PARTITION
//#define S8_Manual_Unroll_MAC
#define S9_Pipeline

#ifdef S1_Baseline
//*******************S1_Baseline



/*
	Filename: fir.cpp
		FIR lab wirtten for WES/CSE237C class at UCSD.

	INPUT:
		x: signal (chirp)

	OUTPUT:
		y: filtered output

*/

#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;

	Shift_Accum_Loop:
		for (i = N - 1; i >= 0; i--) {
			if (i == 0) {
				acc += x * c[0];
				shift_reg[0] = x;
			}
			else {
				shift_reg[i] = shift_reg[i - 1];
				acc += shift_reg[i] * c[i];
			}
		}
		*y = acc;
}
#endif

#ifdef S2_Remove_if
//*******************S2_Remove_if
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;

	Shift_Accum_Loop:

		for (i = N - 1; i > 0; i--) {
			shift_reg[i] = shift_reg[i - 1];
			acc += shift_reg[i] * c[i];
		}

		acc += x * c[0];
		shift_reg[0] = x;
		*y = acc;
}


#endif


#ifdef S3_Cycle_Partition
//*******************S3_Cycle_Partition
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;
	MAC:
	for (i = N - 1; i >= 0; i--) {
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif



#ifdef S4_Manual_Unroll_TDL
//*******************S4_Manual_Unroll_TDL
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 1; i = i - 2) {
		shift_reg[i] = shift_reg[i - 1];
		shift_reg[i - 1] = shift_reg[i - 2];
	}
	if (i == 1) {
		shift_reg[1] = shift_reg[0];
	}
	shift_reg[0] = x;

	acc = 0;
	MAC:
	for (i = N - 1; i >= 0; i--) {
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif


#ifdef S5_Unroll_TDL
//*******************S5_Unroll_TDL
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
#pragma HLS UNROLL factor=2
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;
	MAC:
	for (i = N - 1; i >= 0; i--) {
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif



#ifdef S6_Unroll_MAC
//*******************S6_Unroll_MAC
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
	// Write your code here
	static data_t shift_reg[N];

	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
#pragma HLS UNROLL factor=2
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;
	MAC:
	for (i = N - 1; i >= 0; i--) {
#pragma HLS UNROLL factor=4
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif



#ifdef S7_ARRAY_PARTITION
//*******************S7_ARRAY_PARTITION
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
#pragma HLS ARRAY_PARTITION variable=c complete dim=1
	// Write your code here
	static data_t shift_reg[N];
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
#pragma HLS UNROLL skip_exit_check factor=2
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;
	MAC:
	for (i = N - 1; i >= 0; i--) {
#pragma HLS UNROLL factor=4
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif



#ifdef S8_Manual_Unroll_MAC
//*******************S8_Manual_Unroll_MAC
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
#pragma HLS ARRAY_PARTITION variable=c complete dim=1
	// Write your code here
	static data_t shift_reg[N];
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
#pragma HLS UNROLL skip_exit_check factor=11
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;


	MAC:
	for (i = N - 1; i >= 2; i -= 3) {
		acc += 	shift_reg[i] * c[i] + 
				shift_reg[i - 1] * c[i - 1] +
				shift_reg[i - 2] * c[i - 2] +
				shift_reg[i - 3] * c[i - 3];
	}

	for (; i >= 0; i--) {
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif

#ifdef S9_Pipeline
//*******************S9_Pipeline
#include "fir.h"

void fir (data_t *y,data_t x)
{
	coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
#pragma HLS ARRAY_PARTITION variable=c complete dim=1
	// Write your code here
	static data_t shift_reg[N];
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1
	acc_t acc;
	int i;

	acc = 0;

	TDL:
	for (i = N - 1; i > 0; i--) {
#pragma HLS UNROLL skip_exit_check factor=11
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;


	MAC:
	for (i = N - 1; i >= 2; i -= 3) {
#pragma HLS PIPELINE
		acc += 	shift_reg[i] * c[i] +
				shift_reg[i - 1] * c[i - 1] +
				shift_reg[i - 2] * c[i - 2] +
				shift_reg[i - 3] * c[i - 3];
	}

	for (; i >= 0; i--) {
#pragma HLS PIPELINE
		acc += shift_reg[i] * c[i];
	}
	*y = acc;
}


#endif
