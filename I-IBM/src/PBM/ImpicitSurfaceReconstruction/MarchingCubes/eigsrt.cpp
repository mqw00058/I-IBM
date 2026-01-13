/* This code is from Numerical Recipes in C.
 * It is used only for studying and testing.
 * No commercial uses.
 * Copyright (C) Numerical Recipes */

#include "nr.h"

namespace MarchingCubes{
void NR::eigsrt(Vec_IO_DP &d, Mat_IO_DP &v){
/* Given the eigenvalue d[0..n-1] and eigenvectors v[0..n-1][0..n-1] as output 
 * from jacobi or tqli, this routine sorts the eigenvalues into descending
 * order, and rearranges the columns of v correspondingly. The method is
 * straight insertion.
 */
    int i, j, k;
    DP p;

    int n = d.size();
    for (i = 0; i < n - 1; i ++){
	k = i;
	p = d[k];
	for (j = i; j < n; j ++)
            if (d[j] >= p){
		k = j;
		p = d[k];
            }
	if (k != i){
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j ++){
		p = v[j][i];
		v[j][i] = v[j][k];
		v[j][k] = p;
            }
	}
    }
}
}