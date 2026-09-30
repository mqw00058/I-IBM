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
//   $Date: 2007.08.16 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file DiscreteCurvatureT.hh
    
 */

//=============================================================================
//
//  CLASS DiscreteCurvatureT
//
//=============================================================================

#ifndef OPENMESH_DISCRETECURVATURE_DISCRETECURVATURET_HH
#define OPENMESH_DISCRETECURVATURE_DISCRETECURVATURET_HH


//== INCLUDES =================================================================

#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>

#include <math.h>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace OpenMesh {
namespace DiscreteCurvature {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class DiscreteCurvatureT
{
public:

	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::VertexHandle				VertexHandle;
	typedef VectorT<Scalar, 3>						Vec3;  


public:

	// constructor & destructor
	DiscreteCurvatureT(Mesh& _mesh);
	virtual ~DiscreteCurvatureT();


public:
	// Operations
	virtual void initialize();					// Initialize

	virtual void GaussianCurvature();		// Calculate Gaussian curvature
	virtual void MeanCurvature();			// Calculate mean curvature
	virtual void MaxPrincipalCurvature();	// Calculate maximum principal curvature
	virtual void MinPrincipalCurvature();	// Calculate minimum principal curvature
	virtual void FindMinMaxCurv();				// Find Maximum and Minimum Curvature value from the result

	const Scalar& curvature(VertexHandle _vh) const			// return curvature value
	{ return mesh_.property(curvature_, _vh); }
	const Scalar& max_curvature() const						// return maximum curvature value
	{ return max_curvature_; }
	const Scalar& min_curvature() const						// return minimum curvature value
	{ return min_curvature_; }
	
private:
	// Member variable
	OpenMesh::FPropHandleT<bool>		obtuse_;				// Is a triangle obtuse or not?

	OpenMesh::VPropHandleT<Vec3>		Kh_normal_;				// Mean curvature normal
	OpenMesh::VPropHandleT<Scalar>      Kg_;					// Gaussian curvature
	OpenMesh::VPropHandleT<Scalar>      Kmax_;					// Maximum principal curvature
	OpenMesh::VPropHandleT<Scalar>      Kmin_;					// Minimum principal curvature

	OpenMesh::VPropHandleT<Scalar>      curvature_;

	
	//--------------------------------------------------
	OpenMesh::VPropHandleT<Scalar>      voronoi_area_;			//curvature 계산시 voronoi cell 저장
	//--------------------------------------------------
	Scalar								area_;					//area for entire surface
	//

	Scalar								max_curvature_;
	Scalar								min_curvature_;

public:	
	// set vertex color from vertex curvature
	void color_coding();

	//---------------------------------------------------
	//vertex _vh의 voronoi area 
	//Scalar area(VertexHandle _vh)
	//{ return mesh_.property(voronoi_area_, _vh); }
	
	//전체 surface의 area
	Scalar surfacearea()
	{ return area_;	}


	//----------------------------------------------------

	// easier access to vertex curvature
	Scalar curvature(VertexHandle _vh) 
	{ return mesh_.property(curvature_, _vh); }

	// easier access to mean curvature at a vertex
	Scalar meancurvature(VertexHandle _vh) 
	{ return mesh_.property(Kh_normal_, _vh).norm() * 0.5f; }

	// easier access to Gaussian curvature at a vertex
	Scalar gaussiancurvature(VertexHandle _vh) 
	{ return mesh_.property(Kg_, _vh); }

	// easier access to max. principal curvature at a vertex
	Scalar maximum_prinacipalcurvature(VertexHandle _vh) 
	{ return mesh_.property(Kmax_, _vh); }

	// easier access to minimum principal curvature at a vertex
	Scalar minimum_prinacipalcurvature(VertexHandle _vh) 
	{ return mesh_.property(Kmin_, _vh); }

	//-----------------------------------------------------

	// area
	OpenMesh::VPropHandleT<Scalar> area()
	{ return voronoi_area_;	}

	// mean curvature normal
	OpenMesh::VPropHandleT<Vec3> Kh_normal()
	{ return Kh_normal_;	}

	// Gaussian curvature
	OpenMesh::VPropHandleT<Scalar> Kg()
	{ return Kg_;	}

	// maximum Principal curvature
	OpenMesh::VPropHandleT<Scalar> KMAX()
	{ return Kmax_;	}

	// minimum Principal curvature
	OpenMesh::VPropHandleT<Scalar> KMIN()
	{ return Kmin_;	}



protected:

	Mesh &mesh_;

};


//=============================================================================
} // namespace DiscreteCurvature
} // namespace OpenMesh
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_DISCRETECURVATURET_CC)
#define OPENMESH_DISCRETECURVATURET_TEMPLATES
#include "DiscreteCurvatureT.cc"
#endif
//=============================================================================
#endif // OPENMESH_DISCRETECURVATURE_DISCRETECURVATURET_HH defined
//=============================================================================

