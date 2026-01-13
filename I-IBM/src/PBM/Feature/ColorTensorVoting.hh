#pragma once

#ifndef PBM_FEATURE_COLORTENSORVOTING_HH
#define PBM_FEATURE_COLORTENSORVOTING_HH
//== INCLUDES =================================================================

#include <OpenMesh/Core/Utils/Property.hh>
#include <PBM/Common/NormalEstimation.h>
#include<vector>


using namespace OpenMesh;
//== FORWARDDECLARATIONS ======================================================
typedef Vec3uc color;
typedef Vec3f point;

//== NAMESPACES ===============================================================
namespace PBM{
namespace Feature{
//== CLASS DEFINITION =========================================================
template <class Mesh>
class CColorTensorVotingT
{
public:
	CColorTensorVotingT(Mesh &_mesh);
	~CColorTensorVotingT(void);

public:
	void initialize();
	void tensorvoting();
	void color_coding();

	// member variable
	int nPts;			// the number of poits
	

private:
	Mesh&  mesh_;				// OpenMesh data structure
	vector<color> color_set;	// color list
	vector<point> point_set;
	PBM::NormalEstimation *ne;

	color var;					// color variance of each channel

	OpenMesh::VPropHandleT<float>	fweight_;
	

};
//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_FEATURE_COLORTENSORVOTING_CC)
#define PBM_COLORTENSORVOTING_TEMPLATES
#include "ColorTensorVoting.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_TensorVotingT_HH defined
//=============================================================================
