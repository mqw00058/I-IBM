//=============================================================================
// Curvature estimation of Point-set surface
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2012.08.30 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file MultiScaleTT.hh

 */

//=============================================================================
//
//  CLASS MultiScaleT
//
//=============================================================================

#ifndef PBM_PROPERTY_CURVATURE_HH
#define PBM_PROPERTY_CURVATURE_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <PBM/Common/NormalEstimation.h>

using namespace OpenMesh;

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================


namespace PBM {
namespace Property {

//== CLASS DEFINITION =========================================================
template <class Mesh>
class Curvature
{

public:

	// constructor & destructor
	Curvature( Mesh& _mesh );
	virtual ~Curvature();

	void initialize();



	/* visualization */
	void color_coding(int k);

	// the minimum number of scale
	// At zero scale, PCA fails to compute the eigenvalues
	int min_scale;
	int max_scale;
	

private:

	
	
	//// easier access to surface variation
	float& meancurvature(VertexHandle _vh) 
	{ return mesh_.property(Kh_normal_, _vh); }

	
	int n_features;
	int numNeighbor;
	float radius;

	// multi-scale measures
	OpenMesh::VPropHandleT<Vec3f>	Kh_normal_;		//mean curvature normal
	//OpenMesh::VPropHandleT<Scalar>	Kg_;			//Gaussian curvature
	//OpenMesh::VPropHandleT<Scalar>	Kmax_;			//max Principal curvature
	//OpenMesh::VPropHandleT<Scalar>	Kmin_;			//min Principal curvature
	//OpenMesh::VPropHandleT<Scalar>	Area_;			//Voronoi area each vertex


	NormalEstimation	*ne;



protected:
	Mesh&  mesh_;


};


//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_PROPERTY_CURVATURE_CC)
#define PBM_PROPERTY_CURVATURE_HH
#include "curvature.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_MultiScaleT_HH defined
//=============================================================================

