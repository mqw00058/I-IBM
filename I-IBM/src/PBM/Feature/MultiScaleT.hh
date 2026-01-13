//=============================================================================
//                                                                            
//                               OpenMesh                                     
//        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen          
//                           www.openmesh.org                                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This library is free software; you can redistribute it and/or modify it 
//   under the terms of the GNU Library General Public License as published  
//   by the Free Software Foundation, version 2.                             
//                                                                             
//   This library is distributed in the hope that it will be useful, but       
//   WITHOUT ANY WARRANTY; without even the implied warranty of                
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
//   Library General Public License for more details.                          
//                                                                            
//   You should have received a copy of the GNU Library General Public         
//   License along with this library; if not, write to the Free Software       
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2011.03.21 $
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

#ifndef PBM_FEATURE_MULTISCALET_HH
#define PBM_FEATURE_MULTISCALET_HH


//== INCLUDES =================================================================
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <PBM/Common/NormalEstimation.h>

using namespace OpenMesh;

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================


namespace PBM {
namespace Feature {

//== CLASS DEFINITION =========================================================
template <class Mesh>
class MultiScaleT
{
	/**
	Scale-space representation
		
	*common notation
	point cloud: point_set
	scale = # of neighbors: k
	ev: eigenvalues
	*/
public:

	// constructor & destructor
	MultiScaleT( Mesh& _mesh );
	virtual ~MultiScaleT();

	void initialize();
	void extract_features_surfvar();

	/* compute the multiscale surface variation [Pauly03] at every scale 
	   we're interested in
		

	*/

	/* compute the surface variation [Pauly03] in a scale-space */
	void compute_multiscale_surfvar(int k_min, int k_max);

	/* compute the curvedness [Ho09] in a scale-space */
	void compute_multiscale_curvedeness(int k_min, int k_max);

	/* compute the maximal residual of MLS projection in a scale-space */
	//void compute_multiscale_residual(int k_min, int k_max);

	/* compute the multi-scale measures */
	void compute_multiscale_measure(int mode, int k_min, int k_max){

		min_scale = k_min;
		max_scale = k_max;

		switch (mode)
		{
		case 0: 
			compute_multiscale_surfvar(k_min, k_max);
			extract_features_surfvar();
			break;
		case 1: compute_multiscale_curvedeness(k_min, k_max);	break;
//		case 2: compute_multiscale_residual(k_min, k_max);		break;
		}
		
	}

	/* visualization */
	void color_coding(int k);
	/* multi-scale feature color coding */
	void feature_color_coding();

	// the minimum number of scale
	// At zero scale, PCA fails to compute the eigenvalues
	int min_scale;
	int max_scale;
	

private:

	
	
	//// easier access to surface variation
	float& surface_variation(VertexHandle _vh, int k) 
	{ return mesh_.property(surface_variation_, _vh)[k-min_scale]; }

	//// easier access to curvedness
	float& curvedness(VertexHandle _vh, int k) 
	{ return mesh_.property(curvedness_, _vh)[k-min_scale]; }

	//// easier access to multi-scale measures
	float& multiscale_measure(VertexHandle _vh, int k) 
	{ return mesh_.property(multiscale_measure_, _vh)[k-min_scale]; }

	//// easier access to residual
	float& residual(VertexHandle _vh, int k) 
	{ return mesh_.property(residual_, _vh)[k-min_scale]; }


	int n_features;
	int numNeighbor;
	float radius;

	// multi-scale measures
	OpenMesh::VPropHandleT<vector<float>>	surface_variation_;
	OpenMesh::VPropHandleT<vector<float>>	curvedness_;
	OpenMesh::VPropHandleT<vector<float>>	residual_;
	OpenMesh::VPropHandleT<vector<float>>	multiscale_measure_;


	vector<Vec3f> point_set;
	NormalEstimation	*ne;



protected:
	Mesh&  mesh_;


};


//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_MULTISCALET_CC)
#define PBM_MULTISCALET_TEMPLATES
#include "MultiScaleT.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_MultiScaleT_HH defined
//=============================================================================

