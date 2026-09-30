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

/** \file MultiScaleT.cc

*/

//=============================================================================
//
//  CLASS MultiScaleT - IMPLEMENTATION
//
//=============================================================================

#ifndef PBM_PROPERTY_CURVATURE_CC
#define PBM_PROPERTY_CURVATURE_CC

//== INCLUDES =================================================================
#include "curvature.hh"

//== NAMESPACES ===============================================================


namespace PBM {
namespace Property {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
Curvature<Mesh>::
	Curvature(Mesh& _mesh)
	: mesh_(_mesh)
{
	mesh_.add_property(Kh_normal_);
	initialize();
}

//-----------------------------------------------------------------------------


template <class Mesh>
Curvature<Mesh>::
	~Curvature()
{
	mesh_.remove_property(Kh_normal_);	
}





//-----------------------------------------------------------------------------
template <class Mesh>
void Curvature<Mesh>::initialize()
{


}




//-----------------------------------------------------------------------------
template <class Mesh>
void Curvature<Mesh>::color_coding(int k)
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;	
}

//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif