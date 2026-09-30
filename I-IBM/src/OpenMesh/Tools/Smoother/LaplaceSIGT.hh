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
//   $Revision: 2006.10.` $
//   $Date: 2006/01/18 10:53:00 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file LaplaceSIGT.hh
    
 */

//=============================================================================
//
//  CLASS LaplaceSIGT
//
//=============================================================================

#ifndef OPENMESH_LAPLACESIG_LAPLACESIGT_HH
#define OPENMESH_LAPLACESIG_LAPLACESIGT_HH


//== INCLUDES =================================================================

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Smoother {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class LaplaceSIGT
{
public:

	typedef typename Mesh::Point         Point;
	typedef typename Mesh::Scalar        Scalar;
	typedef typename Mesh::VertexHandle  VertexHandle;
	typedef typename Mesh::EdgeHandle    EdgeHandle;

public:

	// constructor & destructor
	LaplaceSIGT( Mesh& _mesh );
	virtual ~LaplaceSIGT();

	/// iterative Laplacian smoothing
	void smooth(unsigned int _iters);


public:
	// Operations
	virtual void initialize();					// Initialize

	/// calculate vertex and edge weights
	void calc_weights();

	// easier access to new vertex positions
	Point& new_pos(VertexHandle _vh) 
	{ return mesh_.property(vpos_, _vh); }
	
	// easier access to vertex weights
	Scalar& weight(VertexHandle _vh) 
	{ return mesh_.property(vweight_, _vh); }
	
	// easier access to edge weights
	Scalar& weight(EdgeHandle _eh) 
	{ return mesh_.property(eweight_, _eh); }
	
private:
	// Member variable


protected:

	Mesh&  mesh_;
	OpenMesh::VPropHandleT<Point>   vpos_;
	OpenMesh::VPropHandleT<Scalar>  vweight_;
	OpenMesh::EPropHandleT<Scalar>  eweight_;
};


//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_LAPLACESIGT_CC)
#define OPENMESH_LAPLACESIGT_TEMPLATES
#include "LaplaceSIGT.cc"
#endif
//=============================================================================
#endif // OPENMESH_LAPLACESIG_LAPLACESIGT_HH defined
//=============================================================================

