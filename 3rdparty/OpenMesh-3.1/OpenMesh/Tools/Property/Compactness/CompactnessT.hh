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
//   $Revision: 2006.11.17 $
//   $Date: 2006/11/17 10:53:00 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file CompactnessT.hh
    
 */

//=============================================================================
//
//  CLASS CompactnessT
//
//=============================================================================

#ifndef OPENMESH_COMPACTNESS_COMPACTNESST_HH
#define OPENMESH_COMPACTNESS_COMPACTNESST_HH


//== INCLUDES =================================================================

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Compactness {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class CompactnessT
{
public:

	// Type Definition
	typedef typename Mesh::Scalar					Scalar;

public:

	// constructor & destructor
	CompactnessT( Mesh& _mesh );
	virtual ~CompactnessT();


public:
	// Operations
	virtual void initialize();					// Initialize
	const Scalar& compactness(FaceHandle _fh) const			// return curvature value
	{ return mesh_.property(compactness_, _fh); }
	const Scalar& max_compactness() const						// return maximum curvature value
	{ return max_compactness_; }
	const Scalar& min_compactness() const						// return minimum curvature value
	{ return min_compactness_; }
	const Scalar& ave_compactness() const						// return minimum curvature value
	{ return ave_compactness_; }
	
private:
	// Member variable


protected:

	Mesh&  mesh_;

	OpenMesh::FPropHandleT<Scalar>      compactness_;
	Scalar								max_compactness_;
	Scalar								min_compactness_;
	Scalar								ave_compactness_;

};


//=============================================================================
} // namespace Compactness
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_COMPACTNESST_CC)
#define OPENMESH_COMPACTNESST_TEMPLATES
#include "CompactnessT.cc"
#endif
//=============================================================================
#endif // OPENMESH_Compactness_CompactnessT_HH defined
//=============================================================================

