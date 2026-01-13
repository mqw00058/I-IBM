#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include "NR.h"
using namespace std;



//==========================================================================
// Type Definition for Numerical Recipes

typedef NRVec<double> Vec_DP, Vec_I_DP, Vec_O_DP,Vec_IO_DP;
typedef NRVec<int> Vec_INT, Vec_I_INT, Vec_O_INT,Vec_I_INT;
typedef NRMat<double> Mat_DP, Mat_IO_DP,Mat_I_DP;
typedef double DP;
//==========================================================================


namespace NR { 
		void gaussj(Mat_IO_DP &a, Mat_IO_DP &b);
		void ludcmp(Mat_IO_DP &a, Vec_O_INT &indx, DP &d);
		void lubksb(Mat_I_DP &a, Vec_I_INT &indx, Vec_IO_DP &b);
		template<class T>
		inline void SWAP(T &a, T &b)
			{T dum=a; a=b; b=dum;}

		inline void nrerror(const string error_text)
			//Numerical Recipes standard error handler
			{
				cerr << "Numerical Recipes run-time error..." << endl;
				cerr << error_text << endl;
				cerr << "...now exiting to system..." << endl;
				exit(1);
			}

}