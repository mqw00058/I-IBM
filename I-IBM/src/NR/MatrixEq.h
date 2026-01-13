#pragma once


//#include "NR.h"
#include "NR_Matrix.h"

using namespace NR;

//==========================================================================
// Type Definition for Numerical Recipes

typedef NRVec<double> Vec_DP, Vec_I_DP, Vec_O_DP;
typedef NRVec<int> Vec_INT, Vec_I_INT, Vec_O_INT;
typedef NRMat<double> Mat_DP, Mat_IO_DP;
typedef double DP;
//==========================================================================

class MatrixEq
{
public:
	double** inverseA;
	double* solX;
	int sizeN;
	Mat_DP NR_matA, NR_inverseA, NR_solX_GJ;
	Vec_INT NR_indx;
	Vec_DP NR_solX_LU;
	DP NR_d;

public:
	MatrixEq(void);
	~MatrixEq(void);
	double** GJinverse(int t_sizeN, double** t_matA);
	
	double* GJsolveX(int t_sizeN, double** t_matA, double* t_vecB);

	double LUdecomposition(int t_sizeN, double** t_matA);
	
	double* LUsolveX(int t_sizeN, double* t_vecB);;
};
