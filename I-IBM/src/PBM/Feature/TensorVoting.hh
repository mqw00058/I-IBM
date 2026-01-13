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
//   $Date: 2011.06.25 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file TensorVoting.hh

 */

//=============================================================================
//
//  CLASS TensorVotingT
//
//=============================================================================

#ifndef PBM_FEATURE_TENSORVOTING_HH
#define PBM_FEATURE_TENSORVOTING_HH


//== INCLUDES =================================================================
#include <pbm/Common/NormalEstimation.h>


#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include<vector>
#include<string>
#include<algorithm>
#include<iterator>
#include<numeric>
#include<exception>
#include<map>
#include<valarray>
#include<sstream>
#include<functional>
#include<iostream>
#include <math.h>

using namespace OpenMesh;


//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================


namespace PBM {
namespace Feature {

	
//== CLASS DEFINITION =========================================================
template <class Mesh>
class TensorVotingT
{

	
public:
typedef typename Mesh::Scalar					Scalar;
typedef std::valarray<Scalar>					vector_t;
typedef std::multimap< size_t, vector_t >		vectors_t;

	// constructor & destructor
	TensorVotingT( Mesh& _mesh );
	virtual ~TensorVotingT();

	
	void initialize();
	void apply(int minScale, int maxScale){

		
		//ne->Delaunay_neighborhood(5, 10.0, 9, knn);
		//normalvoting();
		multiscale_tensorvoting(minScale, maxScale);
		//normalvoting();
		 
		//KMeansClustering(3, 50, 0.01);
		color_coding();

		// feature refine
		feature_classification();
		feature_completion();
		// line-type feature reconstruction
		feature_connection();


	}
	
	//void normalvoting();
	void multiscale_tensorvoting(int minScale, int maxScale, float min_w, float max_w);
	void normalvoting();

	void feature_classification();
	void feature_completion();
	void feature_connection();

	// clustering
	void clustering(unsigned int num_of_clusters);
	void KMeansClustering(unsigned int num_of_clusters, unsigned int maximum_num_of_iterations, Scalar epsilon_);
	bool update(Scalar);

	void color_coding();

	Scalar euclidean_sqdistance(std::valarray<Scalar> left, 
		std::valarray<Scalar> right )
	{
		std::valarray<Scalar> t1 = std::abs(left - right);
		std::valarray<Scalar> t2 = std::pow( t1, Scalar(2) );

		return Scalar(t2.sum());
	}




	// easier access to the saliency of vertex
	float& fweight(VertexHandle _vh) 
	{ return mesh_.property(fweight_, _vh); }
	// easier access to the saliency of vertex
	int& scale(VertexHandle _vh) 
	{ return mesh_.property(scale_, _vh); }



private:
	NormalEstimation *ne;

	OpenMesh::VPropHandleT<Vec3f>	e1_;
	OpenMesh::VPropHandleT<Vec3f>	e2_;
	OpenMesh::VPropHandleT<Vec3f>	e3_;
	OpenMesh::VPropHandleT<Vec3f>	ev_;

	OpenMesh::VPropHandleT<float>	fweight_;
	OpenMesh::VPropHandleT<float>	min_fweight_;

	OpenMesh::VPropHandleT<int>		scale_;
	OpenMesh::VPropHandleT<int>		nfeature_;
	OpenMesh::VPropHandleT<bool>	feature_candidate_;

	
	
	vector<int> feature_queue;	// index vector


	double **matI;		//identity matrix


	vectors_t centers;					// 각 클러스터의 중심점
	vectors_t vectors;

	int nPts;			// point의 개수




protected:

	Mesh&  mesh_;				// OpenMesh data structure
	vector<Vec3f>	point_set;	// point list (data structure)

};


//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(PBM_FEATURE_TENSORVOTING_CC)
#define PBM_TensorVotingT_TEMPLATES
#include "TensorVoting.cc"
#endif
//=============================================================================
#endif // PBM_FEATURE_TensorVotingT_HH defined
//=============================================================================

