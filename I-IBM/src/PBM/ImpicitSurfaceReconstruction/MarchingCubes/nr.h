/* This code is from Numerical Recipes in C.
 * It is used only for studying and testing.
 * No commercial uses.
 * Copyright (C) Numerical Recipes */

#ifndef _NR_H_
#define _NR_H_
#include <fstream>
#include <complex>
#include "nrutil.h"
#include "nrtypes.h"
using namespace std;

namespace MarchingCubes{
namespace NR {
	void gaussj(Mat_IO_DP &a, Mat_IO_DP &b);
	void jacobi(Mat_IO_DP &a, Vec_O_DP &d, Mat_O_DP &v, int &nrot);
	void eigsrt(Vec_O_DP &d, Mat_O_DP &v);
}
}
#endif /* _NR_H_ */
