//=============================================================================
// Multi-Scale Feature-Aware Filtering
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2013.01.25 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//=============================================================================

#pragma once


//=============================================================================
//
//  CLASS MultiScaleJBF
//
//=============================================================================
#ifndef PBM_FILTER_MSJBF_HH
#define PBM_FILTER_MSJBF_HH



//== INCLUDES =================================================================
#include <pbm/Common/NormalEstimation.h>

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>
//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================
//typedef Point<3> Point3;

using namespace OpenMesh;
typedef PBM::NormalEstimation	NE;

namespace PBM{
namespace Filter{


//== CLASS DEFINITION =========================================================
template <class Mesh>
class MultiScaleJBFT
{
public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::Point					Point;

public:

	// constructor & destructor
	MultiScaleJBFT( Mesh& _mesh );
	virtual ~MultiScaleJBFT();

	void initialize();
	void CurvatureFlow(float * k_i);
	void BalancedFlowFiltering(int K/*parameter to contronl the diffusion speed*/,  int N/* num of iteration*/, float s/*step length*/);
	void Gauss(vector<Vec3f> &pts);
	void color_coding();






private:

	// member variables
	OpenMesh::VPropHandleT<Vec3f>	e1_;
	OpenMesh::VPropHandleT<Vec3f>	e2_;
	OpenMesh::VPropHandleT<Vec3f>	e3_;
	OpenMesh::VPropHandleT<Vec3f>	ev_;

	OpenMesh::VPropHandleT<float>	fweight_;
	OpenMesh::VPropHandleT<float>	min_fweight_;
	OpenMesh::VPropHandleT<float>	density_;

	OpenMesh::VPropHandleT<int>		scale_;
	OpenMesh::VPropHandleT<int>		nfeature_;
	OpenMesh::VPropHandleT<bool>	feature_candidate_;


	int *knn;		// k-nearest neighbor index;
	int *nn;		// the number of neighbor
	int nMax;



protected:
	Mesh& mesh_;
	vector<Vec3f>	point_set;	// point list (data structure)
	NE *ne;
	
	int nPts;
};
}
}
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_FILTER_MSJBF_CC)
#define OPENMESH_MultiScaleJBF_TEMPLATES
#include "MultiScaleJBF.cc"
#endif
//=============================================================================
#endif // OPENMESH_FILTER_MultiScaleJBF_HH defined
//=============================================================================

