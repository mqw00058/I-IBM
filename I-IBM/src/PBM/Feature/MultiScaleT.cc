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

#ifndef PBM_FEATURE_MULTISCALET_CC
#define PBM_FEATURE_MULTISCALET_CC

//== INCLUDES =================================================================
#include "MultiScaleT.hh"


//== NAMESPACES ===============================================================


namespace PBM {
namespace Feature {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
MultiScaleT<Mesh>::
	MultiScaleT(Mesh& _mesh)
	: mesh_(_mesh)
{
	mesh_.add_property(surface_variation_);
	mesh_.add_property(curvedness_);
	mesh_.add_property(residual_);
	mesh_.add_property(multiscale_measure_);

	initialize();
}

//-----------------------------------------------------------------------------


template <class Mesh>
MultiScaleT<Mesh>::
	~MultiScaleT()
{
	mesh_.remove_property(surface_variation_);
	mesh_.remove_property(curvedness_);
	mesh_.remove_property(residual_);
	mesh_.remove_property(multiscale_measure_);
}





//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::initialize()
{

	// geometry property setting
	Mesh::ConstVertexIter	v_it, v_end(mesh_.vertices_end());

	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
		//feature status
		mesh_.status(v_it).set_feature(false);
		//construct point list
		point_set.push_back(mesh_.point(v_it));
	}
	//normal estimation class
	ne = new NormalEstimation(point_set);

	// the # of feature points
	n_features = 0;


}

//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::compute_multiscale_surfvar(int k_min, int k_max)
{
	// the minimum scale is 3 (see constructor)
	for(int scale=k_min; scale<=k_max; ++scale){

		// compute the eigenvalues of the covariance matrix
		ne->compute_normal(scale);
		
		// eigenvalues (lamda)
		float l0, l1, l2;

		for(int i=0; i<(int)point_set.size(); ++i){
			Vec3f ev = ne->eigenValue(i);
			l0 = ev[0];	// largest
			l1 = ev[1];
			l2 = ev[2];	// smallest

			float sigma_n = l2 / (l0+l1+l2);
			mesh_.property(surface_variation_, mesh_.vertex_handle(i)).push_back(sigma_n);
		}
	}
}

//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::compute_multiscale_curvedeness(int k_min, int k_max)
{
}


//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::color_coding(int k)
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		values.push_back(surface_variation(v_it, k));


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
		val = surface_variation(v_it, k);
		col = Mesh::Color(255,255,255);
		unsigned char u;

		if (val < v0) {
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


//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::extract_features_surfvar()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	// some parameters
	float ratio = 0.6f;
	float S_MAX = 0.05f;
	int W_MAX = (max_scale-min_scale) * ratio;


	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){

		int weight=0;
		for(int s=min_scale; s<=max_scale; ++s){

			float sigma = surface_variation(v_it, s);
			if(sigma > S_MAX) 
				weight++;
		}

		if(weight > W_MAX){
			mesh_.status(v_it).set_feature(true);
			n_features++;
		} else
			mesh_.status(v_it).set_feature(false);
	}

	// feature color coding
	feature_color_coding();

}

//-----------------------------------------------------------------------------
template <class Mesh>
void MultiScaleT<Mesh>::feature_color_coding()
{

	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Color       col;

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		bool isfeature = mesh_.status(v_it).feature();

		if (isfeature) {
			col = Mesh::Color(255, 0, 0);
		}
		else {
			col = Mesh::Color(0, 0, 255);
		}
		mesh_.set_color(v_it, col);
	}
}

	//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif