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
//   $Laplacian smoothing of a point-set - Umbrella operator [Pauly02]
//	 $
//   $Date: 2013.01.26 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//    
// Description
//	- scale dependent umbrella operator (1/pi-pj)
//=============================================================================

/** \file UmbrellaOperatorT.hh
    
 */
#pragma once
//=============================================================================
//
//  CLASS UmbrellaOperatorT
//
//=============================================================================

#ifndef PBM_FILTER_UMBRELLA_HH
#define PBM_FILTER_UMBRELLA_HH


//== INCLUDES =================================================================
#include <pbm/Common/NormalEstimation.h>

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>
//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================
using namespace OpenMesh;
typedef PBM::NormalEstimation	NE;

namespace PBM {
namespace Filter {

//== CLASS DEFINITION =========================================================

template <class Mesh>
class UmbrellaOperatorT
{
public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::Point					Point;
public:

	// constructor & destructor
	UmbrellaOperatorT( Mesh& _mesh );
	virtual ~UmbrellaOperatorT();


public:
	// Operations
	virtual void initialize();					// Initialize
	virtual void apply(int iteration, float lambda);

private:
	// Member variable

protected:

	Mesh&  mesh_;
	vector<Vec3f>	point_set;	// point list (data structure)
	NE *ne;
	int nPts;
};


//=============================================================================
} // namespace Filter
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_FILTER_UMBRELLA_CC)
#define OPENMESH_UMBRELLA_TEMPLATES
#include "UmbrellaOperator.cc"
#endif
//=============================================================================
#endif // OPENMESH_ClassEx_ClassExT_HH defined
//=============================================================================

