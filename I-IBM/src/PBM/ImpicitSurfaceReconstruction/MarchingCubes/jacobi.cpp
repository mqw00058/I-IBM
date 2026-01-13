/* This code is from Numerical Recipes in C.
 * It is used only for studying and testing.
 * No commercial uses.
 * Copyright (C) Numerical Recipes */

#include <cmath>
#include "nr.h"
using namespace std;

namespace MarchingCubes{
namespace {
    inline void rot(Mat_IO_DP &a, const DP s, const DP tau, const int i,
                    const int j, const int k, const int l){
	DP g, h;
	g = a[i][j];
	h = a[k][l];
	a[i][j] = g - s * (h + g * tau);
	a[k][l] = h + s * (g - h * tau);
    }
}

void NR::jacobi(Mat_IO_DP &a, Vec_O_DP &d, Mat_O_DP &v, int &nrot){
/* Computes all eigenvalues and eigenvectors of a real symmetric matrix 
 * a[0..n-1][0..n-1]. On output, elements of a above the diagnoal are
 * destroyed. d[0..n-1] returns the eigenvalues of a. v[0..n-1][0..n-1]
 * is a matrix whose columns contain, on output, the normalized eigenvectors
 * of a. nrot returns the number of Jacobi rotations that were required.
 */
    int i, j, ip, iq;
    DP tresh, theta, tau, t, sm, s, h, g, c;

    int n = d.size();
    Vec_DP b(n), z(n);
    for (ip = 0; ip < n; ip ++){		// Initialize to the identity matrix.
	for (iq = 0; iq < n; iq ++)
            v[ip][iq] = 0.0;
            v[ip][ip] = 1.0;
    }
    for (ip = 0; ip < n; ip ++){		// Initialize b and d to the diagnoal
	b[ip] = d[ip] = a[ip][ip];		// of a.
	z[ip] = 0.0;				// This vector will accumulate terms
    }						// of the form tapq as in equation 
						// (11.1.14).
    nrot = 0;
    for (i = 1; i <= 50; i ++){
	sm = 0.0;
	for (ip = 0; ip < n - 1; ip ++){	// Sum magnitude of off-diagonal
            for (iq = ip + 1; iq < n; iq ++)    // elements.
		sm += fabs(a[ip][iq]);
        }
	if (sm == 0.0)				// The normal return, which relies
            return;				// on quadratic convergence to
	if (i < 4)				// machine underflow.
            tresh = 0.2 * sm / (n * n);         // ...on the first three sweeps.
	else
            tresh = 0.0;			// ...thereafter.
        for (ip = 0; ip < n - 1; ip ++){
            for (iq = ip + 1; iq < n; iq ++){
		g = 100.0 * fabs(a[ip][iq]);
		// After four sweeps, skip the rotation if the off-diagonal
		// element is small.
		if (i > 4 && (fabs(d[ip]) + g) == fabs(d[ip]) &&
                    (fabs(d[iq]) + g) == fabs(d[iq]))
                    a[ip][iq] = 0.0;
		else if (fabs(a[ip][iq]) > tresh){
                    h = d[iq] - d[ip];
                    if ((fabs(h) + g) == fabs(h))
			t = (a[ip][iq]) / h;
                    else {
			theta = 0.5 * h / (a[ip][iq]);
			t = 1.0 / (fabs(theta) + sqrt(1.0 + theta * theta));
                        if (theta < 0.0) t = -t;
                    }
                    c = 1.0 / sqrt(1 + t * t);
                    s = t * c;
                    tau = s / (1.0 + c);
                    h = t * a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    d[ip] -= h;
                    d[iq] += h;
                    a[ip][iq] = 0.0;
                    for (j = 0; j < ip; j ++)		// Case of rotations 
			rot(a, s, tau, j, ip, j, iq);	// 	0 <= j < p
                    for (j = ip + 1; j < iq; j ++)	// Case of rotations
			rot(a, s, tau, ip, j, j, iq);	//	p < j < q
                    for (j = iq + 1; j < n; j ++)	// Case of rotations
			rot(a, s, tau, ip, j, iq, j);	//	q < j < n
                    for (j = 0; j < n; j ++)
			rot(v, s, tau, j, ip, j, iq);
			++nrot;
                    }
            }
	}
	for (ip = 0; ip < n; ip ++){
            b[ip] += z[ip];
            d[ip] = b[ip];
            z[ip] = 0.0;
        }
    }
    nrerror("Too many iterations in routine jacobi.");
}
}