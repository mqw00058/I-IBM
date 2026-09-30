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

/** \file CurvatureT.cc
    
 */

//=============================================================================
//
//  CLASS CurvatureT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_CCURVATURET_C

//== INCLUDES =================================================================

#include <vector>
#include <float.h>
#include <OpenMesh/Tools/Property/Curvature/CurvatureT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Curvature {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
CurvatureT<Mesh>::
CurvatureT(Mesh& _mesh)
  : mesh_(_mesh)
{
	n_feature_ = 0;
	
	mesh_.add_property(curvature_);
	mesh_.add_property(bary_area_);
	max_curvature_ = -FLT_MAX;
	min_curvature_ = FLT_MAX;

	// SIGGRAPH Course Note 참고
	mesh_.add_property(vweight_);
	mesh_.add_property(eweight_);
}

//-----------------------------------------------------------------------------

template <class Mesh>
CurvatureT<Mesh>::
~CurvatureT()
{
	mesh_.remove_property(curvature_);
	mesh_.remove_property(bary_area_);

	// SIGGRAPH Course Note 참고
	mesh_.remove_property(vweight_);
	mesh_.remove_property(eweight_);
}

//-----------------------------------------------------------------------------

template <class Mesh>
void CurvatureT<Mesh>::initialize()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = 0;
		mesh_.property(bary_area_, v_it) = 0;
		mesh_.status(v_it).set_feature(false);
	}
}

//-----------------------------------------------------------------------------
// Calculate Gaussian curvature based on Rodregues method
template <class Mesh>
void CurvatureT<Mesh>::CalcGaussCurvature1()
{
    
	// clear curvature
	typename Mesh::VertexIter  v_it  = mesh_.vertices_begin(), v_end = mesh_.vertices_end();
	typename Mesh::VertexVertexIter vv_it;

	for (; v_it != v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = 0;
		mesh_.property(bary_area_, v_it) = 0;
	}

	// calc curvatures and area
	typename Mesh::FaceIter          f_it  = mesh_.faces_begin(), f_end = mesh_.faces_end();

	typename Mesh::FaceVertexIter    fv_it;
	typename Mesh::VertexHandle      vh0, vh1, vh2;

	Vec3							   v0, v1, v2, n;
	Scalar							   area(0);
	Scalar							   angle0(0), angle1(0), angle2(0);
	Scalar							   temp;

	// Calculate sum of vertex angles
	for (; f_it != f_end; ++f_it)
	{
		fv_it = mesh_.fv_iter(f_it.handle());
		vh0 = fv_it.handle();  ++fv_it;
		vh1 = fv_it.handle();  ++fv_it;
		vh2 = fv_it.handle();

		{
			v0 = vector_cast<Vec3>(mesh_.point(vh0));
			v1 = vector_cast<Vec3>(mesh_.point(vh1));
			v2 = vector_cast<Vec3>(mesh_.point(vh2));
		}

		n    =  (v1-v0) % (v2-v0);
		area = n.norm();
		if (area > FLT_MIN) 
		{
			n /= area;
			area *= 0.5;
		}

		temp = dot((v1-v0),(v2-v0))/((v1-v0).norm()*(v2-v0).norm());
		angle0 = acos(temp);
		if(temp > 1)
			angle0 = acos(1.0f);
		if(temp < -1)
			angle0 = acos(-1.0f);

		temp = dot((v2-v1),(v0-v1))/((v2-v1).norm()*(v0-v1).norm());
		angle1 = acos(temp) ;
		if(temp > 1)
			angle0 = acos(1.0f);
		if(temp < -1)
			angle0 = acos(-1.0f);

		temp = dot((v0-v2),(v1-v2))/((v0-v2).norm()*(v1-v2).norm());
		angle2 = acos(temp);
		if(temp > 1)
			angle0 = acos(1.0f);
		if(temp < -1)
			angle0 = acos(-1.0f);

		mesh_.property(curvature_, vh0) += angle0;
		mesh_.property(curvature_, vh1) += angle1;
		mesh_.property(curvature_, vh2) += angle2;

		mesh_.property(bary_area_, vh0) += area;
		mesh_.property(bary_area_, vh1) += area;
		mesh_.property(bary_area_, vh2) += area;
	}

	for (v_it  = mesh_.vertices_begin(); v_it != v_end; ++v_it)
	{
		if(mesh_.is_boundary(v_it.handle()))  
		{
			mesh_.property(curvature_, v_it) = (M_PI - mesh_.property(curvature_, v_it)) / (mesh_.property(bary_area_, v_it)/3);
			if(mesh_.property(curvature_,v_it) < 0)
				mesh_.property(curvature_,v_it) = -mesh_.property(curvature_,v_it);
		}

		else
		{
			mesh_.property(curvature_, v_it) = (2*M_PI - mesh_.property(curvature_, v_it)) / (mesh_.property(bary_area_, v_it)/3);
			if(mesh_.property(curvature_,v_it) < 0)
				mesh_.property(curvature_,v_it) = -mesh_.property(curvature_,v_it);
		}

	}

}

//-----------------------------------------------------------------------------

// Calculate Mean curvature based on Zorin method
//			    v0
//		       /|\
//		      / | \
//		     /  |  \
//	 angle1	v3  |   v2 angle0
//			 \  |  /
//			  \ | /
//			   \|/
//			    v1

template <class Mesh>
void CurvatureT<Mesh>::CalcMeanCurvature1()
{
	// clear curvature
	typename Mesh::VertexIter  v_it  = mesh_.vertices_begin(), v_end = mesh_.vertices_end();
	typename Mesh::VertexVertexIter vv_it;
	typename Mesh::HalfedgeHandle heh0, heh1;

	for (; v_it != v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = 0;
		mesh_.property(bary_area_, v_it) = 0;
	}

	// calc curvatures and area
	typename Mesh::EdgeIter          e_it  = mesh_.edges_begin(), e_end = mesh_.edges_end();
	typename Mesh::VertexHandle      vh0, vh1, vh2, vh3;

	Vec3							   v0, v1, v2, v3, n1, n2;
	Scalar							   area1(0), area2(0);
	Scalar							   angle0(0), angle1(0);
	Scalar							   temp;
	
	// Calculate sum of (cot(angle0)+cot(angle1))*(edge length)
	for (; e_it != e_end; ++e_it)
	{
		heh0 = mesh_.halfedge_handle(e_it, 0);
		
		vh0 = mesh_.from_vertex_handle(heh0);
		vh1 = mesh_.to_vertex_handle(heh0);
		vh2 = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh0));

		{
			v0 = vector_cast<Vec3>(mesh_.point(vh0));
			v1 = vector_cast<Vec3>(mesh_.point(vh1));
			v2 = vector_cast<Vec3>(mesh_.point(vh2));
		}

		n1    =  (v1-v0) % (v2-v0);
		area1 = n1.norm();
		if (area1 > FLT_MIN) 
		{
			n1 /= area1;
			area1 *= 0.5;
		}

		temp = dot((v0-v2),(v1-v2))/((v0-v2).norm()*(v1-v2).norm());
		angle0 = acos(temp);
		if(temp > 1)	
			angle0 = acos(1.0f);		
		if(temp < -1)
			angle0 = acos(-1.0f);	

		if(!mesh_.is_boundary(e_it))
		{
			heh1 = mesh_.halfedge_handle(e_it, 1);
			vh3 = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh1));
			v3 = vector_cast<Vec3>(mesh_.point(vh3));

			n2    =  (v0-v1) % (v3-v1);
			area2 = n2.norm();
			if (area2 > FLT_MIN) 
			{
				n2 /= area2;
				area2 *= 0.5;
			}

			mesh_.property(bary_area_, vh0) += area1/6;
			mesh_.property(bary_area_, vh0) += area2/6;

			mesh_.property(bary_area_, vh1) += area1/6;
			mesh_.property(bary_area_, vh1) += area2/6;

			temp = dot((v0-v3),(v1-v3))/((v0-v3).norm()*(v1-v3).norm());
			angle1 = acos(temp);
			if(temp > 1)	
				angle0 = acos(1.0f);		
			if(temp < -1)
				angle0 = acos(-1.0f);

			Scalar convex_test;					// whether the edge is convex or concave?

			convex_test = ( (n1%n2) + (v1-v0).normalize() ).norm();

			if(convex_test > 1)
			{
				mesh_.property(curvature_, vh0) += ( 1/tan(angle0)+1/tan(angle1) ) * (v1-v0).norm() * 0.5;
				mesh_.property(curvature_, vh1) += ( 1/tan(angle0)+1/tan(angle1) ) * (v1-v0).norm() * 0.5;
			}
			else
			{
				mesh_.property(curvature_, vh0) += -( 1/tan(angle0)+1/tan(angle1) ) * (v1-v0).norm() * 0.5;
				mesh_.property(curvature_, vh1) += -( 1/tan(angle0)+1/tan(angle1) ) * (v1-v0).norm() * 0.5;			
			}
		}

		else
		{
			mesh_.property(bary_area_, vh0) += area1/6;
			mesh_.property(bary_area_, vh1) += area1/6;		

			mesh_.property(curvature_, vh0) += (1/tan(angle0) )* (v1-v0).norm() * 0.5;
			mesh_.property(curvature_, vh1) += (1/tan(angle0) )* (v1-v0).norm() * 0.5;;
		}

	}
	
    for (v_it  = mesh_.vertices_begin(); v_it != v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(curvature_, v_it) / mesh_.property(bary_area_, v_it);

		// ----------------------------------------------------------------
		// Convex test
		v0 = v0*0;
		for(vv_it=mesh_.vv_iter( v_it ); vv_it; ++vv_it)
		{
			v0 += vector_cast<Vec3>(mesh_.point(vv_it)) - vector_cast<Vec3>(mesh_.point(v_it));
		}

		v0 *= 1/v0.norm();
		v1 = vector_cast<Vec3>(mesh_.normal(v_it));

		if(dot(v0,v1) > 0)
			mesh_.property(curvature_, v_it) = -fabs(mesh_.property(curvature_, v_it));
		// ----------------------------------------------------------------
		
	}


}

//-----------------------------------------------------------------------------

// Calculate Mean curvature based on Steiner method : using dihedral angles
//       v0
//      /|\
//     / | \
//    /  |  \
//   v3  |   v2
//    \  |  /
//     \ | /
//      \|/
//       v1
template <class Mesh>
void CurvatureT<Mesh>::CalcMeanCurvature2()
{
	// clear curvature
	typename Mesh::VertexIter  v_it  = mesh_.vertices_begin(), v_end = mesh_.vertices_end();
	typename Mesh::VertexVertexIter vv_it;
	typename Mesh::HalfedgeHandle heh0, heh1;

	for (; v_it != v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = 0;
		mesh_.property(bary_area_, v_it) = 0;
	}

	// calc curvatures and area
	typename Mesh::EdgeIter          e_it  = mesh_.edges_begin(), e_end = mesh_.edges_end();

	//Mesh::FaceVertexIter    fv_it;
	typename Mesh::VertexHandle      vh0, vh1, vh2, vh3;

	Vec3							   v0, v1, v2, v3, n1, n2;
	Scalar							   area1(0), area2(0);
	Scalar							   angle0(0);

	for (; e_it != e_end; ++e_it)
	{
		heh0 = mesh_.halfedge_handle(e_it, 0);

		vh0 = mesh_.from_vertex_handle(heh0);
		vh1 = mesh_.to_vertex_handle(heh0);
		vh2 = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh0));

		{
			v0 = vector_cast<Vec3>(mesh_.point(vh0));
			v1 = vector_cast<Vec3>(mesh_.point(vh1));
			v2 = vector_cast<Vec3>(mesh_.point(vh2));
		}

		n1    =  (v1-v0) % (v2-v0);
		area1 = n1.norm();
		if (area1 > FLT_MIN) 
		{
			n1 /= area1;
			area1 *= 0.5;
		}

		if(!mesh_.is_boundary(e_it))
		{
			heh1 = mesh_.halfedge_handle(e_it, 1);
			vh3 = mesh_.to_vertex_handle(mesh_.next_halfedge_handle(heh1));
			v3 = vector_cast<Vec3>(mesh_.point(vh3));

			n2    =  (v0-v1) % (v3-v1);
			area2 = n2.norm();


			if (area2 > FLT_MIN) 
			{
				n2 /= area2;
				area2 *= 0.5;
			}

			mesh_.property(bary_area_, vh0) += area1/6;
			mesh_.property(bary_area_, vh0) += area2/6;

			mesh_.property(bary_area_, vh1) += area1/6;
			mesh_.property(bary_area_, vh1) += area2/6;

			angle0 = acos( dot(n1, n2) );		// calculate a dihedral angle
			if(dot(n1,n2) > 1)	
				angle0 = acos(1.0f);		
			if(dot(n1,n2) < -1)
				angle0 = acos(-1.0f);

			//mesh_.property(curvature_, vh0) += (angle0 * (v1-v0).norm() * 0.25);
			//mesh_.property(curvature_, vh1) += (angle0 * (v1-v0).norm() * 0.25);
				
			Scalar convex_test;					// whether the edge is convex or concave?

			convex_test = ( (n1%n2) + (v1-v0).normalize() ).norm();

			if(convex_test > 1)
			{
				mesh_.property(curvature_, vh0) += (angle0 * (v1-v0).norm() * 0.25);
				mesh_.property(curvature_, vh1) += (angle0 * (v1-v0).norm() * 0.25);
			}

			else
			{
				mesh_.property(curvature_, vh0) += -(angle0 * (v1-v0).norm() * 0.25);
				mesh_.property(curvature_, vh1) += -(angle0 * (v1-v0).norm() * 0.25);
			}
		}

		else
		{
			mesh_.property(bary_area_, vh0) += area1/6;
			mesh_.property(bary_area_, vh1) += area1/6;		
		}

	}

	for (v_it  = mesh_.vertices_begin(); v_it != v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(curvature_, v_it) / mesh_.property(bary_area_, v_it);
	}

}

template <class Mesh>
void CurvatureT<Mesh>::FindMinMaxCurv()
{
	typename Mesh::VertexIter        v_it, v_end(mesh_.vertices_end());
	
	// put all curvature values into one array
	std::vector<Scalar> curv_values;
	curv_values.reserve(mesh_.n_vertices());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		if(!mesh_.is_boundary(v_it))
			curv_values.push_back(curvature(v_it));
	}
		

	// Find min and max curvature
	unsigned int n = curv_values.size()-1;
	std::sort(curv_values.begin(), curv_values.end());
	min_curvature_ = curv_values[0];
	max_curvature_ = curv_values[n];
}

//-----------------------------------------------------------------------------

template <class Mesh>
void CurvatureT<Mesh>::calc_weights()
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


//-----------------------------------------------------------------------------


template <class Mesh>
void CurvatureT<Mesh>::calc_curvature()
{
  typename Mesh::VertexIter        v_it, v_end(mesh_.vertices_end());
  typename Mesh::HalfedgeHandle    h;
  typename Mesh::EdgeHandle        e;
  typename Mesh::VertexVertexIter  vv_it;
  typename Mesh::Point             laplace(0.0, 0.0, 0.0);


  for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    curvature(v_it) = 0.0;
    laplace = Mesh::Point(0,0,0);

    if (!mesh_.is_boundary(v_it.handle()))
    {
      for (vv_it=mesh_.vv_iter(v_it); vv_it; ++vv_it)
      {
	h = vv_it.current_halfedge_handle();
	e = mesh_.edge_handle(h);

	laplace += weight(e) * (mesh_.point(vv_it) - mesh_.point(v_it));
      }
      laplace *= weight(v_it);

      curvature(v_it) = laplace.norm();
    }
  }
}


//-----------------------------------------------------------------------------


template <class Mesh>
void CurvatureT<Mesh>::color_coding()
{

  typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
  typename Mesh::Scalar      curv, min_curv(FLT_MAX), max_curv(-FLT_MAX);
  typename Mesh::Color       col;


  // put all curvature values into one array
  std::vector<Scalar> curv_values;
  curv_values.reserve(mesh_.n_vertices());
  for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
    curv_values.push_back(curvature(v_it));


  // discard upper and lower 5%
  unsigned int n = curv_values.size()-1;
  unsigned int i = n / 20;
  std::sort(curv_values.begin(), curv_values.end());
  min_curv = curv_values[i];
  max_curv = curv_values[n-1-i];


  // define uniform color intervalls [v0,v1,v2,v3,v4]
  Scalar v0, v1, v2, v3, v4;
  v0 = min_curv + 0.0/4.0 * (max_curv - min_curv);
  v1 = min_curv + 1.0/4.0 * (max_curv - min_curv);
  v2 = min_curv + 2.0/4.0 * (max_curv - min_curv);
  v3 = min_curv + 3.0/4.0 * (max_curv - min_curv);
  v4 = min_curv + 4.0/4.0 * (max_curv - min_curv);



  // map curvatures to colors
  for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
  {
    curv = curvature(v_it);
    col = Mesh::Color(255,255,255);
    
    unsigned char u;

    if (curv < v0)
    {
      col = Mesh::Color(0, 0, 255);
    }
    else if (curv > v4) 
    {
      col = Mesh::Color(255, 0, 0);
    }

    else if (curv <= v2) 
    {
      if (curv <= v1) // [v0, v1]
      {
	u = (unsigned char) (255.0 * (curv - v0) / (v1 - v0));
	col = Mesh::Color(0, u, 255);
      }      
      else // ]v1, v2]
      {
	u = (unsigned char) (255.0 * (curv - v1) / (v2 - v1));
	col = Mesh::Color(0, 255, 255-u);
      }
    }
    else 
    {
      if (curv <= v3) // ]v2, v3]
      {
	u = (unsigned char) (255.0 * (curv - v2) / (v3 - v2));
	col = Mesh::Color(u, 255, 0);
      }
      else // ]v3, v4]
      {
	u = (unsigned char) (255.0 * (curv - v3) / (v4 - v3));
	col = Mesh::Color(255, 255-u, 0);
      }
    }

    mesh_.set_color(v_it, col);
  }
}

//-----------------------------------------------------------------------------

template <class Mesh>
void CurvatureT<Mesh>::detect_feature(unsigned int percent_)
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	Scalar _threshold;			// percent에 해당하는 curvature의 값
	
	std::vector<Scalar> curvature_values;
		
	curvature_values.clear();
	curvature_values.reserve(mesh_.n_vertices());
	
	
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		curvature_values.push_back(curvature(v_it));
		mesh_.status(v_it).set_feature(false);
	}

	// percent_에 해당하는 max. color diffence 값을 찾는다.
	std::sort(curvature_values.begin(), curvature_values.end(), greater<Scalar>( ));

	vector <Scalar>::size_type  n = curvature_values.size()-1;
	vector <Scalar>::size_type  i = n * percent_ / 100;
	_threshold = curvature_values[i];

	// _threshold 값 이상의 max. color difference를 갖는 vertex 를 feature로 셋업
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		if(curvature(v_it) >= _threshold)
		{
			mesh_.status(v_it).set_feature(true);
			n_feature_++;
		}
	}

}

//=============================================================================
} // namespace Curvature
} // namespace OpenMesh
//=============================================================================
