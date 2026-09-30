#ifndef IMPLICIT_SURFACE_RECONSTRUCTION_CC
#define IMPLICIT_SURFACE_RECONSTRUCTION_CC

#include "ImplicitSurfaceReconstruction.h"

namespace PBM {

	template <class Mesh>
	ImplicitSurfaceReconstruction<Mesh>::ImplicitSurfaceReconstruction(Mesh& _mesh)
		: mesh_(_mesh)7
	{
		mesh_.add_property(ev_);		// eigenvalue
		mesh_.add_property(e1_);		// eigenvector
		mesh_.add_property(e2_);		// eigenvector
		mesh_.add_property(e3_);		// eigenvector
		mesh_.add_property(feature_candidate_);	// surface saliency
		mesh_.add_property(scale_);		// surface(point) type
		mesh_.add_property(nfeature_);
		mesh_.add_property(fweight_);
		mesh_.add_property(min_fweight_);
		mesh_.add_property(density_);

		initialize(); 

	}

	template <class Mesh>
	ImplicitSurfaceReconstruction<Mesh>::~ImplicitSurfaceReconstruction(void)
	{
		mesh_.remove_property(ev_);		// eigenvalue
		mesh_.remove_property(e1_);		// eigenvector
		mesh_.remove_property(e2_);		// eigenvector
		mesh_.remove_property(e3_);		// eigenvector
		mesh_.remove_property(feature_candidate_);	// surface saliency
		mesh_.remove_property(scale_);
		mesh_.remove_property(nfeature_);
		mesh_.remove_property(fweight_);
		mesh_.remove_property(min_fweight_);
		mesh_.remove_property(density_);

		delete ne;

		delete [] knn;
		delete [] nn;
	}


	//-----------------------------------------------------------------------------
	template <class Mesh>
	void ImplicitSurfaceReconstruction<Mesh>::initialize()
	{
		// geometry property setting
		Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());

		for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){

			// initialize point set
			point_set.push_back(mesh_.point(v_it));		
		}	
		nPts	= (int)point_set.size();
		ne		= new NE(point_set);

		// for neighborhood
		nMax = 20;
		knn		= new int [nMax*nPts];
		nn		= new int [nPts];

		for(int i=0; i<nPts; ++i){
			// neighborhood computation
			//nn[i] = ne->k_nearest_neighbor(i, nMax, knn + i*nMax);
			ne->k_nearest_neighbor(i, nMax,  knn + i*nMax);
			nn[i] = nMax;
		}


	}


}
#endif