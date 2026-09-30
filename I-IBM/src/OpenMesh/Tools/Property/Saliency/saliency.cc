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

#ifndef OPENMESH_PROPERTY_SALIENCY_CC
#define OPENMESH_PROPERTY_SALIENCY_CC

//== INCLUDES =================================================================
#include "saliency.hh"

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Property {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
SaliencyT<Mesh>::
	SaliencyT(Mesh& _mesh)
	: mesh_(_mesh)
{

	mesh_.add_property(saliency_);
	mesh_.add_property(Kh_normal_);
	mesh_.add_property(Kg_	);
	mesh_.add_property(Kmax_);
	mesh_.add_property(Kmin_);
	initialize();


}

//-----------------------------------------------------------------------------


template <class Mesh>
SaliencyT<Mesh>::
	~SaliencyT()
{
	mesh_.remove_property(saliency_);
	mesh_.remove_property(Kh_normal_);
	mesh_.remove_property(Kg_	);
	mesh_.remove_property(Kmax_);
	mesh_.remove_property(Kmin_);
}





//-----------------------------------------------------------------------------
template <class Mesh>
void SaliencyT<Mesh>::initialize()
{
	// geometry property setting
	Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
		point_set.push_back(mesh_.point(v_it));
		// initialize
		mesh_.property(saliency_, v_it) = 0;

	}	
	ne = new NormalEstimation(point_set);


	// discrete curvature
	// 둔각(obtuse) 삼각형 테스트
	OpenMesh::FPropHandleT<bool>		obtuse_;				// Is a triangle obtuse or not?
	mesh_.add_property(obtuse_);
	Mesh::FaceIter				f_it, f_end(mesh_.faces_end());
	Mesh::FaceVertexIter		fv_it;
	Mesh::Point				p0, p1, p2, d0, d1;

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

	Mesh::VertexOHalfedgeIter		voh_it;
	Mesh::HalfedgeHandle			h0, h1;
	Mesh::FaceHandle				f0;
	Vec3f							laplace(0.0, 0.0, 0.0);
	Scalar							Kh, Kg, area, theta;	// Kh(mean curvature), Kg(Gaussian curvature), area(mixed area), theta(꼭지점의 각도)

	//전체 surface의 면적

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		// 초기화		
		laplace = Vec3f(0,0,0);
		area = cot_alpha = cot_beta = theta = 0.0f;

		mesh_.property(Kh_normal_ , v_it)			= Vec3f(0.0f, 0.0f, 0.0f);
		mesh_.property(Kg_	, v_it)					= 0.0f;
		mesh_.property(Kmax_				, v_it) = 0.0f;
		mesh_.property(Kmin_				, v_it) = 0.0f;


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

		}		// end of boundary test

	}		// end of vertex iterator

	mesh_.remove_property(obtuse_);


}





//-----------------------------------------------------------------------------
template <class Mesh>
void SaliencyT<Mesh>::compute_saliency(float minscale, float maxscale)
{
	const int nScale = 5;
	const int nMax	 = 500;

	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	float sigma[nScale];
	int* knn = new int[nMax];
	int nn;


	sigma[0] = 2*epsilon;
	sigma[1] = 3*epsilon;
	sigma[2] = 4*epsilon;
	sigma[3] = 5*epsilon;
	sigma[4] = 6*epsilon;

	OpenMesh::VPropHandleT<Scalar>	temp;		//temp
	mesh_.add_property(temp);

	// main loop
	for(int i=minscale; i<maxscale; ++i){

		VertexHandle Mvh;	//max vertex handle(index)
		float M(-FLT_MAX);
		float m(0.f);
		int num_local_max(0);
		

		//i = 0;

		float s = sigma[i];
		float s2 = s*s;

		// 1. for each vertex

		for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){

			VertexHandle vh = v_it;
			int vID = vh.idx();
			Vec3f p = mesh_.point(vh);

			
			Scalar G_s(0.f), G_2s(0.f), n_factor(0.00001f), n_factor2(0.00001f);
			// find N within 2* (2-sigma)
			nn = ne->k_nearest_neighbor(vID, 4*s, nMax, knn);
			for(int j=0; j<nn; ++j){

				VertexHandle vh0 = mesh_.vertex_handle(knn[j]);
				Vec3f q = mesh_.point(vh0);
				Scalar kh = this->meancurvature(vh0);
				float dist = (p-q).norm();

				// compute G_2sigma
				float exp_weight = exp(-dist*dist/(8*s2));
				G_2s += kh * exp_weight;
				n_factor2 += exp_weight;

				// compute G_sigma
				if(dist < 2*s){
					exp_weight = pow(exp_weight,4.f);
					G_s += kh * exp_weight;
					n_factor += exp_weight;
				}
			}
			// Gaussian-weighted average(scale sigma)
			G_s /= n_factor;
			G_2s /= n_factor2;

			// saliency at p 
			mesh_.property(temp, vh) = fabs(G_s - G_2s);

			// check global max M
			if(M < mesh_.property(temp, vh)){
				M = mesh_.property(temp, vh);
				Mvh	= vh;
			}

		}// end for vertex (1st path)

		// 2. for each vertex
		for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){

			VertexHandle vh = v_it;
			Scalar temp_vh = mesh_.property(temp, vh);

			if(vh == Mvh) continue;

			int vID = vh.idx();
			// check local max
			ne->k_nearest_neighbor(vID, 10, knn);	//find 8-neighborhood
			bool local_max = true;
			

			for(int j = 0; j<10; ++j){

				VertexHandle vh0 = mesh_.vertex_handle(knn[j]);
				if( temp_vh <= mesh_.property(temp, vh0) ){
					local_max = false;
					break;
				}
			}

			if(local_max){
				num_local_max++;
				m = m + (temp_vh - m)/num_local_max;
			}

			mesh_.property(saliency_, vh) = temp_vh;
		}// end for vertex (2nd path)


		float f = (M-m)*(M-m);
		// 3. for each vertex
 		for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
 
 			VertexHandle vh = v_it;
 
 			//normalize
 			mesh_.property(temp, vh) /= M;
 			//globally multiplying (M-m)^2
 			mesh_.property(temp, vh) *= f;
 			// Final Saliency Value
 			mesh_.property(saliency_, vh) += mesh_.property(temp, vh);
 
 		}// end for vertex (3rd path)

	}// end for every scale


	mesh_.remove_property(temp);

	color_coding();
}


//-----------------------------------------------------------------------------
template <class Mesh>
void SaliencyT<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;	



	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		values.push_back(mesh_.property(saliency_, v_it));	// surface saliency
	}


	// discard upper and lower 5%
	unsigned int n = values.size()-1;
	unsigned int i = n / 20;
	std::sort(values.begin(), values.end());
	min_val = values[i];
	max_val = values[n-1-i];


	// define uniform color intervalls [v0,v1,v2,v3,v4]
	Mesh::Scalar v0, v1, v2, v3, v4;

 	v0 = min_val + 0.0/4.0 * (max_val - min_val);
 	v1 = min_val + 1.0/4.0 * (max_val - min_val);
 	v2 = min_val + 2.0/4.0 * (max_val - min_val);
 	v3 = min_val + 3.0/4.0 * (max_val - min_val);
 	v4 = min_val + 4.0/4.0 * (max_val - min_val);



	// map curvatures to colors
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		val = mesh_.property(saliency_, v_it);
		col = Mesh::Color(255,255,255);
		unsigned char u;

		if (val < v0) {	// v0
			col = Mesh::Color(0, 0, 255);
		}
		else if (val > v4) {
			col = Mesh::Color(255, 0, 0);
		}

		else if (val <= v2) {
			if (val <= v1) // [v0, v1]
			{
				u = (unsigned char) (255.0 * (val - v0) / (v1 - v0));
				col = Mesh::Color(0, u, 255);
			}      
			else // ]v1, v2]
			{
				u = (unsigned char) (255.0 * (val - v1) / (v2 - v1));
				col = Mesh::Color(0, 255, 255-u);
			}
		}
		else {
			if (val <= v3) // ]v2, v3]
			{
				u = (unsigned char) (255.0 * (val - v2) / (v3 - v2));
				col = Mesh::Color(u, 255, 0);
			}
			else // ]v3, v4]
			{
				u = (unsigned char) (255.0 * (val - v3) / (v4 - v3));
				col = Mesh::Color(255, 255-u, 0);
			}
		}

		mesh_.set_color(v_it, col);
	}

}

//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif