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
//   $Date: 2012.12.18 $
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

#ifndef OPENMESH_FILTER_BILATERALFILTER_CC
#define OPENMESH_FILTER_BILATERALFILTER_CC

//== INCLUDES =================================================================
#include "BilateralFilter.hh"
#include <pbm/Common/NormalEstimation.h>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Filter {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
BilateralFilterT<Mesh>::
	BilateralFilterT(Mesh& _mesh)
	: mesh_(_mesh)
{
	initialize();
}

//-----------------------------------------------------------------------------
template <class Mesh>
BilateralFilterT<Mesh>::
	~BilateralFilterT()
{
	
}

//-----------------------------------------------------------------------------
// Jones filter weight
template <class Mesh>
void BilateralFilterT<Mesh>::initialize()
{
	// geometry property setting
        typename Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
		point_set.push_back(mesh_.point(v_it));
	}	
	ne = new NE(point_set);
	
}


//-----------------------------------------------------------------------------
// Jones filter weight
template <class Mesh>
float BilateralFilterT<Mesh>::wt(Vec3f p1, Vec3f p2, float invsigma2)
{
	float d2 = invsigma2 * (p1-p2).sqrnorm();
	return (d2 >= truncate) ? 0.0f : exp(-0.5f*d2);
	//return (d2 >= 25.0f) ? 0.0f : exp(-0.5f*d2);
}


//-----------------------------------------------------------------------------
template <class Mesh>
void BilateralFilterT<Mesh>::Jones_filter(VertexHandle v_it, 
										  vector<unsigned> &flags, unsigned &flag_curr,
										  float invsigma2_1, float invsigma2_2, bool pass1, 
										  vector<Point> &mpoints)
{

	int vid = v_it.idx();
	const Point &p = pass1 ? mesh_.point(v_it) : mpoints[vid];	//normal computation
	Point flt(0.f, 0.f, 0.f);	//filtered point
	
	float sum_w = 0.0f;

	flag_curr++;

        typename Mesh::VertexFaceIter vf_it;

	vector<unsigned> support;
	// 1-ring
	for(vf_it = mesh_.vf_iter(v_it);vf_it; ++vf_it)
	{
		int f = (vf_it.handle()).idx();
		support.push_back(f);
	}


	// for each supporting face
	while (!support.empty())
	{
		unsigned int f = support.back();
		support.pop_back();
		if (flags[f] == flag_curr)
			continue;
		flags[f] = flag_curr;

		int v0, v1, v2;
		Point p0, p1, p2, c;

		FaceHandle fh = mesh_.face_handle(f);
                typename Mesh::ConstFaceVertexIter cfvIt = mesh_.cfv_iter(fh);
		p0 = mesh_.point(cfvIt.handle());
		v0 = cfvIt.handle().idx();
		p1 = mesh_.point((++cfvIt).handle());
		v1 = cfvIt.handle().idx();
		p2 = mesh_.point((++cfvIt).handle());
		v2 = cfvIt.handle().idx();
		c = (p0 + p1 + p2) * (1.0f / 3.0f);


		// spatial weight
		float w = wt(p, c, invsigma2_1);
		if (w == 0.0f)
			continue;
		
		//w *= 0.5f * ((p1-p0)%(p2-p0)).norm();	//area-weight

		if (pass1) {		// mollification
			flt += w * c;
			sum_w += w;
		} else {

			Vec3f fn = (mpoints[v1]-mpoints[v0])%(mpoints[v2]-mpoints[v0]);
			fn.normalize();
			Point prediction = p - fn * ((p-c)|fn);
			w *= wt(p, prediction, invsigma2_2);		//influence weight
			if (w == 0.0f)
				continue;
			flt += w * prediction;
			sum_w += w;

		}

		// for adjacent faces of a face
                typename Mesh::FaceFaceIter ff_it;

		for(ff_it = mesh_.ff_iter(fh);ff_it;++ff_it){

			int ae = ff_it.handle().idx();
			if (ae < 0 || flags[ae] == flag_curr)
				continue;

			support.push_back(ae);
		}

	}

	if (sum_w == 0.0f)
		flt = p;
	else
		flt *= 1.0f / sum_w;

	if(pass1){
		mpoints[vid] = flt;
	} else{
		mesh_.set_point(v_it, flt);
	}


}

//-----------------------------------------------------------------------------
template <class Mesh>
void BilateralFilterT<Mesh>::Jones_bFilter()
{
	int nv = mesh_.n_vertices(), nf = mesh_.n_faces();
	vector<Point> mpoints(nv);	// vertex normal

	float sigma3 = 0.5f * sigma1;
	float invsigma2_1 = 1.0f / (sigma1*sigma1);
	float invsigma2_2 = 1.0f / (sigma2*sigma2);
	float invsigma2_3 = 1.0f / (sigma3*sigma3);

	truncate = 4*sigma1*sigma1;


        typename Mesh::ConstVertexIter	v_it, v_end(mesh_.vertices_end());

	vector<unsigned> flags(nf);
	unsigned flag_curr = 0;

	// step 1. mollification (Gaussian smoothing)
	for(v_it=mesh_.vertices_begin(); v_it != v_end; ++v_it) {
		Jones_filter(v_it, flags, flag_curr, invsigma2_3, 0.0f, true, mpoints);
	}
	// step 2. bilateral ()
	for(v_it=mesh_.vertices_begin(); v_it != v_end; ++v_it) {
		Jones_filter(v_it, flags, flag_curr, invsigma2_1, invsigma2_2, false, mpoints);
	}


}


//-----------------------------------------------------------------------------
template <class Mesh>
void BilateralFilterT<Mesh>::Fleishman_filter(VertexHandle vh)
{
	const int nMax	 = 8;
	int* knn = new int[nMax];
	int vID = vh.idx();
	int nn = ne->k_nearest_neighbor(vID, 2*sigma1, nMax, knn);
	float invsigma2_1;
	float invsigma2_2;
	
//        VertexHandle vh;

//	mesh_.face_handle

	// 1. compute the vertex normal
	Point p = mesh_.point(vh);
	Vec3f n(0.f,0.f,0.f);
        typename Mesh::ConstVertexFaceIter vf_it;

	for(vf_it = mesh_.cvf_iter(vh); vf_it; ++vf_it)
	{
		Point p0, p1, p2;
                typename Mesh::ConstFaceVertexIter cfvIt = mesh_.cfv_iter(vf_it.handle());
		p0 = mesh_.point(cfvIt.handle());
		p1 = mesh_.point((++cfvIt).handle());
		p2 = mesh_.point((++cfvIt).handle());
		
		n +=  mesh_.normal(vf_it.handle()) * 0.5f * ((p1-p0)%(p2-p0)).norm();	
	}
	n.normalize();					
	//n = ne->normalVector(vID);	//point geometry

	//2. compute the sigma2 (statistics of height)
	float sum_w(0.f);
	float normalizer(0.000001f);
	float u(0.f);
	float st(0.000001f);

	// standard dev. of the offsets in the selected neighborhood.
	for (int i=0; i<nn; ++i)
	{
		Point q = mesh_.point( mesh_.vertex_handle( knn[i] ) );
		float h = (n|(p-q));
		float delta = h-u;
		u = u + delta/(i+1);
		st = st + delta*(h-u);
	}
	st = sqrt(st/nn);

	invsigma2_1 = 1.f / (sigma1*sigma1);
	invsigma2_2 = 1.f / (st*st);

	//3. per point filtering

	for (int i=0; i<nn; ++i)
	{
		Point q = mesh_.point( mesh_.vertex_handle( knn[i] ) );
		float t = (p-q).norm();
		float h = (n|(p-q));
		float wc = exp(-0.5f * t*t * invsigma2_1);
		float ws = exp(-0.5f * h*h * invsigma2_2);
		sum_w += wc*ws*h;
		normalizer += wc*ws;

	}

	float d = sum_w/normalizer;
	point_set[vID] = p - n * d;

	// anti-shrinkage

	delete[] knn;
}


//-----------------------------------------------------------------------------
template <class Mesh>
void BilateralFilterT<Mesh>::Fleishman_bFilter(int iter)
{
        typename Mesh::ConstVertexIter	v_it, v_end(mesh_.vertices_end());
	VertexHandle	vh, vhq;
	Point			p, q;
	Vec3f			dp, n;
	float			wc, ws, t, h, sum_w, normalizer, d;
	
	
	int				nPts = mesh_.n_vertices();
	int				t_id = 0;
	
	const int nMax = 10;
	int *neighbor_table = new int[nMax*nPts];
	int *nofN = new int[nPts];

	//kernel std
	float sigma_c = sigma1;
	float sigma2_c = sigma_c*sigma_c;
	float sigma_s = 0.01f;
	float sigma2_s = sigma_s*sigma_s;

	for(int i=0; i<nPts; ++i)
	{
		nofN[i] = ne->k_nearest_neighbor(i, sigma_c, nMax, 
			neighbor_table + i*nMax);
		//nofN[i] = nMax;
 		//ne->k_nearest_neighbor(i, nMax, neighbor_table + i*nMax);

	}

//	float u		= 0.f;
//	float st	= 0.000001f;

	for(int it=0; it<iter; ++it)
	{
		for(int i=0; i<nPts; ++i){

			vh = mesh_.vertex_handle(i);
			p = mesh_.point(vh);
			n = mesh_.normal(vh);
			sum_w = 0.f;
			normalizer = 0.00001f;
			
			//// standard dev. of the offsets in the selected neighborhood.
		/*	for(int j=0; j< nofN[i]; ++j)
			{
				vhq = mesh_.vertex_handle( neighbor_table[i*nMax + j] );
				q = mesh_.point(vhq);
				h = (n|(p-q));
				delta = h-u;
				u = u + delta/(j+1);
				st = st + delta*(h-u);
			}
			st = sqrt(st/ nofN[i]);
			sigma2_s = 1/(st*st);*/

			for(int j=0; j< nofN[i]; ++j){

				vhq = mesh_.vertex_handle( neighbor_table[i*nMax + j] );
				q = mesh_.point(vhq);
				t = (p-q).norm();
				h = (n|q-p);
				wc = exp(-t*t/(2*sigma2_c));
				ws = exp(-h*h/(2*sigma2_s));
				sum_w += wc*ws*h;
				normalizer += wc*ws;
			}
			d = sum_w/normalizer;
			point_set[i] = p + n * d;
		}

		t_id=0;
		for (v_it = mesh_.vertices_begin(); v_it!=v_end; ++v_it, ++t_id)
		{
			mesh_.set_point( v_it, point_set[t_id]);
		}

		//mpoint_set = point_set;

		// 2. Anti-shrinkage
		//for(int i=0; i<nPts; ++i){

		//	vh = mesh_.vertex_handle(i);
		//	nn = ne->k_nearest_neighbor(i, 2*sigma1, nMax, knn);
		//	dp = (mesh_.point(vh) - point_set[i])/nn;

		//	// volume compensation
		//	for(int j=0; j<nn; ++j){
		//		mpoint_set[ knn[j] ]  = mpoint_set[knn[j]] + dp;
		//	}
		//}
		// final position update
		/*for(int i=0; i<nPts; ++i){
			vh = mesh_.vertex_handle(i);
			mesh_.set_point(vh, point_set[i]);
		}*/

		//for (v_it = mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		//{
		//	Fleishman_filter(v_it);
		//}

		//mpoint_set = point_set;


		//// volume-preservation
		//for (v_it = mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		//{
		//	vh = v_it.handle();
		//	vID = vh.idx();
		//	nn = ne->k_nearest_neighbor(vID, 2*sigma1, nMax, knn);
		//	dp = (mesh_.point(v_it) - point_set[i])/nn;
		//	for(int j=0; j<nn; ++j){
		//		mpoint_set[ knn[j] ]  = mpoint_set[knn[j]] + dp;
		//	}
		//}

		
		
	}
	delete[] neighbor_table;
	delete[] nofN;

	
}




template <class Mesh>
void BilateralFilterT<Mesh>::apply(int iter_, float sigma1_, float sigma2_ )
{
	iter = iter_;
	sigma1 = sigma1_;
	sigma2 = sigma2_;


	if(mode == JONES){
		Jones_bFilter();
	} else if(mode == FLEISHMAN) {
		Fleishman_bFilter(iter_);
	}


}




//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif
