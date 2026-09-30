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
//   $Date: 2011.02.22 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//	 $Modified by Min Ki Park (minkp@gist.ac.kr):: Gaussian noise, color noise
//                                                                            
//=============================================================================

/** \file AddNoiseT.cc
    
 */

//=============================================================================
//
//  CLASS AddNoiseT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_ADDNOISE_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Noise/AddNoiseT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace Noise {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
AddNoiseT<Mesh>::
AddNoiseT(Mesh& _mesh)
  : mesh_(_mesh)
{
	
}

//-----------------------------------------------------------------------------


template <class Mesh>
AddNoiseT<Mesh>::
~AddNoiseT()
{

}

//-----------------------------------------------------------------------------

template <class Mesh>
void AddNoiseT<Mesh>::initialize()
{

}

//-----------------------------------------------------------------------------
template <class Mesh>
void AddNoiseT<Mesh>::apply_geometric_noise(int type_, float ratio_, float factor_)
{
	
	typename Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	srand((unsigned)time( NULL ));


	//Uniform noise
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		Vec3 noise;
		Scalar temp_ratio(0.0);

		if(type_ == 0)	//uniform noise
		{
			temp_ratio = ratio_ * factor_ * ((Scalar)(rand()%100) - 50.0)/50.0;		

		} else if(type_ == 1) { //Gaussian noise (with zero mean and unit variance) using Box-Muller algorithm

			static double v, fac;
			static int phase = 0;
			double S, Z, U1, U2, u;

			if (phase)
				Z = v * fac;
			else {
				do {
					U1 = (double)rand() / RAND_MAX;
					U2 = (double)rand() / RAND_MAX;

					u = 2. * U1 - 1.;
					v = 2. * U2 - 1.;
					S = u * u + v * v;
				} while(S >= 1);

				fac = sqrt (-2. * log(S) / S);
				Z = u * fac;
			}
			phase = 1 - phase;
			temp_ratio =ratio_ * factor_ * Z;
		} 

		noise = vector_cast<Vec3> (mesh_.point(v_it) + temp_ratio * mesh_.normal(v_it) );
		mesh_.set_point(v_it, noise);
	}

	
}

//-----------------------------------------------------------------------------
template <class Mesh>
void AddNoiseT<Mesh>::apply_appearance_noise(int type_, float ratio_, Vec3f var)
{

	typename Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());
	srand((unsigned)time( NULL ));
	int temp = 0;


	// uniform noise
	if(type_ == 0){

		int range0 = (int) sqrt(var[0] * 3) * 2;
		int range1 = (int) sqrt(var[1] * 3) * 2;
		int range2 = (int) sqrt(var[2] * 3) * 2;
		for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		{
			GeoTriMesh::Color c = mesh_.color(v_it);

			temp = c[0] + rand() % range0 - range0 / 2 ;
			if (temp < 0) 		 temp = 0;
			else if (temp > 255) temp = 255;
			c[0] = temp;

			temp = c[1] + rand() % range1 - range1 / 2 ;
			if (temp < 0) 		 temp = 0;
			else if (temp > 255) temp = 255;
			c[1] = temp;

			temp = c[2] + rand() % range2 - range2 / 2 ;
			if (temp < 0) 		 temp = 0;
			else if (temp > 255) temp = 255;
			c[2] = temp;

			mesh_.set_color(v_it, c);
		}

	}
	// gaussian noise
	else if(type_ == 1){


		for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
		{
			GeoTriMesh::Color c = mesh_.color(v_it);


			temp = c[0] + ratio_ * var[0] * noise();
			if (temp < 0)		 temp = 0;
			else if (temp > 255) temp = 255;
			c[0] = temp;
			temp = c[1] + ratio_ * var[1] * noise();
			if (temp < 0)		 temp = 0;
			else if (temp > 255) temp = 255;
			c[1] = temp;
			temp = c[2] + ratio_ * var[2] * noise();
			if (temp < 0)		 temp = 0;
			else if (temp > 255) temp = 255;
			c[2] = temp;
			
			mesh_.set_color(v_it, c);
		}
	}

}


//-----------------------------------------------------------------------------
template <class Mesh>
float AddNoiseT<Mesh>::noise()
{
	static double v, fac;
	static int phase = 0;
	double S, Z, U1, U2, u;

	if (phase){
		Z = v * fac;
	}
	else {
		for(int i=0; i<3; ++i){

			do {
				U1 = (double)rand() / RAND_MAX;
				U2 = (double)rand() / RAND_MAX;

				u = 2. * U1 - 1.;
				v = 2. * U2 - 1.;
				S = u * u + v * v;
			} while(S >= 1);

			fac = sqrt (-2. * log(S) / S);
			Z = u * fac;


		}

	}
	phase = 1 - phase;

	return Z;
}



//=============================================================================
} // namespace AddNoise
} // namespace OpenMesh
//=============================================================================

