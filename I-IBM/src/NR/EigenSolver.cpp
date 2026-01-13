//#include "StdAfx.h"
#include "EigenSolver.h"


EigenSolver::EigenSolver(void)
{
	matA_		= NULL;
	matV_		= NULL;
	eigenValue_ = NULL;
}

EigenSolver::~EigenSolver(void)
{
}

void EigenSolver::SolveEigenSystem(int _matSize, double **_matA)
{
	matSize_ = _matSize;

	int i,j;
	int nrot;		// number of JACOBI rotations

	matA_		= Mat_DP(matSize_, matSize_);
	matV_		= Mat_DP(matSize_, matSize_);
	eigenValue_ = Vec_DP(matSize_);

	//init matA_
	for(i=0; i<matSize_;i++){
		for(j=0; j<matSize_; j++){
			matA_[i][j]=_matA[i][j];
		}
	}

	NR::jacobi(matA_,eigenValue_,matV_,nrot);

	qDebug() << "****** Finding Eigenvectors ******" << endl;
	qDebug() << "number of JACOBI rotations: " << nrot << endl;
	qDebug() << "eigenvalues: " << endl;
	for (i=0;i<matSize_;i++) 
	{
		qDebug() <</* setw(12) <<*/ eigenValue_[i];
		if ((i+1) % 5 == 0) qDebug() << endl;
	}
	qDebug() << endl << "unsorted eigenvectors:" << endl;
	for (i=0;i<matSize_;i++) 
	{
		qDebug() << /*setw(9) <<*/ "number" << /*setw(4) <<*/ (i+1) << endl;
		for (j=0;j<matSize_;j++) 
		{
			qDebug() << /*setw(12) <<*/ matV_[j][i];
			if ((j+1) % 5 == 0) qDebug() << endl;
		}
		qDebug() << endl;
	}
}

void EigenSolver::SortEigenvectors()
{
	int i,j;

	NR::eigsrt(eigenValue_, matV_);
	
	qDebug() << endl << "****** Sorting Eigenvectors ******";
	qDebug() << endl << "eigenvalues: " << endl;
	for (i=0;i<matSize_;i++) 
	{
		qDebug() << /*setw(12) *//*<<*/ eigenValue_[i];
		if ((i+1) % 5 == 0) qDebug() << endl;
	}
	qDebug() << endl << "sorted eigenvectors:" << endl;
	for (i=0;i<matSize_;i++) 
	{
		qDebug() << /*setw(9) <<*/ "number" << /*setw(4) <<*/ (i+1) << endl;
		for (j=0;j<matSize_;j++) 
		{
			qDebug() << /*setw(12) <<*/ matV_[j][i];
			if ((j+1) % 5 == 0) qDebug() << endl;
		}
		qDebug() << endl;
	}
	qDebug() << endl;
}