//=============================================================================
// Mesh saliency
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2012.09.20 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//   $Referecend by [Lee2005]                                                                           
//=============================================================================

/** \file MultiScaleTT.hh

 */


//=============================================================================
//
//  CLASS MultiScaleT
//
//=============================================================================
#ifndef OPENMESH_PROPERTY_SALIENCY_HH
#define OPENMESH_PROPERTY_SALIENCY_HH



//== INCLUDES =================================================================
#include <src/PBM/Common/NormalEstimation.h>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>




//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================
//typedef Point<3> Point3;

using namespace OpenMesh;


namespace OpenMesh {
namespace Property {

//== CLASS DEFINITION =========================================================
template <class Mesh>
class SaliencyT
{
public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;

public:

	// constructor & destructor
	SaliencyT( Mesh& _mesh );
	virtual ~SaliencyT();

	void initialize();


	void compute_saliency(float minscale, float maxscale);

	/* visualization */
	void color_coding();

	// the minimum number of scale
	// At zero scale, PCA fails to compute the eigenvalues

	float epsilon; // 0.3% of the length of the diagonal of the bounding box of the model

private:


	//// easier access to surface variation
	float meancurvature(VertexHandle _vh) 
	{ return 	mesh_.property(Kh_normal_, _vh).norm() * 0.5f; }

	
	int n_features;
	int numNeighbor;
	float radius;

	// multi-scale measures
	OpenMesh::VPropHandleT<Vec3f>	Kh_normal_;		//mean curvature normal
	OpenMesh::VPropHandleT<Scalar>	Kg_;			//Gaussian Saliency
	OpenMesh::VPropHandleT<Scalar>	Kmax_;			//max Principal Saliency
	OpenMesh::VPropHandleT<Scalar>	Kmin_;			//min Principal Saliency

	OpenMesh::VPropHandleT<Scalar>	saliency_;		//vertex saliency value



protected:
	
	Mesh& mesh_;
	vector<Vec3f>	point_set;	// point list (data structure)
	NormalEstimation *ne;
	


};


//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_PROPERTY_SALIENCY_CC)
#define OPENMESH_PROPERTY_SALIENCY_HH
#include "saliency.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_MultiScaleT_HH defined
//=============================================================================

