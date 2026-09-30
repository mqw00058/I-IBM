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
//   $Date: 2006.12.05 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file ColorConversionT.hh
    
 */

// http://www.brucelindbloom.com/ (Reference)

//=============================================================================
//
//  CLASS ColorConversionT
//
//=============================================================================

#ifndef OPENMESH_ColorConversion_ColorConversionT_HH
#define OPENMESH_ColorConversion_ColorConversionT_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace ColorConversion {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class ColorConversionT
{
public:

	// Type Definition
	typedef typename Mesh::VertexHandle				VertexHandle;
	typedef VectorT<double, 3>						Vec3; 
	typedef typename Mesh::Color					Color;

public:

	// constructor & destructor
	ColorConversionT( Mesh& _mesh );
	virtual ~ColorConversionT();


public:
	// Operations
	virtual void initialize();					// Initialize
	virtual void RGB2XYZ();						// Convert RGB to XYZ for all vertices
	virtual void RGB2XYZ(VertexHandle _vh);		// Convert RGB to XYZ at a vertex
	virtual void XYZ2Lab();						// Convert XYZ to Lab for all vertices
	virtual void XYZ2Lab(VertexHandle _vh);		// Convert XYZ to Lab at a vertex
	virtual void Lab2XYZ();						// Convert Lab to XYZ for all vertices
	virtual void Lab2XYZ(VertexHandle _vh);		// Convert Lab to XYZ at a vertex
	virtual void XYZ2RGB();						// Convert XYZ to RGB for all vertices
	virtual void XYZ2RGB(VertexHandle _vh);		// Convert XYZ to RGB at a vertex

	virtual void Set_Lab(VertexHandle _vh, Vec3 _lab);		// Set the lab value of a vertex

	virtual double ColorDiff(VertexHandle _vh1, VertexHandle _vh2);	// Calculate Color difference between _vh1 and _vh2
																		  
	// easier access to the color components						  
	Vec3& XYZ(VertexHandle _vh) 
	{ return mesh_.property(xyz_, _vh); }
	Vec3& Lab(VertexHandle _vh) 
	{ return mesh_.property(lab_, _vh); }
	Color& NewRGB(VertexHandle _vh)
	{ return mesh_.property(RGB_, _vh); }
	
private:
	// Member variable
	OpenMesh::VPropHandleT<Vec3>	xyz_;
	OpenMesh::VPropHandleT<Vec3>	lab_;
	OpenMesh::VPropHandleT<Color>	RGB_;


protected:

	Mesh&  mesh_;



};


//=============================================================================
} // namespace ColorConversion
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_ColorConversionT_CC)
#define OPENMESH_ColorConversionT_TEMPLATES
#include "ColorConversionT.cc"
#endif
//=============================================================================
#endif // OPENMESH_ColorConversion_ColorConversionT_HH defined
//=============================================================================

