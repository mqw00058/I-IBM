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
//   $Date: 2011.05.02 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file TensorVoting.cc

*/

//=============================================================================
//
//  CLASS TensorVotingT - IMPLEMENTATION
//
//=============================================================================

#ifndef PBM_FEATURE_TENSORVOTING_CC
#define PBM_FEATURE_TENSORVOTING_CC

//== INCLUDES =================================================================
#include "TensorVoting.hh"
#include "Clustering.h"




//== NAMESPACES ===============================================================
namespace PBM {
namespace Feature {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
TensorVotingT<Mesh>::TensorVotingT(Mesh& _mesh)
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

	initialize();

}

//-----------------------------------------------------------------------------
template <class Mesh>
TensorVotingT<Mesh>::~TensorVotingT()
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

}

//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::initialize()
{
	// geometry property setting
	Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());

	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
		//feature status
		mesh_.status(v_it).set_feature(false);
		//construct point list
		point_set.push_back(mesh_.point(v_it));
		mesh_.property(feature_candidate_, v_it) = true;

		mesh_.property(fweight_, v_it)	= 2.f;
	}

	ne = new NormalEstimation(point_set);

	//identity matrix
	matI = new double* [3];
	// Initialize matrix
	for(int i=0; i<3; ++i) {
		matI[i] = new double [3];
		for(int j=0; j<3; ++j) {
			matI[i][j] = (i==j)?1:0;
		}
	}

	nPts = (int)point_set.size();

}

//------------------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::multiscale_tensorvoting(int minScale, int maxScale, float min_w, float max_w)
{


	Vec3f pi,pj,nj;
	double **matT,**matV; 		// matT: voting tensor (3x3), matV: ball votes
	matT = new double* [3];
	matV = new double* [3];
	// Initialize matrix
	for(int i=0; i<3; ++i) {
		matT[i] = new double [3];
		matV[i] = new double [3];
	}



	// feature candidate

	feature_queue.clear();	// reset feature_candidate queue
	for(int i=0; i<nPts; ++i){
		feature_queue.push_back(i);
	}


	//---------------MULTI-SCALE Tensor Voting---------------//
	// scale loop (k:#of neighborhood)
	// scale loop (r:maximum range of neighborhood)
	float avgLen = ne->avgLen;
	//int ring(3);
	int ring;

	if(minScale < 10) ring=1;
	else if(minScale < 30) ring=2;
	else if(minScale < 50) ring=3;
	else if(minScale < 81) ring=4;
	else if(minScale < 121) ring=5;


	for(int k = minScale; k<maxScale; ++ring, k+= (ring*6)){

		// initialize
		float sigma2 = avgLen*k;
		//float range = avgLen * 1.6f * ring;	//iter가 결국 ring
		float range = avgLen * 2.5f * ring;	//iter가 결국 ring
		//int* knn = new int [200];
		int* knn = new int [k+1];

		for(vector<int>::const_iterator it=feature_queue.begin(); it!=feature_queue.end(); ){

			int i = *it; 

			VertexHandle vh = mesh_.vertex_handle(i);

			// multi-scale tensor voting process


			// find the k-nearest neighborhood of p 
			//ne->k_nearest_neighbor(i, k, knn);
			int kk = ne->k_nearest_neighbor(i, range, k, knn);
			//int kk = ne->Delaunay_neighborhood(i, 2*range, k+1, knn);
			//int kk = ne->neighborhood_selection(i, 2*range, 1, knn);

			for(int ii=0; ii<3; ++ii) {
				for(int jj=0; jj<3; ++jj) {
					matT[ii][jj] = 0;
				}
			}

			pi = point_set[i];



			// 2-ring iteration 돌아야 함
			//for(Mesh::VertexOHalfedgeIter voh_it=mesh_.voh_iter(vh); voh_it; ++voh_it){
			// vote integration
			//	HalfedgeHandle hh = voh_it.handle();
			//	pj = mesh_.point(mesh_.to_vertex_handle(hh));


			for(int j=0; j<kk; ++j){

				pj = point_set[ knn[j] ];

				// compute the ball vote
				Vec3f v = pi-pj;
				float l = v.norm();

				v = v/l;
				float len = exp(-(l*l/sigma2));	// vote length
				float vv;
				float vvnorm=v.sqrnorm();

				// I - len*(v*v_transpose)
				for(int ii=0; ii<3;++ii){
					for(int jj=0; jj<3;++jj){

						vv = v[ii]*v[jj];
						matV[ii][jj] = len *  (matI[ii][jj] - vv/vvnorm);
					}
				}

				// integrating the ball votes
				matT[0][0] += matV[0][0];matT[0][1] += matV[0][1];matT[0][2] += matV[0][2];
				matT[1][0] += matV[1][0];matT[1][1] += matV[1][1];matT[1][2] += matV[1][2];
				matT[2][0] += matV[2][0];matT[2][1] += matV[2][1];matT[2][2] += matV[2][2];

			}

			// Eigen-decomposition for analysis
			EigenSolver A;
			A.SolveEigenSystem(3, matT);
			A.SortEigenvectors();
			float ev1, ev2, ev3;	// eigenvalues
			Vec3f e1, e2, e3;		// eigenvectors

			ev1 = A.eigenValue_[0];
			ev2 = A.eigenValue_[1];
			ev3 = A.eigenValue_[2];

			mesh_.property(ev_, vh)[0] = ev1;
			mesh_.property(ev_, vh)[1] = ev2;
			mesh_.property(ev_, vh)[2] = ev3;

			e1[0] = A.matV_[0][0]; e1[1] = A.matV_[1][0]; e1[2] = A.matV_[2][0];
			e2[0] = A.matV_[0][1]; e2[1] = A.matV_[1][1]; e2[2] = A.matV_[2][1];
			e3[0] = A.matV_[0][2]; e3[1] = A.matV_[1][2]; e3[2] = A.matV_[2][2];

			mesh_.property(e1_, vh) = e1;
			mesh_.property(e2_, vh) = e2;
			mesh_.property(e3_, vh) = e3;

			float ss = ev1-ev2;
			float sc = ev2-ev3;
			float sp = ev3;




			//-------------------------------------------------------------
			// scale 및 weight 분석
			float weight_ = (ev2+ev3)/ev1;
			//weight_ = (ev2-ev3);		// curve

			float prev_weight_ = mesh_.property(fweight_, vh);
			mesh_.property(fweight_, vh) = weight_;		//weight 값을 저장
			//scale 저장
			mesh_.property(scale_, vh) = k;

			// initial weight 값 저장
			if(ring ==1){
				mesh_.property(min_fweight_, vh) = weight_;		
			}


			// MS-TV
			if(weight_ <= min_w){
				mesh_.status(vh).set_feature(false);
				it = feature_queue.erase(it);				//queue에서 제거
				mesh_.property(feature_candidate_, vh) = false;

			}
			else if(weight_ >= max_w){
				mesh_.status(vh).set_feature(true);
				it = feature_queue.erase(it);				//queue에서 제거
				mesh_.property(feature_candidate_, vh) = false;
			}
			else if(ring> 1 && (weight_ > prev_weight_ + 0.05)){	// 증가하는 non-feature의 scale을 제한한다.
				mesh_.property(fweight_, vh) = prev_weight_;		//이전 weight 값을 저장
				mesh_.status(vh).set_feature(false);
				it = feature_queue.erase(it);						//queue에서 제거
			}
			// 감소하거나 변화가 없는 weight는 그대로 진행시킨다.
			else{
				it++;
			}

		} // end of point loop
		delete[] knn;
	} // end of scale loop

	// feature queue에 남아있는 점들을 대상으로만 optimal scale을 찾기 위한 뭔가를 한다.

	delete[] matT;
	delete[] matV;
}

//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::feature_classification()
{

	int k = 8;
	int* knn = new int [k];
	int j=0;


	// geometry property setting
	Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	for(int i=0; i<nPts; ++i){

		VertexHandle vh = mesh_.vertex_handle(i);

		// 아직 결정이 되지 않은 점들을 대상으로
		if(mesh_.property(feature_candidate_, vh)){

			float weight = mesh_.property(fweight_, vh);
			float w0 = mesh_.property(min_fweight_, vh);
			int nf=0;
			int nwf=0;

			// 1. scale이 점차 감소한 점들 ==> non-feature
			if(w0 - weight > 0.2){
				mesh_.status(vh).set_feature(false);
				mesh_.property(feature_candidate_, vh) = false;
			}

			else{
				ne->k_nearest_neighbor(i, k, knn);
				// 2. scale이 점차 증가한 점들 ==> feature or vicinity 
				//ne->k_nearest_neighbor(i, k, knn);
				// for k-neighborhood
				float mean(0.f);
				float std(0.f);
				float M2(0.f);
				float minvalue(100.f);


				//	for(Mesh::VertexOHalfedgeIter voh_it=mesh_.voh_iter(vh); voh_it; ++voh_it, ++j){
				// vote integration
				//		HalfedgeHandle hh = voh_it.handle();
				//		VertexHandle vhj = mesh_.to_vertex_handle(hh);

				// compute mean and standard deviation using on-line algorithm
				for(j=0; j<k; ++j){

					VertexHandle vhj = mesh_.vertex_handle( knn[j] );
					float wj = mesh_.property(fweight_, vhj);

					float delta = wj-mean;
					mean += delta/(j+1);
					M2 += delta * (wj-mean);

					if(wj < minvalue)
						minvalue = wj;


					//if(wj > weight )
					if((!mesh_.property(feature_candidate_, vhj)) && mesh_.status(vhj).feature() )
						++nf;
					if(wj > weight )
						++nwf;
				}

				std = sqrt(M2/j);

				float k = 0.5f;
				float R = 0.4f;
				//float t = mean * (1 + k*(std/R -1));	// adaptive threshold (Sauvola's method)
				//float t = (1-k)*mean + k*minvalue + k*std/R*(mean-minvalue);	// Christian method
				float t = mean;

				//mean과 variance를 함께 봅시다.

				if( t > weight)
					mesh_.status(vh).set_feature(false);
				else if( t < weight){

					// 8-neighbor중에 2개까지 허용(weight가 local maximum인가)
					if(nf > 0)
						mesh_.status(vh).set_feature(false);
					if(nwf >= 3)
						mesh_.status(vh).set_feature(false);
					else
						mesh_.status(vh).set_feature(true);
				}
			}

		}
	}

	delete[] knn;
}

//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::feature_completion()
{

	int k = 8;
	int* knn = new int [k];
	int nf = 0;
	int avg_weight=0;



	// 모든 feature 들의 neighborhood 개수 계산
	for(int i=0; i<nPts; ++i ){

		VertexHandle vh = mesh_.vertex_handle(i);
		ne->k_nearest_neighbor(i, k, knn);
		avg_weight += mesh_.property(fweight_, vh);

		if(mesh_.status(vh).feature()){	
			int n_features(0);
			for(int j=0; j<k; ++j){
				VertexHandle vhj = mesh_.vertex_handle( knn[j] );
				if(mesh_.status(vhj).feature()){
					n_features++;
				}
			}
			mesh_.property(nfeature_, vh) = n_features;
		}
	}
	avg_weight /= nPts;



	// island 제거

	for(int i=0; i<nPts; ++i ){

		VertexHandle vh = mesh_.vertex_handle(i);
		ne->k_nearest_neighbor(i, k, knn);

		//1. feature 로 판명된 outlier 제거
		if(mesh_.status(vh).feature()){			

			nf = mesh_.property(nfeature_, vh);
			int w = mesh_.property(fweight_, vh);

			//if(w < avg_weight){
			//	mesh_.status(vh).set_feature(false);
			//	continue;
			//}

			//isolated point
			if(nf ==0)		
				mesh_.status(vh).set_feature(false);
			// end point
			else if(nf == 1){

				ne->k_nearest_neighbor(i, k, knn);
				for(int j=0; j<k; ++j){
					VertexHandle vhj = mesh_.vertex_handle( knn[j] );
					if(mesh_.status(vhj).feature()){
						int nnf =  mesh_.property(nfeature_, vhj);

						if(nnf < 2){
							mesh_.status(vh).set_feature(false);
							mesh_.status(vhj).set_feature(false);
						}
						break;
					}
				}

			}




		}
		//2. feature로 분류되지 않은 gap 보완
		//else{
		//
		//		}

	} // end of point

	delete[] knn;
}


//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::feature_connection()
{
	int k = 8;
	EdgeHandle eh;
	int* knn = new int [k];


	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());	
	typename Mesh::VertexOHalfedgeIter	voh_it;	
	typename Mesh::EdgeIter				e_it, e_end(mesh_.edges_end());

	Vec3f _e3, _edge;
	VertexHandle _vh1;
	Vec3f _v0, _v1;
	EdgeHandle _eh;

	//초기화
	for(int i=0; i<nPts; ++i ){

		VertexHandle vh = mesh_.vertex_handle(i);
		mesh_.property(nfeature_, vh) = 0.f;
	}



	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		if(mesh_.status(v_it).feature())
		{
			_e3 = mesh_.property(e3_, v_it);
			for (voh_it=mesh_.voh_iter( v_it ); voh_it; ++voh_it)
			{
				_v0 = mesh_.point(v_it);
				_vh1 = mesh_.to_vertex_handle(voh_it); 
				_v1 = (mesh_.point(_vh1));

				if(mesh_.status(_vh1).feature())
				{
					_edge = (_v1-_v0)/(_v1-_v0).norm();
					_eh = mesh_.edge_handle(voh_it);

					if(dot(_e3, _edge) >= 0.7 ){
						mesh_.status(_eh).set_feature(true);
						++mesh_.property(nfeature_, v_it);	//연결된 edge의 개수
						++mesh_.property(nfeature_, _vh1);	//연결된 edge의 개수
					}
				}
			}	
		}
	}


	return;

	// pruning
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		if(mesh_.status(v_it).feature())
		{


			if(mesh_.property(nfeature_, v_it) == 0){	// outlier
				mesh_.status(v_it).set_feature(false);

				for (voh_it=mesh_.voh_iter( v_it ); voh_it; ++voh_it)
				{
					_vh1 = mesh_.to_vertex_handle(voh_it); 
					_eh = mesh_.edge_handle(voh_it);
					mesh_.status(_eh).set_feature(false);
				}
			}
			// 
			// 			if(mesh_.property(nfeature_, v_it) == 1){	// end point
			// 
			// 				for (voh_it=mesh_.voh_iter( v_it ); voh_it; ++voh_it)
			// 				{			// 
			// 					_vh1 = mesh_.to_vertex_handle(voh_it); 
			// 					_eh = mesh_.edge_handle(voh_it);
			// 					if(mesh_.status(_vh1).feature() && mesh_.property(nfeature_, _vh1) == 0 )
			//					{
			// 						mesh_.status(v_it).set_feature(false);
			// 						mesh_.status(_vh1).set_feature(false);
			// 						mesh_.status(_eh).set_feature(false);
			// 					}			// 				}
			// 			}

		}
	}



}


//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{

		values.push_back(mesh_.property(fweight_, v_it));	// surface saliency
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


//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::KMeansClustering(unsigned int num_of_clusters, unsigned int maximum_num_of_iterations, Scalar epsilon_)
{
	typename Mesh::VertexIter        v_it, v_end(mesh_.vertices_end());

	std::vector< vector_t > temp;

	vector_t vector (3);			// (e1, e2) pair
	vector_t temp_vector (3);

	int i;


	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		// cluster 만들기 (어떤 값을 기준으로 분류할 것인가)
		vector[0] = mesh_.property(saliency_, v_it)[0];
		vector[1] = mesh_.property(saliency_, v_it)[1];
		vector[2] = mesh_.property(saliency_, v_it)[2];

		//map
		vectors.insert( std::make_pair(0, vector) );
		temp.push_back(vector);
	}




	//----------------modified-----------------//
	// initial guess by choosing the extreme value
	// saliency 값(s1이 가장 큰 값 vs. s2가 가장 큰 값)

	// 	int maxS1_idx, minS1_idx;
	// 	float maxS1 = -1.f, minS1 = 10.f;
	// 
	// 	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	// 	{
	// 		if(maxS1 < mesh_.property(saliency_, v_it)[0]){
	// 			maxS1 = mesh_.property(saliency_, v_it)[0];
	// 			VertexHandle vh = v_it;
	// 			maxS1_idx = vh.idx();
	// 		}
	// 		if(minS1 > mesh_.property(saliency_, v_it)[0]){
	// 			minS1 = mesh_.property(saliency_, v_it)[0];
	// 			VertexHandle vh = v_it;
	// 			minS1_idx = vh.idx();
	// 		}
	// 	}

	//	centers.insert( std::make_pair(0, temp[maxS1_idx]) );
	//	centers.insert( std::make_pair(1, temp[minS1_idx]) );

	// Shuffling for initial guess
	std::random_shuffle( temp.begin(), temp.end() );
	// Assign initail centers
	for(int i = 0; i<num_of_clusters; ++i)
	{
		centers.insert( std::make_pair(i, temp[i]) );
	}

	// clustering
	bool	ret = true;

	for(i=0; ret && i<maximum_num_of_iterations; ++i )
	{
		clustering(num_of_clusters);
		ret = update(epsilon_);
	}

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		vector[0] = mesh_.property(saliency_, v_it)[0];
		vector[1] = mesh_.property(saliency_, v_it)[1];
		vector[2] = mesh_.property(saliency_, v_it)[2];

		double dist = FLT_MAX;
		double temp_dist;

		typename vectors_t::const_iterator cit; 

		for(i=0, cit = centers.begin(); cit!=centers.end(); ++cit, ++i )
		{
			temp_vector[0] = cit->second[0];
			temp_vector[1] = cit->second[1];
			temp_vector[2] = cit->second[2];
			temp_dist = fabs(vector[0] - temp_vector[0])*fabs(vector[0] - temp_vector[0])
				+ fabs(vector[1] - temp_vector[1])*fabs(vector[1] - temp_vector[1])
				+ fabs(vector[2] - temp_vector[2])*fabs(vector[2] - temp_vector[2]);

			if(temp_dist < dist)
			{
				dist = temp_dist;
				mesh_.property(vclusterid_, v_it) = i;
			}

		}

	}


	temp.clear();
}




//-----------------------------------------------------------------------------
template <class Mesh>
void TensorVotingT<Mesh>::clustering(unsigned int num_of_clusters)
{
	vector_t distances(num_of_clusters);
	vectors_t temp_vectors;

	// For all vectors
	for(typename vectors_t::const_iterator it = vectors.begin(); it!=vectors.end(); ++it)
	{
		// Calculate the distance to center 
		size_t i(0);
		for( typename vectors_t::const_iterator cit = centers.begin(); cit!=centers.end(); ++cit, ++i )
		{
			distances[i] = euclidean_sqdistance( cit->second, it->second );
		}

		// Assign nearest center
		temp_vectors.insert(
			std::make_pair(
			std::distance<typename vector_t::value_type*>( 
			&distances[0], 
			std::min_element( &distances[0], &distances[0] + distances.size() )
			),
			it->second
			)
			);
	}

	vectors = temp_vectors;
}

//-----------------------------------------------------------------------------
template <class Mesh>
bool TensorVotingT<Mesh>::update(Scalar epsilon)
{
	bool ret = false;

	for(typename vectors_t::iterator it=centers.begin(); it!=centers.end(); ++it)
	{
		std::pair<typename vectors_t::const_iterator, typename vectors_t::const_iterator> 
			range = vectors.equal_range(it->first);

		size_t num_of_vectors = std::distance( range.first, range.second );

		vector_t temp(Scalar(0), 3);

		for( typename vectors_t::const_iterator vit=range.first; vit!=range.second; ++vit)
		{
			temp += vit->second;
		}

		temp = temp / static_cast<Scalar>(num_of_vectors);

		vector_t test = std::abs(temp - it->second);
		if( std::find_if( 
			&test[0], &test[0]+test.size(), 
			std::bind2nd(std::greater<Scalar>(), epsilon) ) 
			== &test[0]+test.size() )
			ret = true;

		it->second = temp;
	}
	return ret;
}







//-----------------------------------------------------------------------------
//template <class Mesh>
//void TensorVotingT<Mesh>::color_coding()
//{
//	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
//	typename Mesh::Color       col;
//
//	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
//	{
//		int temp_id = (int)((int)(mesh_.property(vclusterid_, v_it))%(int)(10));
//
//		switch(temp_id)
//		{
//		case 0:
//			{
//				col = Mesh::Color(255,0,0);
//			}
//			break;
//
//		case 1:
//			{
//				col = Mesh::Color(0,255,0);
//			}
//			break;
//
//		case 2:
//			{
//				col = Mesh::Color(0,0,255);
//			}
//			break;
//
//		case 3:
//			{
//				col = Mesh::Color(0,255,255);
//			}
//			break;
//
//		case 4:
//			{
//				col = Mesh::Color(255,0,255);
//			}
//			break;
//
//		case 5:
//			{
//				col = Mesh::Color(255,255,0);
//			}
//			break;
//
//		case 6:
//			{
//				col = Mesh::Color(128,0,128);
//			}
//			break;
//
//		case 7:
//			{
//				col = Mesh::Color(0,128,128);
//			}
//			break;
//
//		case 8:
//			{
//				col = Mesh::Color(128,128,0);
//			}
//			break;
//
//		case 9:
//			{
//				col = Mesh::Color(0,0,0);
//			}
//			break;
//		} // end of switch
//
//		mesh_.set_color(v_it, col);
//
//	}	// end of for
//}



	//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif