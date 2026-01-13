//=============================================================================
// Mesh saliency
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2012.12.18 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//   $Referecend by Mesh Bilateral Filter                                                                           
//=============================================================================

#pragma once


//=============================================================================
//
//  CLASS BilateralFilterT
//
//=============================================================================
#ifndef OPENMESH_FILTER_BILATERALFILTER_HH
#define OPENMESH_FILTER_BILATERALFILTER_HH



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

namespace OpenMesh {
namespace Filter {

//== CLASS DEFINITION =========================================================
template <class Mesh>
class BilateralFilterT
{
public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::Point					Point;

public:

	// constructor & destructor
	BilateralFilterT( Mesh& _mesh );
	virtual ~BilateralFilterT();

	enum FilterMode {JONES=1, FLEISHMAN} mode;
	
	void apply(int iter, float sigma1, float sigma2);


private:

	void initialize();

	void Fleishman_bFilter(int iter);
	void Fleishman_filter(VertexHandle vh);

	void Jones_bFilter();
	void Jones_filter(VertexHandle v_it, vector<unsigned> &flags, unsigned &flag_curr, 
		float invsigma2_1, float invsigma2_2, bool pass1, vector<Point> &mpoints);

	inline float wt(Vec3f p1, Vec3f p2, float invsigma2);
	

protected:
	Mesh& mesh_;
	vector<Vec3f>	point_set;	// point list (data structure)



	NE *ne;

	int iter;
	float sigma1;
	float sigma2;
	float truncate;




};


//=============================================================================
} // namespace Filter
} // namespace BilaterFilter
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_FILTER_BILATERALFILTER_CC)
#define OPENMESH_FILTER_BILATERALFILTER_HH
#include "BilateralFilter.cc"
#endif
//=============================================================================
#endif // OPENMESH_FILTER_BILATERALFILTER_HH defined
//=============================================================================

