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
//   $Laplacian smoothing - Umbrella operator [Pauly02] "Multi-resolution Modeling of Point-Sampled Geometry"
//   $Date: 2013.01.26 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file UmbrellaOperatorT.cc
    
 */

//=============================================================================
//
//  CLASS UmbrellaOperatorT - IMPLEMENTATION
//
//=============================================================================

#define PBM_FILTER_UMBRELLA_CC

//== INCLUDES =================================================================

#include "UmbrellaOperator.hh"

//== NAMESPACES ===============================================================


namespace PBM {
namespace Filter {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
UmbrellaOperatorT<Mesh>::
UmbrellaOperatorT(Mesh& _mesh)
  : mesh_(_mesh)
{
	initialize();
}

//-----------------------------------------------------------------------------


template <class Mesh>
UmbrellaOperatorT<Mesh>::
~UmbrellaOperatorT()
{

	delete[] ne;
}

//-----------------------------------------------------------------------------

template <class Mesh>
void UmbrellaOperatorT<Mesh>::initialize()
{
	// geometry property setting
	Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){
		point_set.push_back(mesh_.point(v_it));
	}	
	ne = new NE(point_set);
	nPts = (int)point_set.size();

}

//-----------------------------------------------------------------------------
template <class Mesh>
void UmbrellaOperatorT<Mesh>::apply(int iteration, float lambda)
{
	Point			pi, pj, dp;				//points
	VertexHandle	vh, vhn;				//vertex handle
	float			weight, sum_of_weight;	//weight
	vector<Vec3f>	mpoint_set;	

	const int nMax = 10;
	int *neighbor_table = new int[nMax*nPts];
	int *nofN = new int[nPts];


	for(int i=0; i<nPts; ++i)
	{
		nofN[i] = nMax;
		ne->k_nearest_neighbor(i, nMax, neighbor_table + i*nMax);
	}


	for(int iter=0; iter<iteration; ++iter){

		mpoint_set.clear();
		// 1. compute the new position
		for(int i=0; i<nPts; ++i){
		
			dp = Point(0.f,0.f,0.f);
			sum_of_weight = 0.f;
			vh = mesh_.vertex_handle(i);
			pi = mesh_.point(vh);

			for(int j=0; j< nofN[i]; ++j)
			{
				
				vhn = mesh_.vertex_handle( neighbor_table[i*nMax + j] );
				pj = mesh_.point(vhn);
				weight = 1/(pj-pi).norm();	//scale-dependent umbrella operator
				sum_of_weight += weight;

				dp += weight * (pj-pi);

			}

			// temp update
			mpoint_set.push_back(pi + lambda*dp/sum_of_weight);
		}
		point_set = mpoint_set;

		// 2. Anti-shrinkage
		for(int i=0; i<nPts; ++i){
			vh = mesh_.vertex_handle(i);
			dp = (mesh_.point(vh) - point_set[i])/nMax;

			// volume compensation
			for(int j=0; j< nofN[i]; ++j){
				mpoint_set[ neighbor_table[i*nMax + j] ]  = mpoint_set[ neighbor_table[i*nMax + j] ] + dp;
			}
		}

		// 3. final position update
		for(int i=0; i<nPts; ++i){
			vh = mesh_.vertex_handle(i);
			mesh_.set_point(vh, mpoint_set[i]);
		}

	}

	delete[] neighbor_table;
	delete[] nofN;

}
//=============================================================================
} // namespace UmbrellaOperatorT
} // namespace OpenMesh
//=============================================================================
