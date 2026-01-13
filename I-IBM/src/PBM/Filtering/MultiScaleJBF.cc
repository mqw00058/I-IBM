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
//   $Date: 2013.01.25 $
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

#ifndef PBM_FILTER_MSJBF_CC
#define PBM_FILTER_MSJBF_CC

//== INCLUDES =================================================================
#include "MultiScaleJBF.hh"

//== NAMESPACES ===============================================================


 namespace PBM {
 namespace Filter {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
MultiScaleJBFT<Mesh>::
MultiScaleJBFT(Mesh& _mesh)
: mesh_(_mesh)
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

//-----------------------------------------------------------------------------
template <class Mesh>
MultiScaleJBFT<Mesh>::
~MultiScaleJBFT()
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
void MultiScaleJBFT<Mesh>::initialize()
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

#define G(x,sigma) exp(-(x*x)/2*(sigma*sigma))

template <class Mesh>
void MultiScaleJBFT<Mesh>::CurvatureFlow(float *k_i)
{

	//const int SIZE_N=20;
	//ne->compute_normal(SIZE_N);

	float *sigma_n=new float [nPts];

	//Vec3f *du_dt= new Vec3f [nPts];


	for(int i=0; i<nPts; ++i)
	{
		Vec3f lambda=ne->eigenValue(i);
		sigma_n[i]=lambda[2]/(lambda[0]+lambda[1]+lambda[2]);

		
		//int knn[SIZE_N];
		//ne->k_nearest_neighbor(i,SIZE_N, knn);

		/////////////////////////////////////////////////////////////////////
			float *x=new float [nMax];
			float sum_x=0.0;
			Vec3f p,*u=new Vec3f[nMax];
			p=point_set[i];
			for(int j=0; j<nMax; j++)
			{
				//q = point_set[ knn[i*nMax + j] ];

				u[j] = point_set[knn[i*nMax+j]];
				//sum_x+=ne->kdt->disti(i,knn[i*nMax+j]);
				sum_x+=(p-u[j]).norm();

				
				//p[0]=ne->kdt->ptss[i].x[0];	p[0]=ne->kdt->ptss[i].x[1];	p[0]=ne->kdt->ptss[i].x[2];
				
				//u[j][0]=ne->kdt->ptss[].x[0];	u[j][1]=ne->kdt->ptss[knn[i*nMax+j]].x[1];	u[j][2]=ne->kdt->ptss[knn[i*nMax+j]].x[2];
			}

			float mean_x=sum_x/nMax;
			float sigma_x=0.0;
			for(int j=0; j<nMax; j++)
			{
				sigma_x+=(x[j]-mean_x);
			}
			sigma_x=sqrt(sigma_x/nMax);
		///////////////////////////////////////////////////////////////////


			int sum_G=0.0, sum_Gdot=0.0;
			for(int j=0; j<nMax; j++)
			{
				sum_Gdot+=G(x[j],sigma_x)*dot(ne->normal[i],Vec3f(p[0]-u[j][0],p[1]-u[j][1],p[2]-u[j][2]));
				sum_G+=G(x[j],sigma_x);
			}
			float Dire=sum_Gdot/sum_G;
			int sign=0;
			if(Dire>=0)
				sign=1;
			else
				sign=-1;

			
			
		k_i[i]=sign*sigma_n[i];
		
		mesh_.property(fweight_,mesh_.vertex_handle(i))=k_i[i];

		//du_dt[i]=Vec3f(-k_i[i]*ne->normal[i][0], -k_i[i]*ne->normal[i][1], -k_i[i]*ne->normal[i][2]);


		// update
		//mesh_.point(vh, p);
		delete [] x;
		delete [] u;
	}
	delete [] sigma_n;
	
	//delete [] du_dt;
}

template <class Mesh>
void MultiScaleJBFT<Mesh>::Gauss(vector<Vec3f> &pts)
{
	int size = pts.size();
	float *h_point = (float*)malloc(3*size*sizeof(float));
	//GeoTriMesh::ConstVertexIter  v_it(I_.vertices_begin()), v_end(I_.vertices_end());
	//GeoTriMesh::VertexVertexIter vv_it;


	int t_id=0;
	for (int i=0; i<size; ++i)
	{
		Vec3f p = pts[i];
		h_point[ t_id ] = p[0];
		h_point[ ++t_id ] = p[1];
		h_point[ ++t_id ] =p[2];
	}


	//Gaussian filtering
	const float sigma = ne->avgLen;
	const float sigma2 = sigma*sigma;


	//GeoTriMesh::ConstVertexIter v_it(mesh_.vertices_begin()), v_end(mesh_.vertices_end());
	Vec3f p, q, dp;
	VertexHandle vhp, vhq;
	float sqdist, w, sum_w;

	for(int i=0; i<size; ++i){

		//vhp = I_.vertex_handle(i);
		//p = I_.point(vhp);

		p=pts[i];
		dp.vectorize(0.f);
		sum_w = 0.f;

		for(int j=0; j< nMax; ++j){

			//vhq = I_.vertex_handle( knn[i*nMax + j] );		
		
			//for(vv_it=mesh_.vv_iter(v_it); vv_it; vv_it++){
			
			//q = I_.point(vhq);

			q= pts[knn[i*nMax+j]];
			sqdist = (p-q).sqrnorm();
			w = exp(-sqdist/(2*sigma2));
			sum_w += w;

			dp += w*q;
		}

		h_point[i*3]	= dp[0]/sum_w;
		h_point[i*3+1]	= dp[1]/sum_w;
		h_point[i*3+2]	= dp[2]/sum_w;
	}


	t_id=0;
	for (int i=0; i<size; ++i, ++t_id)
	{
		pts[i]= Vec3f( h_point[3*t_id+0],h_point[3*t_id+1],h_point[3*t_id+2] );	
	}

	free(h_point);
}

#define g(x,K) 1/(1+K*x*x)
template <class Mesh>
void MultiScaleJBFT<Mesh>::BalancedFlowFiltering(const int K/*parameter to contronl the diffusion speed*/, int N/* num of iteration*/, float s/*step length*/)
{

	float *k_H,*k_;		//curvature flow;
	vector<Vec3f> ini_point_set, v, v_;

	for(int i=0; i<point_set.size(); i++)
	{		
		ini_point_set.push_back(point_set[i]);
		v.push_back(point_set[i]);
		v_.push_back(point_set[i]);

	//	if(p[0]!=point_set[i][0] || p[1]!=point_set[i][1] || p[2]!=point_set[i][2])
	//		TRACE("%d\n",i);
	}
			
	for (int iter=0; iter<N; iter++)
	{
		Gauss(v_);
		
	/*	for(int i=0; i<v.size(); i++)
		{
			vi=I_.vertex_handle(i);
			Vec3f pt=I_.point(vi);
			v_.push_back(pt);
		}*/

		delete ne;
		ne = new NE(v_);
		ne->compute_normal(nMax);
		k_H=new float [nPts];
		CurvatureFlow(k_H);


		delete ne;
		ne = new NE(v);
		ne->compute_normal(nMax);
		k_=new float [nPts];
		CurvatureFlow(k_);

		//color_coding();

		GeoTriMesh::VertexHandle vh;
		for(int i=0; i<ini_point_set.size(); i++)
		{
			vh=mesh_.vertex_handle(i);
			Vec3f n=ne->normalVector(i);
			mesh_.set_color(vh, Mesh::Color(128+n[0]*128,128+n[1]*128,128+n[2]*128));
			//if(i==2680)
			//	i=2680;

			Vec3f vini_vi=(ini_point_set[i]-v[i]);

			Vec3f _kn=-k_[i]*(ne->normalVector(i));
			
			v[i]=v[i] + s*(g(k_H[i],K)*_kn  +  (1-g(k_H[i],K))*vini_vi );

		}
		delete [] k_H;
		delete [] k_;
		//v_.clear();

		for(int i=0; i<v.size(); i++)
			v_[i]=v[i];

	}
 
 	for(int i=0; i<v.size(); i++)
 	{
 		mesh_.set_point(mesh_.vertex_handle(i),v[i]);
	} 


}


template <class Mesh>
void MultiScaleJBFT<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{

		//values.push_back(mesh_.property(density_, v_it));	
		values.push_back(mesh_.property(fweight_, v_it));	// feature weight
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
		//val = mesh_.property(density_, v_it);	
		val = mesh_.property(fweight_, v_it);
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