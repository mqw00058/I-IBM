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
//   $Revision: 2006.7.5 $
//   $Date: 2006/01/18 10:53:00 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file CurvatureT.hh
    
 */

//=============================================================================
//
//  CLASS CurvatureT
//
//=============================================================================

#ifndef OPENMESH_CURVATURE_CURVATURET_HH
#define OPENMESH_CURVATURE_CURVATURET_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>


#include <math.h>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Curvature {

//== CLASS DEFINITION =========================================================

/** Base class for vertex color operations.
 */	      
template <class Mesh>
class CurvatureT
{
public:

	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::VertexHandle				VertexHandle;
	typedef VectorT<Scalar, 3>						Vec3;  

public:

	// constructor & destructor
	CurvatureT( Mesh& _mesh );
	virtual ~CurvatureT();


public:
	// Color Operations
	virtual void initialize();					// Initialize Color Operations
	
	virtual void CalcGaussCurvature1();			// Calculate Gaussian curvature based on Rodregues method
	virtual void CalcMeanCurvature1();			// Calculate mean curvature based on Zorin method
	virtual void CalcMeanCurvature2();			// Calculate mean curvature based on Steiner method
	virtual void FindMinMaxCurv();				// Find Maximum and Minimum Curvature value from the result
	virtual void detect_feature(unsigned int percent_);		// Set a vertex below the percentage into a feature point 

	const Scalar& curvature(VertexHandle _vh) const			// return curvature value
	{ return mesh_.property(curvature_, _vh); }
	const Scalar& max_curvature() const						// return maximum curvature value
	{ return max_curvature_; }
	const Scalar& min_curvature() const						// return minimum curvature value
	{ return min_curvature_; }

	unsigned int n_feature()											// return number of feature points
	{ return n_feature_; }

private:

	OpenMesh::VPropHandleT<Scalar>      curvature_;
	OpenMesh::VPropHandleT<Scalar>      bary_area_;
	Scalar								max_curvature_;
	Scalar								min_curvature_;

	unsigned int n_feature_;								// number of feature

////////////////////////////////////////////////////////////////////////////
// SIGGRAPH 2006 Course Note
public:	
	/// calculate vertex and edge weights
	void calc_weights();

	/// calculate curvature per vertex
	void calc_curvature();

	/// set vertex color from vertex curvature
	void color_coding();


	// easier access to vertex weights
	Scalar& weight(VertexHandle _vh) 
	{ return mesh_.property(vweight_, _vh); }

	// easier access to vertex curvature
	Scalar& curvature(VertexHandle _vh) 
	{ return mesh_.property(curvature_, _vh); }

	// easier access to edge weights
	Scalar& weight(EdgeHandle _eh) 
	{ return mesh_.property(eweight_, _eh); }

private:
	OpenMesh::VPropHandleT<Scalar>  vweight_;
	OpenMesh::EPropHandleT<Scalar>  eweight_;
////////////////////////////////////////////////////////////////////////////

protected:

	Mesh&  mesh_;
};


//=============================================================================
} // namespace Curvature
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_CURVATURET_C)
#define OPENMESH_CURVATURET_TEMPLATES
#include "CurvatureT.cc"
#endif
//=============================================================================
#endif // OPENMESH_CURVATURE_CURVATURET_HH defined
//=============================================================================

