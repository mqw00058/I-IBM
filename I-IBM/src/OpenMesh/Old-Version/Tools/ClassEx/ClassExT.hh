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

/** \file ClassExT.hh
    
 */

//=============================================================================
//
//  CLASS ClassExT
//
//=============================================================================

#ifndef OPENMESH_ClassEx_ClassExT_HH
#define OPENMESH_ClassEx_ClassExT_HH


//== INCLUDES =================================================================

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace ClassEx {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class ClassExT
{
public:

	// Type Definition

public:

	// constructor & destructor
	ClassExT( Mesh& _mesh );
	virtual ~ClassExT();


public:
	// Operations
	virtual void initialize();					// Initialize
	
private:
	// Member variable


protected:

	Mesh&  mesh_;



};


//=============================================================================
} // namespace ClassEx
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_ClassExT_CC)
#define OPENMESH_ClassExT_TEMPLATES
#include "ClassExT.cc"
#endif
//=============================================================================
#endif // OPENMESH_ClassEx_ClassExT_HH defined
//=============================================================================

