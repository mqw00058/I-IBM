#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include "NR.h"
using namespace std;

//==========================================================================
// Type Definition for Numerical Recipes

typedef NRVec<double> Vec_O_DP, Vec_IO_DP, Vec_DP;
typedef NRMat<double> Mat_IO_DP, Mat_O_DP;
typedef double DP;
//==========================================================================

namespace NR 
{
	inline void rot(Mat_IO_DP &a, const DP s, const DP tau, const int i,
		const int j, const int k, const int l)
	{
		DP g,h;

		g=a[i][j];
		h=a[k][l];
		a[i][j]=g-s*(h+g*tau);
		a[k][l]=h+s*(g-h*tau);
	}

	void jacobi(Mat_IO_DP &a, Vec_O_DP &d, Mat_O_DP &v, int &nrot);
	void eigsrt(Vec_IO_DP &d, Mat_IO_DP &v);

	//inline void nrerror(const string error_text)
	//		//Numerical Recipes standard error handler
	//		{
	//			cerr << "Numerical Recipes run-time error..." << endl;
	//			cerr << error_text << endl;
	//			cerr << "...now exiting to system..." << endl;
	//			exit(1);
	//		}


} // end of namespace NR