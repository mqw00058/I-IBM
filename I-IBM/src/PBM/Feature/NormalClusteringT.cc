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
//   $Date: 2011.02.28 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file NormalClusteringT.cc
    
 */

//=============================================================================
//
//  CLASS NormalClusteringT - IMPLEMENTATION
//
//=============================================================================

#define PBM_NormalClusteringT_CC

//== INCLUDES =================================================================
#include "NormalClusteringT.hh"
#include "Clustering.h"
#include <math.h>
//== NAMESPACES ===============================================================


namespace PBM {
namespace Feature {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
NormalClusteringT<Mesh>::
NormalClusteringT(Mesh& _mesh)
  : mesh_(_mesh)
{
	mesh_.add_property(normal_);
	mesh_.add_property(eigenvalue_);
	mesh_.add_property(normal_variation_);
}

//-----------------------------------------------------------------------------


template <class Mesh>
NormalClusteringT<Mesh>::
~NormalClusteringT()
{
	mesh_.remove_property(normal_);
	mesh_.remove_property(eigenvalue_);
	mesh_.remove_property(normal_variation_);

	delete ne;
}

//-----------------------------------------------------------------------------
template <class Mesh>
void NormalClusteringT<Mesh>::initialize()
{
	// geometry property setting
        typename Mesh::ConstVertexIter	v_it, v_end(mesh_.vertices_end());
	

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
void NormalClusteringT<Mesh>::apply(int k_, double range_, double sparams_)
{
	k = k_;
	r = range_;
	sens_params = sparams_;
	
	// 0. Initialize
	initialize();
	// 1. normal estimation
	//normal_estimation();
	// 2. clustering
	GaussmapClustering();
	// 3. feature detection (classification)
	
	// 4. color coding (visualization):: optional?
	color_coding();
}

//-----------------------------------------------------------------------------
// estimate the normal vector of a point p using PCA of the covariance matrix
template <class Mesh>
void NormalClusteringT<Mesh>::normal_estimation()
{
	//construct point list
        typename Mesh::ConstVertexIter v_it, v_end(mesh_.vertices_end());
	vector<Vec3f> point_set;

	for( v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it){
		point_set.push_back(mesh_.point(v_it));
	}

	ne->compute_normal(k);	//parameter: the # of neighborhood		
	
	// set the estimated normal and eigenvalues
	for( v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it){
		VertexHandle vh = v_it.handle();
		mesh_.property(eigenvalue_, v_it)	= ne->eigenValue(vh.idx());
		mesh_.property(normal_, v_it)		= ne->normalVector(vh.idx());
	}

}

//-----------------------------------------------------------------------------
// implementation of Gauss map clustering [Weber10]
template <class Mesh>
void NormalClusteringT<Mesh>::GaussmapClustering()
{
	int *knn = new int[k+1];
	//ne->compute_normal(k);	//parameter: the # of neighborhood		

	vector<Vec3f> feature_candidate;

	float range = ne->avgLen * r * 1.6f;


	// 1. flatness test ==> remove feature candidate points
	for(unsigned int v_idx = 0; v_idx < point_set.size(); ++v_idx){
	
		if(v_idx == 809)
			v_idx = 809;


		// search the knn 
		int kk = ne->k_nearest_neighbor(v_idx, range, k, knn);
		//ne->k_nearest_neighbor(v_idx, k, knn);	// range=2.0!

		vector<Vec3f> gaussmap;		// all possible normal list
		vector<float> angle_list;	// the angle between normals in the gaussmap

		int num_normal = kk*(kk-1)/2;
		gaussmap.reserve(num_normal);
		angle_list.reserve(num_normal*(num_normal-1)/2);


		/* Clustering algorithm */
		AgglomerativeClustering AC(num_normal);

		// construct the Gauss map
		for(int i=0; i<kk; ++i){
			for(int j=i+1; j<kk; ++j){

				Vec3f ppi = point_set[ knn[i] ] - point_set[ v_idx ];
				Vec3f ppj = point_set[ knn[j] ] - point_set[ v_idx ];
				Vec3f nij = ppi%ppj;	//cross product
				Vec3f nji = ppj%ppi;
				nij = nij.normalize_cond();
				nji = nji.normalize_cond();
				Vec3f N = nij;

				if(N.norm() == 0){
					AC.num_clusters--;
					continue;
				}

				for(unsigned int ii=0; ii<gaussmap.size(); ++ii){

					float dot = max(gaussmap[ii] | nij, gaussmap[ii] | nji);
					N = (gaussmap[ii] | nij) > (gaussmap[ii] | nji) ? nij:nji;
					if(dot > 1.f) dot = 1.f;		// avoiding the floating point error
					float theta = acos(dot);		
					angle_list.push_back(theta);

					AC.set_distance_table(ii, gaussmap.size(), theta);
				} 
				gaussmap.push_back(N);
			}
		}

		AC.set_initial_cluster(gaussmap);

		// We have the list of the angles between every normal vectors, then compute 
		// a standard deviation (or variance) for median
		std::sort(angle_list.begin(), angle_list.end());
		float median_angle = angle_list[angle_list.size()/2];

		float sigma = 0.f;		// standard deviation
		for(unsigned int i=0; i<angle_list.size(); ++i){
			sigma += (angle_list[i]-median_angle)*(angle_list[i]-median_angle);
		}	
		sigma /= angle_list.size();		// variance
		sigma = sqrt(sigma);	// standard deviation

		VertexHandle vh = mesh_.vertex_handle(v_idx);
		mesh_.property(normal_variation_, vh) = sigma;


		// add the point into the feature candidate list
		// reduce search range for the Gauss map clustering
		const float T_FLAT = 0.3f;
		if(sigma < T_FLAT){		// flat region point
                        mesh_.set_color(vh, typename Mesh::Color(0, 0, 255));		// blue color for a flat point
			continue;
		} 
		
		// Now, this point is classified as a feature candidate.
		feature_candidate.push_back(point_set[v_idx]);

		// cluster ºÐ¼®!!!
		AC.do_clustering(sens_params);

		int num_clusters = 0;
		for(int i=0; i<AC.num_clusters; ++i){
			if(AC.clusters[i].num_elements < 3)
				continue;
			num_clusters++;
		}

		switch (num_clusters)
		{
		case 1:
			mesh_.status(vh).set_feature(false);
			break;

		case 2: case 3: case 4:
			mesh_.status(vh).set_feature(true);
			break;

		default:
			break;
		}
	}

	delete [] knn;




}

//-----------------------------------------------------------------------------
template <class Mesh>
void NormalClusteringT<Mesh>::color_coding()
{
        typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
        typename Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
        typename Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		values.push_back(mesh_.property(normal_variation_, v_it));


	// discard upper and lower 5%
	unsigned int n = values.size()-1;
	unsigned int i = n / 20;
	std::sort(values.begin(), values.end());
	min_val = values[i];
	max_val = values[n-1-i];


	// define uniform color intervalls [v0,v1,v2,v3,v4]
        typename Mesh::Scalar v0, v1, v2, v3, v4;
	v0 = min_val + 0.0/4.0 * (max_val - min_val);
	v1 = min_val + 1.0/4.0 * (max_val - min_val);
	v2 = min_val + 2.0/4.0 * (max_val - min_val);
	v3 = min_val + 3.0/4.0 * (max_val - min_val);
	v4 = min_val + 4.0/4.0 * (max_val - min_val);



	// map curvatures to colors
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		val = mesh_.property(normal_variation_, v_it);
                col = typename Mesh::Color(255,255,255);
		unsigned char u;

		if (val < v0) {
                        col = typename Mesh::Color(0, 0, 255);
		}
		else if (val > v4) {
                        col = typename Mesh::Color(255, 0, 0);
		}
		else if (val <= v2) {
			if (val <= v1) // [v0, v1]
			{
				u = (unsigned char) (255.0 * (val - v0) / (v1 - v0));
                                col = typename Mesh::Color(0, u, 255);
			}      
			else // ]v1, v2]
			{
				u = (unsigned char) (255.0 * (val - v1) / (v2 - v1));
                                col = typename Mesh::Color(0, 255, 255-u);
			}
		}
		else {
			if (val <= v3) // ]v2, v3]
			{
				u = (unsigned char) (255.0 * (val - v2) / (v3 - v2));
                                col = typename Mesh::Color(u, 255, 0);
			}
			else // ]v3, v4]
			{
				u = (unsigned char) (255.0 * (val - v3) / (v4 - v3));
                                col = typename Mesh::Color(255, 255-u, 0);
			}
		}

		mesh_.set_color(v_it, col);
	}


}

//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
