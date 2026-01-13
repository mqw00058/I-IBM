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
//   $Date: 2011.02.28 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file NormalClusteringTT.hh

 */

//=============================================================================
//
//  CLASS NormalClusteringT
//
//=============================================================================

#ifndef PBM_FEATURE_NormalClusteringT_HH
#define PBM_FEATURE_NormalClusteringT_HH


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
class NormalClusteringT
{
public:
	// Type Definition
public:

	// constructor & destructor
	NormalClusteringT( Mesh& _mesh );
	virtual ~NormalClusteringT();


public:
	// Operations
	virtual void apply(int k, double range, double sens_params);				// 상황에 따라 parameter가 필요할 수도 있음

	unsigned int n_featurepoints()
	{	return n_features;	}

		
private:
	
	// Member variable
	int		k;		// # of neighbors
	double	r;		// neighborhood range (search range)
	double sens_params;	// sensitivity parameter
	int		n_features;

	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	normal_;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	eigenvalue_;
	OpenMesh::VPropHandleT<float>	normal_variation_;


	// Member function
	void initialize();			
	void normal_estimation();
	void GaussmapClustering();
	void color_coding();


	NormalEstimation	*ne;
	vector<OpenMesh::Vec3f> point_set;


protected:
	Mesh&  mesh_;
	

};


//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_NormalClusteringT_CC)
#define PBM_NormalClusteringT_TEMPLATES
#include "NormalClusteringT.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_NormalClusteringT_HH defined
//=============================================================================

