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

/** \file LaplaceSIGT.cc
    
 */

//=============================================================================
//
//  CLASS LaplaceSIGT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_LAPLACESIG_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Smoother/LaplaceSIGT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Smoother {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
LaplaceSIGT<Mesh>::
LaplaceSIGT(Mesh& _mesh)
  : mesh_(_mesh)
{
	mesh_.add_property(vpos_);	
	mesh_.add_property(vweight_);
	mesh_.add_property(eweight_);
}

//-----------------------------------------------------------------------------


template <class Mesh>
LaplaceSIGT<Mesh>::
~LaplaceSIGT()
{
	mesh_.remove_property(vpos_);
	mesh_.remove_property(vweight_);
	mesh_.remove_property(eweight_);
}

//-----------------------------------------------------------------------------

template <class Mesh>
void LaplaceSIGT<Mesh>::initialize()
{
	calc_weights();
}

template <class Mesh>
void LaplaceSIGT<Mesh>::calc_weights()
{
  typename Mesh::VertexIter        v_it, v_end(mesh_.vertices_end());
  typename Mesh::EdgeIter          e_it, e_end(mesh_.edges_end());
  typename Mesh::VertexFaceIter    vf_it;
  typename Mesh::FaceVertexIter    fv_it;
  typename Mesh::HalfedgeHandle    h0, h1, h2;
  typename Mesh::VertexHandle      v0, v1;
  typename Mesh::Point             p0, p1, p2, d0, d1;
  Scalar						   w, area;

  for (e_it=mesh_.edges_begin(); e_it!=e_end; ++e_it)
  {
    w  = 0.0;

    h0 = mesh_.halfedge_handle(e_it.handle(), 0);
    v0 = mesh_.to_vertex_handle(h0);
    p0 = mesh_.point(v0);

    h1 = mesh_.halfedge_handle(e_it.handle(), 1);
    v1 = mesh_.to_vertex_handle(h1);
    p1 = mesh_.point(v1);

    h2 = mesh_.next_halfedge_handle(h0);
    p2 = mesh_.point(mesh_.to_vertex_handle(h2));
    d0 = (p0 - p2).normalize();
    d1 = (p1 - p2).normalize();
    w += 1.0 / tan(acos(d0|d1));

    h2 = mesh_.next_halfedge_handle(h1);
    p2 = mesh_.point(mesh_.to_vertex_handle(h2));
    d0 = (p0 - p2).normalize();
    d1 = (p1 - p2).normalize();
    w += 1.0 / tan(acos(d0|d1));

    weight(e_it) = w;
  }
   

  for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    area = 0.0;

    for (vf_it=mesh_.vf_iter(v_it); vf_it; ++vf_it)
    {
      fv_it = mesh_.fv_iter(vf_it);

      const Mesh::Point& P = mesh_.point(fv_it);  ++fv_it;
      const Mesh::Point& Q = mesh_.point(fv_it);  ++fv_it;
      const Mesh::Point& R = mesh_.point(fv_it);

      area += ((Q-P)%(R-P)).norm() * 0.5f * 0.3333f;
    }

    weight(v_it) = 1.0 / (4.0 * area);
  }
}

template <class Mesh>
void LaplaceSIGT<Mesh>::smooth(unsigned int _iters)
{
	Mesh::VertexIter        v_it, v_end(mesh_.vertices_end());
	Mesh::HalfedgeHandle    h;
	Mesh::EdgeHandle        e;
	Mesh::VertexVertexIter  vv_it;
	Mesh::Point             laplace(0.0, 0.0, 0.0);
	Mesh::Scalar            w, ww;


	for (unsigned int iter=0; iter<_iters; ++iter)
	{

		// compute new vertex positions by Laplacian smoothing
		for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		{
			laplace = Mesh::Point(0,0,0);
			ww = 0.0;

			if (!mesh_.is_boundary(v_it))
			{
				for (vv_it=mesh_.vv_iter(v_it); vv_it; ++vv_it)
				{
					h = vv_it.current_halfedge_handle();
					e = mesh_.edge_handle(h);
					w = weight(e);
					ww += w;

					laplace += w * (mesh_.point(vv_it) - mesh_.point(v_it));
				}

				laplace /= ww;   // normalize by sum of weights
				laplace *= 0.5;  // damping
			}

			new_pos(v_it) = mesh_.point(v_it) + laplace;
		}



		// update vertex positions
		for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
			mesh_.set_point(v_it, new_pos(v_it));
	}

}
//-----------------------------------------------------------------------------

//=============================================================================
} // namespace Smoother
} // namespace OpenMesh
//=============================================================================
