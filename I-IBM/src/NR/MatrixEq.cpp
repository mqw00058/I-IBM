//#include "StdAfx.h"
#include ".\MatrixEq.h"


MatrixEq::MatrixEq(void)
: sizeN(0)
{
	NR_inverseA = NULL;
	NR_solX_GJ = NULL;
	NR_solX_LU = NULL;
	NR_matA = NULL;
	NR_indx = NULL;
	inverseA = NULL;
	solX = NULL;
}

MatrixEq::~MatrixEq(void)
{
	//if ( *inverseA )	delete *inverseA;
    //if ( solX ) delete solX;	
}



//int main(void)
//{
//	int temp; //##
//
//	MatrixEq matEq;
//	
//	double** t_matA;
//	double** t_inverseA;
//	double* t_vecB;
//	double* t_solX;
//
//	double t_detA;
//	int t_sizeN=2;
//	
//	t_matA = NULL;
//	t_inverseA = NULL;
//	t_vecB = NULL;
//	t_solX = NULL;
//	t_matA = new double* [t_sizeN];
//	t_inverseA = new double* [t_sizeN];
//	t_vecB = new double [t_sizeN];
//	t_solX = new double [t_sizeN];
//
//	for(int i=0; i<t_sizeN ; i++)
//		t_matA[i]= new double [t_sizeN];
//	for(int i=0; i<t_sizeN ; i++)
//		t_inverseA[i]= new double [t_sizeN];
//
//
//	//*******************Get Inverse Matrix of A using Gauss-Jordan****************************//
//	//init matrixA
//	t_matA[0][0]=1;		t_matA[0][1]=2;		
//	t_matA[1][0]=1;		t_matA[1][1]=1;		
//	
//	//print matrix A
//	qDebug() << "MAIN : Matrix A : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		for(int j=0; j<t_sizeN; j++){
//            qDebug() <<  setw(5) << t_matA[i][j];
//		}
//		qDebug() << endl;
//	}
//	
//	t_inverseA = matEq.GJinverse(t_sizeN, t_matA);
//	
//	//print inverse matrix A'
//	qDebug() << "MAIN : Inverse of matrix A : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		for(int j=0; j<t_sizeN; j++){
//            qDebug() <<  setw(5) << t_inverseA[i][j];
//		}
//		qDebug() << endl;
//	}
//	
//
//	//*******************Get Solution X using Gauss-Jordan****************************//
//	
//	//init matrixA
//	t_matA[0][0]=1;		t_matA[0][1]=2;		
//	t_matA[1][0]=1;		t_matA[1][1]=1;		
//	
//	//init vecB
//	t_vecB[0]=2;		t_vecB[1]=3;
//
//	//print matrix A
//	qDebug() << "MAIN : Matrix A : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		for(int j=0; j<t_sizeN; j++){
//            qDebug() <<  setw(5) << t_matA[i][j];
//		}
//		qDebug() << endl;
//	}
//	//print vector B
//	qDebug() << "MAIN : Vector B : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		qDebug() <<  setw(5) << t_vecB[i];
//		qDebug() << endl;
//	}
//		
//	t_solX = matEq.GJsolveX(t_sizeN, t_matA, t_vecB);
//
//	//print solution X
//	qDebug() << "MAIN : Solution X : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		qDebug() <<  setw(5) << t_solX[i];
//		qDebug() << endl;
//	}
//	
//	
//	//*******************LU decomposition of A****************************//
//	
//	//init matrixA
//	t_matA[0][0]=1;		t_matA[0][1]=2;		
//	t_matA[1][0]=1;		t_matA[1][1]=1;	
//	
//	//print matrix A
//	qDebug() << "MAIN : Matrix A : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		for(int j=0; j<t_sizeN; j++){
//            qDebug() <<  setw(5) << t_matA[i][j];
//		}
//		qDebug() << endl;
//	}
//
//	t_detA = matEq.LUdecomposition(t_sizeN, t_matA);
//	
//	qDebug() << endl << "MAIN : Determinant of A : " << t_detA << endl;
//
//
//	//*******************LU solve of X****************************//
//	
//	//init matrixA
//	t_matA[0][0]=1;		t_matA[0][1]=2;		
//	t_matA[1][0]=1;		t_matA[1][1]=1;	
//	
//	//init vecB
//	t_vecB[0]=2;		t_vecB[1]=3;
//
//	//print matrix A
//	qDebug() << "MAIN : Matrix A : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		for(int j=0; j<t_sizeN; j++){
//            qDebug() <<  setw(5) << t_matA[i][j];
//		}
//		qDebug() << endl;
//	}
//
//	//print vector B
//	qDebug() << "MAIN : Vector B : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		qDebug() <<  setw(5) << t_vecB[i];
//		qDebug() << endl;
//	}
//
//    t_solX = matEq.LUsolveX(t_sizeN, t_vecB);
//
//	//print solution X
//	qDebug() << "MAIN : Solution X : " << endl;
//	for(int i=0; i<t_sizeN;i++){
//		qDebug() <<  setw(5) << t_solX[i];
//		qDebug() << endl;
//	}
//
//}

double** MatrixEq::GJinverse(int t_sizeN, double** t_matA)
{
	sizeN = t_sizeN;
	
	//variable for gaussjd
	NR_inverseA = Mat_DP(sizeN,sizeN);
	NR_solX_GJ = Mat_DP(sizeN,1);
	
	//inti inverse matrix A as double**
	inverseA = NULL;
	inverseA = new double* [t_sizeN];
	for(int i=0; i<t_sizeN ; i++)
		inverseA[i]= new double [t_sizeN];


	//init matrix A and inverse matrix A'
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
			NR_inverseA[i][j]=t_matA[i][j];
		}
	}
	
	//init solution x;
	for(int i=0;i<sizeN;i++){
		NR_solX_GJ[0][i]=1.0;
	}
	
	
	//print matrix A
	/*qDebug() << "MatrixEq:: Matrix A : " << endl;
	
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
            qDebug() <<  setw(5) << NR_inverseA[i][j];
		}
		qDebug() << endl;
	}*/
	

	NR::gaussj(NR_inverseA,NR_solX_GJ);
	
	
	//print inverse matrix A'
	/*qDebug() << "MatrixEq:: Inverse of matrix A : " << endl;
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
            qDebug() <<  setw(5) << NR_inverseA[i][j];
		}
		qDebug() << endl;
	}*/
	

	//make return inverseA
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
			inverseA[i][j] = NR_inverseA[i][j];
		}
	}
    
	return inverseA;
}

double* MatrixEq::GJsolveX(int t_sizeN, double** t_matA, double* t_vecB)
{
	sizeN = t_sizeN;
	
	//variable for gaussjd
	NR_inverseA = Mat_DP(sizeN,sizeN);
	NR_solX_GJ = Mat_DP(sizeN,1);
	
	//inti inverse matrix A as double**
	inverseA = NULL;
	solX = NULL;
	inverseA = new double* [t_sizeN];
	solX = new double [t_sizeN];

	for(int i=0; i<t_sizeN ; i++)
		inverseA[i]= new double [t_sizeN];


	//init matrix A and inverse matrix A'  &&  solution X and vector B
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
			NR_inverseA[i][j]=t_matA[i][j];
		}
		NR_solX_GJ[0][i] = t_vecB[i];
	}
		
	
	//print matrix A
	/*qDebug() << "MatrixEq:: Matrix A : " << endl;
		for(int i=0; i<sizeN;i++){
			for(int j=0; j<sizeN; j++){
				qDebug() <<  setw(5) << NR_inverseA[i][j];
			}
			qDebug() << endl;
		}*/
	

	NR::gaussj(NR_inverseA,NR_solX_GJ);
	
	
	//print solution X
	/*qDebug() << "MatrixEq:: Solution X : " << endl;
	for(int i=0; i<t_sizeN;i++){
		qDebug() <<  setw(5) << NR_solX_GJ[0][i];
		qDebug() << endl;
	}*/
	

	//make return solX
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
			inverseA[i][j] = NR_inverseA[i][j];
		}
		solX[i] = NR_solX_GJ[0][i];
	}
	
    
	return solX;
	
}

double MatrixEq::LUdecomposition(int t_sizeN, double** t_matA)
{
	double detA = 0;
	sizeN = t_sizeN;
	
	NR_matA = NULL;

	NR_matA = Mat_DP(sizeN, sizeN);
	NR_indx = Vec_I_INT(sizeN);

	//init matrix A and inverse matrix A'
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
			NR_matA[i][j]=t_matA[i][j];
		}
	}
	
	
	//print matrix A
	/*qDebug() << "MatrixEq:: Matrix A : " << endl;
	
	for(int i=0; i<sizeN;i++){
		for(int j=0; j<sizeN; j++){
            qDebug() <<  setw(5) << NR_matA[i][j];
		}
		qDebug() << endl;
	}*/
	

	NR::ludcmp(NR_matA, NR_indx, NR_d);
	
	detA = NR_d;

	//qDebug() << "MatrixEq:: NR_d : " << NR_d << endl;

	for(int i=0; i<sizeN; i++){
		detA *= NR_matA[i][i];
	}
	
	//qDebug() << "MatrixEq:: Determinant of A : " << detA << endl;
	

	return detA;
}

double* MatrixEq::LUsolveX(int t_sizeN, double* t_vecB)
{
	sizeN = t_sizeN;
	NR_solX_LU = Vec_DP(sizeN);

	solX = NULL;
	solX = new double [t_sizeN];

	//init matrix A and inverse matrix A'  &&  solution X and vector B
	for(int i=0; i<sizeN;i++){
		NR_solX_LU[i] = t_vecB[i];
	}

	NR::lubksb(NR_matA, NR_indx, NR_solX_LU);

	//make return solX
	for(int i=0; i<sizeN;i++){
		solX[i] = NR_solX_LU[i];
	}
	    
	return solX;
}
