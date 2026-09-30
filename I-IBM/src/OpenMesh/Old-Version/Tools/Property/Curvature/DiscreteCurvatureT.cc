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

/** \file DiscreteCurvatureT.cc
    
 */

//=============================================================================
//
//  CLASS DiscreteCurvatureT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_DISCRETECURVATURE_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Property/Curvature/DiscreteCurvatureT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace DiscreteCurvature {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
DiscreteCurvatureT<Mesh>::
DiscreteCurvatureT(Mesh& _mesh)
  : mesh_(_mesh)
{
 	mesh_.add_property(voronoi_area_);
 	mesh_.add_property(curvature_);
 	mesh_.add_property(Kh_normal_);
 	mesh_.add_property(Kg_	);
 	mesh_.add_property(Kmax_);
 	mesh_.add_property(Kmin_);
	
}

//-----------------------------------------------------------------------------


template <class Mesh>
DiscreteCurvatureT<Mesh>::
~DiscreteCurvatureT()
{
 	mesh_.remove_property(voronoi_area_);
 	mesh_.remove_property(curvature_);
 	mesh_.remove_property(Kh_normal_);
 	mesh_.remove_property(Kg_	);
 	mesh_.remove_property(Kmax_);
 	mesh_.remove_property(Kmin_);

}

//-----------------------------------------------------------------------------
//======================================================
template <class Mesh>
void DiscreteCurvatureT<Mesh>::initialize()
{
	// 둔각(obtuse) 삼각형 테스트
	mesh_.add_property(obtuse_);
	typename Mesh::FaceIter				f_it, f_end(mesh_.faces_end());
	typename Mesh::FaceVertexIter		fv_it;
	typename Mesh::Point				p0, p1, p2, d0, d1;

	for (f_it=mesh_.faces_begin(); f_it!=f_end; ++f_it)
	{
		mesh_.property(obtuse_, f_it) = false;
		
		fv_it = mesh_.fv_iter(f_it);
		
		p0 = mesh_.point(fv_it);  ++fv_it;
		p1 = mesh_.point(fv_it);  ++fv_it;
		p2 = mesh_.point(fv_it);
		
		d0 = (p1 - p0).normalize();
		d1 = (p2 - p0).normalize();
		if (acos(d0|d1) < 0)
			mesh_.property(obtuse_, f_it) = true;
		
		d0 = (p0 - p1).normalize();
		d1 = (p2 - p1).normalize();
		if (acos(d0|d1) < 0)
			mesh_.property(obtuse_, f_it) = true;

		d0 = (p0 - p2).normalize();
		d1 = (p1 - p2).normalize();
		if (acos(d0|d1) < 0)
			mesh_.property(obtuse_, f_it) = true;
	}


	Scalar cot_alpha, cot_beta;

	typename Mesh::VertexIter				v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexOHalfedgeIter		voh_it;
	typename Mesh::HalfedgeHandle			h0, h1;
	typename Mesh::FaceHandle				f0;
	Vec3									laplace(0.0, 0.0, 0.0);
	Scalar									Kh, Kg, area, theta;	// Kh(mean curvature), Kg(Gaussian curvature), area(mixed area), theta(꼭지점의 각도)
	
	//전체 surface의 면적
	area_ = 0;

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		// 초기화		
		laplace = Vec3(0,0,0);
		area = cot_alpha = cot_beta = theta = 0.0f;
		
		mesh_.property(Kh_normal_ , v_it)			= Vec3(0.0f, 0.0f, 0.0f);
		mesh_.property(Kg_	, v_it)					= 0.0f;
		mesh_.property(Kmax_				, v_it) = 0.0f;
		mesh_.property(Kmin_				, v_it) = 0.0f;

		mesh_.property(curvature_, v_it)			= 0;

		//boundary vertices가 아닌 v에 대해...
		if (!mesh_.is_boundary(v_it.handle()))
		{
			p0 = mesh_.point(v_it);

			//1-ring
			for (voh_it=mesh_.voh_iter(v_it); voh_it; ++voh_it)
			{
				// mixed area 계산 시작  ---------------------------------
				h0 = voh_it.handle();
				p1 = mesh_.point(mesh_.to_vertex_handle(h0));

				h1 = mesh_.next_halfedge_handle(h0);
				p2 = mesh_.point(mesh_.to_vertex_handle(h1));
				f0 = mesh_.face_handle(h1);
				
				d0 = (p0 - p2).normalize();
				d1 = (p1 - p2).normalize();
	
				
				//직각 삼각형일 경우 tan(a)가 0가 되어서 cot_alpha가 infinite
				if( fabs(d0|d1) < 0.001f )
					cot_alpha = 0.f;

				else
					cot_alpha = 1.0 / tan(acos(d0|d1));

				//직각 이하일때
				if(mesh_.property(obtuse_, f0) == false)
				{
						area += cot_alpha*(p0-p1).length()*(p0-p1).length()/8;
				}

				//둔각일때
				else
				{
					d0 = (p1 - p0).normalize();
					d1 = (p2 - p0).normalize();

					if(acos(d0|d1) < 0)					// p0를 포함하는 각이 둔각인 경우
						area += ((p1-p0)%(p2-p0)).norm() * 0.5f / 2;
					else								// p1이나 p2를 포함하는 각이 둔각인 경우
						area += ((p1-p0)%(p2-p0)).norm() * 0.5f / 4;
				}

				h1 = mesh_.opposite_halfedge_handle(h0);
				h1 = mesh_.next_halfedge_handle(h1);
				p2 = mesh_.point(mesh_.to_vertex_handle(h1)); 
				f0 = mesh_.face_handle(h1);

				d0 = (p0 - p2).normalize();
				d1 = (p1 - p2).normalize();


				//직각일 경우 고려
				if( fabs(d0|d1) < 0.001f )
					cot_beta = 0.f;

				else
					cot_beta = 1.0 / tan(acos(d0|d1));

				if(mesh_.property(obtuse_, f0) == false)
				{
						area += cot_beta*(p0-p1).length()*(p0-p1).length()/8;
				}

				else
				{
					d0 = (p1 - p0).normalize();
					d1 = (p2 - p0).normalize();

					if(acos(d0|d1) < 0)					// p0를 포함하는 각이 둔각인 경우
						area += ((p1-p0)%(p2-p0)).norm() * 0.5f / 2;
					else								// p1이나 p2를 포함하는 각이 둔각인 경우
						area += ((p1-p0)%(p2-p0)).norm() * 0.5f / 4;
				}

				// mixed area 계산 끝 ---------------------------------

				laplace += (cot_alpha + cot_beta)*(p0 - p1);					// Mean curvature normal을 계산하기 위해

				theta += acos( (p1 - p0).normalize() | (p2 - p0).normalize() );	// Gaussian curvature 계산하기 위해

			}		// end of vertexohalfedge iterator

			laplace = laplace/(2*area);
			mesh_.property(Kh_normal_	, v_it) = laplace;
			Kg									= (2*M_PI - theta) / area;
			Kh									= laplace.norm() * 0.5f;
			mesh_.property(Kg_	,		  v_it)	= Kg;
			mesh_.property(Kmax_,		  v_it)	= Kh + sqrt(Kh*Kh - Kg);
			mesh_.property(Kmin_,		  v_it)	= Kh - sqrt(Kh*Kh - Kg);;

			mesh_.property(voronoi_area_, v_it) = area;
			area_ += area;				//surface area

		}		// end of boundary test
		
	}		// end of vertex iterator

	mesh_.remove_property(obtuse_);

}

//-----------------------------------------------------------------------------

template <class Mesh>
void DiscreteCurvatureT<Mesh>::color_coding()
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
      else // [v1, v2]
      {
	u = (unsigned char) (255.0 * (curv - v1) / (v2 - v1));
	col = Mesh::Color(0, 255, 255-u);
      }
    }
    else 
    {
      if (curv <= v3) // [v2, v3]
      {
	u = (unsigned char) (255.0 * (curv - v2) / (v3 - v2));
	col = Mesh::Color(u, 255, 0);
      }
      else // [v3, v4]
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
void DiscreteCurvatureT<Mesh>::GaussianCurvature()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(Kg_, v_it);
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void DiscreteCurvatureT<Mesh>::MeanCurvature()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(Kh_normal_, v_it).norm() * 0.5f;
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void DiscreteCurvatureT<Mesh>::MaxPrincipalCurvature()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(Kmax_, v_it);
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void DiscreteCurvatureT<Mesh>::MinPrincipalCurvature()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(curvature_, v_it) = mesh_.property(Kmin_, v_it);
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void DiscreteCurvatureT<Mesh>::FindMinMaxCurv()
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

//=============================================================================
} // namespace DiscreteCurvature
} // namespace OpenMesh
//=============================================================================
