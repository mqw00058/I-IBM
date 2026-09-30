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

/** \file ColorOpsT.hh
    
 */

//=============================================================================
//
//  CLASS ColorOpsT
//
//=============================================================================

#ifndef OPENMESH_COLOROPS_COLOROPST_HH
#define OPENMESH_COLOROPS_COLOROPST_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>
#include <OpenMesh/Tools/Color/ColorConversionT.hh>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace ColorOps {

using namespace OpenMesh::ColorConversion;

//== CLASS DEFINITION =========================================================

/** Base class for vertex color operations.
 */	      
template <class Mesh>
class ColorOpsT
{
public:

	// Type Definition
	typedef typename Mesh::Color        Color;
	typedef typename OpenMesh::ColorConversion::ColorConversionT<Mesh>		ColorConversion;
	typedef VectorT<double, 3>			Vec3; 
  

public:

	// constructor & destructor
	ColorOpsT( Mesh& _mesh );
	virtual ~ColorOpsT();


public:
	// Color Operations
	virtual void initialize();					// Initialize Color Operations
	
	virtual void SplitChannelR();				// Split R-channel
	virtual void SplitChannelG();				// Split G-channel
	virtual void SplitChannelB();				// Split B-channel

	virtual void RGB_To_Gray();					// Convert Gray scale by RGB
	virtual void R_To_Gray();					// Convert Gray scale by R
	virtual void G_To_Gray();					// Convert Gray scale by G
	virtual void B_To_Gray();					// Convert Gray scale by B

	virtual void UpdateColor();					// Set the new color

	virtual void Soften();						// Soften Operation
	virtual void Sharpen();						// Sharpen Operation
	virtual void Emboss();						// Emboss Operation
	virtual void EdgeDetect();					// Edge Detect Operation
	virtual void Inverse();						// Inverse Color
	virtual void Brightness(int _brightness);	// Brightness control
	virtual void Contrast(int _contrast);		// Contrast control

	virtual void Bilateral(double _closeness, double _similarity);	// Bilateral filtering in CIE-Lab space
	virtual void BilateralRGB(double _closeness, double _similarity);	// Bilateral filtering in RGB space

	virtual void color_coding();

private:

	OpenMesh::VPropHandleT<Color>      new_color_;


protected:

	Mesh&  mesh_;



};


//=============================================================================
} // namespace ColorOps
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_COLOROPST_C)
#define OPENMESH_COLOROPST_TEMPLATES
#include "ColorOpsT.cc"
#endif
//=============================================================================
#endif // OPENMESH_COLOROPS_COLOROPST_HH defined
//=============================================================================

