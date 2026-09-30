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
//   $Date: 2011.02.22 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//	 $Modified by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file AddNoiseT.hh
    
 */

//=============================================================================
//
//  CLASS AddNoiseT
//
//=============================================================================

#ifndef OPENMESH_ADDNOISE_ADDNOISET_HH
#define OPENMESH_ADDNOISE_ADDNOISET_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace Noise {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class AddNoiseT
{
public:

	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::VertexHandle				VertexHandle;
	typedef typename Mesh::EdgeHandle				EdgeHandle;
	typedef VectorT<Scalar, 3>						Vec3; 

public:

	// constructor & destructor
	AddNoiseT( Mesh& _mesh );
	virtual ~AddNoiseT();

	// data: position or color
	enum Data {GEOMETRY=0, COLOR} m_nData; // 화면조작 모드


public:
	// Operations
	virtual void initialize();						// Initialize

	// Noise type_ 0:uniform, 1: Gaussian, 
	virtual void apply_geometric_noise(int type_, float ratio_, float factor_);	// Add Noise
	// Noise type_ 0:uniform, 1: Gaussian, 
	virtual void apply_appearance_noise(int type_, float ratio_, Vec3f var);	// Add Noise

		
private:
	// Member variable
	float noise();

protected:

	Mesh&  mesh_;



};


//=============================================================================
} // namespace Feature
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_ADDNOISET_CC)
#define OPENMESH_ADDNOISET_TEMPLATES
#include "AddNoiseT.cc"
#endif
//=============================================================================
#endif // OPENMESH_ADDNOISE_ADDNOISET_HH defined
//=============================================================================

