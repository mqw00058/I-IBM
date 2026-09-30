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

#ifndef OPENMESH_MESHCLEAN_CC
#define OPENMESH_MESHCLEAN_CC

//== INCLUDES =================================================================
#include "MeshClean.hh"
#include <stack>

//== NAMESPACES ===============================================================


namespace OpenMesh {
	namespace Utils {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
MeshCleanT<Mesh>::
MeshCleanT(Mesh& _mesh)
	: mesh_(_mesh)
{

}

//-----------------------------------------------------------------------------
template <class Mesh>
MeshCleanT<Mesh>::
~MeshCleanT()
{
	
}

template <class Mesh>
bool
MeshCleanT<Mesh>::
findIrregularTriangles(std::set<int> &irregularfacelist)
{
	typename Mesh::VertexHandle vh0, vh1;
	OpenMesh::Vec3f		vec0, vec1;

	OpenMesh::HalfedgeHandle heh;
	typename Mesh::FaceIter f_it, f_end(mesh_.faces_end());
	typename Mesh::FaceHalfedgeIter fh_it;


	float tri_len_a = 0.0, tri_len_b = 0.0, tri_len_c = 0.0, p = 0.0, s = 0.0, r = 0.0, l = 0.0, w = 0.0;

	for (f_it = mesh_.faces_begin(); f_it != f_end; ++f_it)
	{
		fh_it = mesh_.fh_begin(f_it.handle());
		heh = fh_it.handle();
		vh0 = mesh_.from_vertex_handle(heh);
		vh1 = mesh_.to_vertex_handle(heh);

		vec0 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh0));
		vec1 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh1));

		tri_len_a = (vec1 - vec0).length();

		heh = mesh_.next_halfedge_handle(heh);
		vh0 = mesh_.from_vertex_handle(heh);
		vh1 = mesh_.to_vertex_handle(heh);

		vec0 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh0));
		vec1 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh1));

		tri_len_b = (vec1 - vec0).length();


		heh = mesh_.next_halfedge_handle(heh);
		vh0 = mesh_.from_vertex_handle(heh);
		vh1 = mesh_.to_vertex_handle(heh);

		vec0 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh0));
		vec1 = OpenMesh::vector_cast<OpenMesh::Vec3f>(mesh_.point(vh1));

		tri_len_c = (vec1 - vec0).length();

		////////////////////////////////////////////////////////////////
		//          sqrt 헤론의공식: 삼각형넓이 세변으로 구하기       //
		//  세변 a,b,c, => p=(a+b+c)/2, 넓이:s=root(p(p-a)(p-b)(p-c)) //
		////////////////////////////////////////////////////////////////

		p = (tri_len_a + tri_len_b + tri_len_c) *  0.5;
		s = sqrt(p*(p - tri_len_a)*(p - tri_len_b)*(p - tri_len_c));


		//////////////////////////////////////////////////////////////////////////////////
		//                   Garland irregularity term r                                //
		//                                                                              //
		//r=l^2/4*pi*w => l:삼각형 둘레 길이(tr_abc), w:원의 넓이(여기서는 삼각형넓이:s)//
		//////////////////////////////////////////////////////////////////////////////////
		r = (tri_len_a + tri_len_b + tri_len_c)*(tri_len_a + tri_len_b + tri_len_c) / (4 * 3.14*s);

	}

}


template <class Mesh>
std::pair<int, int> MeshCleanT<Mesh>::RemoveSmallConnectedComponentsSize(int maxCCSize)
{
	std::vector < std::set<int>> CCF; //CCF[i] : i번째 isolated 클러스터의 connected faces index 들이  std::set<int> 안으로
	int TotalCC = ConnectedComponents(CCF);
	int DeletedCC = 0;
	for (unsigned int i = 0; i < CCF.size(); ++i)
	{
		if (CCF[i].size()< maxCCSize) // i번째 isolated 클러스터의 connected faces 의 개수가 maxCCSize 보다 작을때 제거
		{
			for (std::set<int>::iterator it = CCF[i].begin(); it != CCF[i].end(); ++it)
			{
				typename Mesh::FaceHandle ff_h(*it);
				if (mesh_.is_valid_handle(ff_h))
				{
					mesh_.delete_face(ff_h);
				}
			}
			DeletedCC++;
		}
	}
	mesh_.garbage_collection();
	mesh_.update_normals();
	return std::make_pair(TotalCC, DeletedCC); //TotalCC : num of ConnectedComponent, DeletedCC : num of Deleted ConnectedComponent
}
template <class Mesh>
int MeshCleanT<Mesh>::ConnectedComponents(std::vector < std::set<int>> &CCF)
{

	typename Mesh::FaceIter fit;	
	

	CCF.clear();
	for (fit = mesh_.faces_begin(); fit != mesh_.faces_end(); ++fit)
	{
		std::stack<int> sf;

		if (!mesh_.status(fit.handle()).tagged())
		{
			sf.push(fit.handle().idx());
			std::set<int> connectedFaces;
			CCF.push_back(connectedFaces);
			while (!sf.empty())
			{
				int index = sf.top();
				typename Mesh::FaceHandle f_h(index);

				CCF.back().insert(f_h.idx());
				mesh_.status(f_h).set_tagged(true);
				sf.pop();

				for (typename Mesh::FFIter ff_it = mesh_.ff_iter(f_h); ff_it; ++ff_it)
				{
					{
						if (!mesh_.status(ff_it).tagged())
						sf.push((*ff_it).idx());

					}
				}
			}
		}
	}
	return CCF.size();
}




//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif