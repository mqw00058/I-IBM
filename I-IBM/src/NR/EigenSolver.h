#pragma once

#include "NR_EigenJacobi.h"
#include <iostream>
#include <iomanip>

using namespace std;
//==========================================================================
// Type Definition for Numerical Recipes

typedef NRVec<double> Vec_DP, Vec_I_DP, Vec_O_DP;
typedef NRVec<int> Vec_INT, Vec_I_INT, Vec_O_INT;
typedef NRMat<double> Mat_DP;
typedef double DP;
//==========================================================================

class EigenSolver
{
public:
	int matSize_;
	Mat_DP matA_, matV_;
	Vec_DP eigenValue_;

	
public:
	EigenSolver(void);
	~EigenSolver(void);

	void SolveEigenSystem(int _matSize, double **_matA);
	void SortEigenvectors();
};
