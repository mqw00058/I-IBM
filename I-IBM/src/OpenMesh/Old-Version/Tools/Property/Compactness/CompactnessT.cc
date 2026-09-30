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

/** \file CompactnessT.cc
    
 */

//=============================================================================
//
//  CLASS CompactnessT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_COMPACTNESS_CC

//== INCLUDES =================================================================

#include <vector>
#include <OpenMesh/Tools/Property/Compactness/CompactnessT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Compactness {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
CompactnessT<Mesh>::
CompactnessT(Mesh& _mesh)
  : mesh_(_mesh)
{
	mesh_.add_property(compactness_);
	max_compactness_ = -FLT_MAX;
	min_compactness_ = FLT_MAX;
	ave_compactness_ = 0.0f;
}

//-----------------------------------------------------------------------------


template <class Mesh>
CompactnessT<Mesh>::
~CompactnessT()
{
	mesh_.remove_property(compactness_);
}

//-----------------------------------------------------------------------------

template <class Mesh>
void CompactnessT<Mesh>::initialize()
{
	typedef  VectorT<float, 3>		Vec3;
	
	typename Mesh::VertexIter		v_it, v_end(mesh_.vertices_end());
	typename Mesh::FaceIter			f_it, f_end(mesh_.faces_end());
	typename Mesh::FaceVertexIter	fv_it; 
	typename Mesh::VertexHandle     vh0, vh1, vh2;

	const double FOUR_ROOT3 = 6.928203230275509;

	for (f_it=mesh_.faces_begin(); f_it!=f_end; ++f_it)
	{
		mesh_.property(compactness_, f_it) = 0;
	}

	double								area(0);
	
	Vec3	v0, v1, v2, n1;
	
	for (f_it=mesh_.faces_begin(); f_it!=f_end; ++f_it)
	{
		fv_it = mesh_.fv_iter(f_it.handle());
		vh0 = fv_it.handle();  ++fv_it;
		vh1 = fv_it.handle();  ++fv_it;
		vh2 = fv_it.handle();
		
		v0 = vector_cast<Vec3>(mesh_.point(vh0));
		v1 = vector_cast<Vec3>(mesh_.point(vh1));
		v2 = vector_cast<Vec3>(mesh_.point(vh2));

		double el0(0), el1(0), el2(0);

		n1 =  (v1-v0) % (v2-v0);
		
		area = n1.norm();

		if (area > FLT_MIN) 
		{
			area *= 0.5;
		}

		el0 = (v1-v0).norm();
		el1 = (v2-v1).norm();
		el2 = (v0-v2).norm();

		mesh_.property(compactness_, f_it) = FOUR_ROOT3 * area /
											( el0*el0 + el1*el1 + el2*el2 );
	}

	std::vector<Scalar> compactness_values;

	compactness_values.clear();
	compactness_values.reserve(mesh_.n_faces());
	
	
	for(f_it=mesh_.faces_begin(); f_it!=f_end; ++f_it)
	{
		compactness_values.push_back(compactness(f_it));
		ave_compactness_ += compactness(f_it);
	}

	// percent_에 해당하는 max. color diffence 값을 찾는다.
	std::sort(compactness_values.begin(), compactness_values.end() );

	vector <Scalar>::size_type  n = compactness_values.size()-1;
	max_compactness_ = compactness_values[n];
	min_compactness_ = compactness_values[0];
	ave_compactness_ /= (n+1);
}
//-----------------------------------------------------------------------------

//=============================================================================
} // namespace Compactness
} // namespace OpenMesh
//=============================================================================
