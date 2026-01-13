//////////////////////////////////////////////////////////////////////////
// PBM::Feature::NormalEstimation class 
// Computes the normal vectors and eigenvalues of input point clouds using PCA.
//
//	$Date: 2011.03.03 $
//  $Created by Min Ki Park (minkp@gist.ac.kr)
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PBM_FEATURE_NORMNALESTIMATION_HH
#define PBM_FEATURE_NORMNALESTIMATION_HH

//#include "stdafx.h"
#include "GLWidget/gl.hh"
#include <NR/EigenSolver.h>
#include <NR/kdtree.h>

using namespace std;


using namespace NR;

namespace PBM {

class NormalEstimation
{

typedef NR::Point<3>		Point3;

public:
	float avgLen;


public:
	vector<Point3> point_set;
	KDtree<3> *kdt;

	// vertex property
	OpenMesh::Vec3f *ev;		//eigenvalue
	OpenMesh::Vec3f *normal;	//normal
	Point3 *centroid;	//center
	// the number of point cloud
	int nPts;


public:

	// constructor (parameter: point cloud <vector>type)
 	NormalEstimation(vector<OpenMesh::Vec3f> points){
 
  		for(vector<OpenMesh::Vec3f>::const_iterator it=points.begin()
  			; it != points.end(); ++it){
  
   				OpenMesh::Vec3f p = *it;
   				point_set.push_back( Point3( p[0], p[1], p[2] ) );
  		}

		/* the number of input points */
		nPts = points.size();
		// allocate memory
		ev			= new OpenMesh::Vec3f[nPts];	// eigen value
		normal		= new OpenMesh::Vec3f[nPts];	// normal
		centroid	= new Point3[nPts];	//center

		
		kdt = new KDtree<3> (point_set);

		compute_avgLen();
  
 	}

	// destructor
	virtual ~NormalEstimation(){

		delete [] ev;
		delete [] normal;
		delete kdt;
	}

	/**************************************************************************
	computer normal vectors of the vertices using PCA with k neighbor points 
    eigenvalue, normal of each vertex are calculated.
	***************************************************************************/
	void compute_normal(int k){
	
		//KDtree<3> kdt(point_set);
		
		// find k-nearest neighbor using kd-tree
		Int *knn = new Int[k];
		Doub *dn = new Doub[k];
		double** matC;			// Covariance matrix
		matC = new double* [3];
		// Initialize matC
		for(int i=0; i<3; i++) {
			matC[i] = new double [3];
		}
		for(int i=0; i<3; i++) {
			for(int j=0; j<3; j++) {
				matC[i][j] = 0;
			}
		}

		for(int idx = 0; idx < nPts; ++idx){

			Point3 p = point_set[idx];
			centroid[idx].x[0]=centroid[idx].x[1]=centroid[idx].x[2]=0.0;
			//								fixed point, range, knn, nMax
			//Int nret = kdt.locatenear(Point3(p[0],p[1],p[2]), 3, knn, 4);
			kdt->nnearest(idx, &knn[1], &dn[1], k-1);
			knn[0] = idx;	//itself!!!! 자신을 포함!!


			// compute centroid


				
			for(int i = 0; i < k; ++i){
				centroid[idx].x[0] = centroid[idx].x[0] + kdt->ptss[ knn[i]].x[0];
				centroid[idx].x[1] = centroid[idx].x[1] + kdt->ptss[ knn[i]].x[1];
				centroid[idx].x[2] = centroid[idx].x[2] + kdt->ptss[ knn[i]].x[2];
			}
			centroid[idx].x[0] = centroid[idx].x[0]/k;
			centroid[idx].x[1] = centroid[idx].x[1]/k;
			centroid[idx].x[2] = centroid[idx].x[2]/k;


			//****************COVARIANCE MATRIX CONSTRUCTION*****************//
			float sum_w = 0.f;
			for(int n = 0; n < k; ++n){
				Point3 p =  kdt->ptss[ knn[n] ];
				float dist = (p.x[0] - centroid[idx].x[0])*(p.x[0] - centroid[idx].x[0]) + (p.x[1] - centroid[idx].x[1])*(p.x[1] - centroid[idx].x[1]) + (p.x[2] - centroid[idx].x[2])*(p.x[2] - centroid[idx].x[2]);
				float w = exp(-dist/0.1);
				sum_w += w;
				for(int i = 0; i < 3; ++i){
					for(int j = 0; j < 3; ++j){
						matC[i][j] += w * (p.x[i] - centroid[idx].x[i]) * (p.x[j] - centroid[idx].x[j]);
					}
				}
			}
			for(int i = 0; i < 3; ++i){
				for(int j = 0; j < 3; ++j){
					matC[i][j] /= ((k-1)*sum_w);
				}
			}

			//****************Principal Component Analysis*****************//


			EigenSolver A;
			A.SolveEigenSystem(3, matC);
			A.SortEigenvectors();

			
			ev[idx] = OpenMesh::Vec3f(A.eigenValue_[0], A.eigenValue_[1], A.eigenValue_[2]);
			normal[idx] = OpenMesh::Vec3f(A.matV_[0][2], A.matV_[1][2], A.matV_[2][2]);


		}
		delete[] matC;
		delete[] knn;
		delete[] dn;

	}

	/**************************************************************************
	computer normal vectors of the vertices using PCA with k neighbor points 
    eigenvalue, normal of each vertex are calculated.
	***************************************************************************/
	void compute_normal(int k, int*knn){
	
	//	KDtree<3> kdt(point_set);
		
		// find k-nearest neighbor using kd-tree
		knn = new Int[k];

		Doub *dn = new Doub[k];
		double** matC;			// Covariance matrix
		matC = new double* [3];
		// Initialize matC
		for(int i=0; i<3; i++) {
			matC[i] = new double [3];
		}
		for(int i=0; i<3; i++) {
			for(int j=0; j<3; j++) {
				matC[i][j] = 0;
			}
		}

		for(int idx = 0; idx < nPts; ++idx){

			Point3 p = point_set[idx];
			centroid[idx].x[0]=centroid[idx].x[1]=centroid[idx].x[2]=0.0;
			//								fixed point, range, knn, nMax
			//Int nret = kdt.locatenear(Point3(p[0],p[1],p[2]), 3, knn, 4);
			kdt->nnearest(idx, &knn[1], &dn[1], k-1);
			knn[0] = idx;	//itself!!!! 자신을 포함!!


			// compute centroid


				
			for(int i = 0; i < k; ++i){
				centroid[idx].x[0] = centroid[idx].x[0] + kdt->ptss[ knn[i]].x[0];
				centroid[idx].x[1] = centroid[idx].x[1] + kdt->ptss[ knn[i]].x[1];
				centroid[idx].x[2] = centroid[idx].x[2] + kdt->ptss[ knn[i]].x[2];
			}
			centroid[idx].x[0] = centroid[idx].x[0]/k;
			centroid[idx].x[1] = centroid[idx].x[1]/k;
			centroid[idx].x[2] = centroid[idx].x[2]/k;


			//****************COVARIANCE MATRIX CONSTRUCTION*****************//
			float sum_w = 0.f;
			for(int n = 0; n < k; ++n){
				Point3 p =  kdt->ptss[ knn[n] ];
				float dist = (p.x[0] - centroid[idx].x[0])*(p.x[0] - centroid[idx].x[0]) + (p.x[1] - centroid[idx].x[1])*(p.x[1] - centroid[idx].x[1]) + (p.x[2] - centroid[idx].x[2])*(p.x[2] - centroid[idx].x[2]);
				float w = exp(-dist/0.1);
				sum_w += w;
				for(int i = 0; i < 3; ++i){
					for(int j = 0; j < 3; ++j){
						matC[i][j] += w * (p.x[i] - centroid[idx].x[i]) * (p.x[j] - centroid[idx].x[j]);
					}
				}
			}
			for(int i = 0; i < 3; ++i){
				for(int j = 0; j < 3; ++j){
					matC[i][j] /= ((k-1)*sum_w);
				}
			}

			//****************Principal Component Analysis*****************//


			EigenSolver A;
			A.SolveEigenSystem(3, matC);
			A.SortEigenvectors();

			
			ev[idx] = OpenMesh::Vec3f(A.eigenValue_[0], A.eigenValue_[1], A.eigenValue_[2]);
			normal[idx] = OpenMesh::Vec3f(A.matV_[0][2], A.matV_[1][2], A.matV_[2][2]);


		}
		delete[] matC;
		delete[] knn;
		delete[] dn;

	}

	/**************************************************************************
	find the k-nearest neighborhood (except p) of p using kd-tree. 
	this function set *knn to the indices of k nearest neighborhood
	***************************************************************************/
	void k_nearest_neighbor(int p, int k, int *knn) {
		
		int *knn_ = new int [k];
		Doub *dn = new Doub[k];
		kdt->nnearest(p, knn_, dn, k);
		// sorting in increasing order
		for(int i=0; i<k-1; ++i){
			for(int j=i+1; j<k; ++j){
				if( kdt->disti(p, knn_[i]) > kdt->disti(p, knn_[j]) ){
					Int tmp = knn_[i];
					knn_[i] = knn_[j];
					knn_[j] = tmp;
				}
			}
		}
		for(int i=0; i<k; ++i){
			knn[i] = knn_[i];
		}
		
		delete [] knn_;
		delete [] dn;
	}

	/**************************************************************************
	 find the k-nearest neighborhood of (except p) p using kd-tree.
	 this function set *knn to the indices of all indices within the search range. 
	 Return the number of neighborhood.
	**************************************************************************/
	int k_nearest_neighbor(int p, double range, int nMax, int *knn) {
	
		int *knn_ = new int [200];
		int nn = (int)kdt->locatenear(point_set[p], range, knn_, 200);
		// sorting in increasing order
		for(int i=0; i<nn-1; ++i){
			for(int j=i+1; j<nn; ++j){
				if( kdt->disti(p, knn_[i]) > kdt->disti(p, knn_[j]) ){
					Int tmp = knn_[i];
					knn_[i] = knn_[j];
					knn_[j] = tmp;

				}
			}
		}
		for(int i=0; i<nMax; ++i){
			knn[i] = knn_[i];
		}
		// p is in the last index
		delete [] knn_;
		return min(nn-1, nMax);
	}
	/**************************************************************************
	find the k-nearest neighborhood of an arbitrary point p using kd-tree.
	this function set *knn to the indices of all indices within the search range.
	Return the number of neighborhood
	**************************************************************************/
	int k_nearest_neighbor(Point3 p, double range, int nMax, int *knn) {
		return (int)kdt->locatenear(p, range, knn, nMax);
	}
	int k_nearest_neighbor(OpenMesh::Vec3f p, double range, int nMax, int *knn) {
		return (int)kdt->locatenear(Point3(p[0], p[1], p[2]), range, knn, nMax);
	}



	/**************************************************************************
	find the k-nearest Delaunay neighborhood of an arbitrary point p using kd-tree.
	this function returns the number of neighborhood topologically.
	**************************************************************************/
	


	//*************************************************************************
	// easier access to the eigenvalues of a vertex idx
	// ev[0] >= ev[1] >= ev[2]
	OpenMesh::Vec3f& eigenValue(int idx)
	{	return ev[idx];		}
	// easier access to the normal vector of a vertex idx
	OpenMesh::Vec3f& normalVector(int idx)
	{	return normal[idx];		}



	/**************************************************************************
	compute the average length between the adjacent point 
	***************************************************************************/
	void compute_avgLen(){
		int knn;
		avgLen = 0.f;
		for(int idx = 0; idx < nPts; ++idx){
			k_nearest_neighbor(idx, 1, &knn);
			avgLen += kdt->disti(idx, knn);
		}
		avgLen /= nPts;
	}





};
} // namespace PBM

#endif
