#pragma once
#ifndef IMPLICIT_SURFACE_RECONSTRUCTION_HH
#define IMPLICIT_SURFACE_RECONSTRUCTION_HH



//== INCLUDES =================================================================

//== FORWARDDECLARATIONS ======================================================
///#include "src/PBM/Common/NormalEstimation.h"
#include "src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/MarchingCube.h"
//#include "src/MemoryLeakDetector/DebugNew.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>


//== NAMESPACES ===============================================================
//typedef Point<3> Point3;
struct TempTraits : public OpenMesh::DefaultTraits
{
#if 1
	typedef OpenMesh::Vec3f Point;
	typedef OpenMesh::Vec3f Normal;
	typedef OpenMesh::Vec3uc Color;

#else
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
	typedef OpenMesh::Vec3uc Color;
#endif
};
typedef OpenMesh::TriMesh_ArrayKernelT<TempTraits> Mesh;
using namespace OpenMesh;

//typedef PBM::NormalEstimation	NE;

namespace PBM{
	namespace ImplicitSurfaceReconstruction{

class ImplicitSurfaceReconstructionT
{	
	//--------------------- Type Definitions	
protected:
	Mesh *mesh_;
	vector<OpenMesh::Vec3f>	point_set;	// point list (data structure)



public:	
	ImplicitSurfaceReconstructionT::
		ImplicitSurfaceReconstructionT(void* _mesh)
	{
			mesh_ = (Mesh*)_mesh;
			mesh_->add_property(ev_);		// eigenvalue
			mesh_->add_property(e1_);		// eigenvector
			mesh_->add_property(e2_);		// eigenvector
			mesh_->add_property(e3_);		// eigenvector
			mesh_->add_property(feature_candidate_);	// surface saliency
			mesh_->add_property(scale_);		// surface(point) type
			mesh_->add_property(nfeature_);
			mesh_->add_property(fweight_);
			mesh_->add_property(min_fweight_);
			mesh_->add_property(density_);

			initialize();
		}
	ImplicitSurfaceReconstructionT::
		~ImplicitSurfaceReconstructionT(void)
	{
			mesh_->remove_property(ev_);		// eigenvalue
			mesh_->remove_property(e1_);		// eigenvector
			mesh_->remove_property(e2_);		// eigenvector
			mesh_->remove_property(e3_);		// eigenvector
			mesh_->remove_property(feature_candidate_);	// surface saliency
			mesh_->remove_property(scale_);
			mesh_->remove_property(nfeature_);
			mesh_->remove_property(fweight_);
			mesh_->remove_property(min_fweight_);
			mesh_->remove_property(density_);

			//	delete ne;

			//delete [] knn;
			//	delete [] nn;

			//	delete [] POINTCLOUD;
		}

	void ImplicitSurfaceReconstructionT::initialize()
	{
		//// geometry property setting
		//Mesh::VertexIter	v_it, v_end(mesh_->vertices_end());

		//for( v_it=mesh_->vertices_begin(); v_it!=v_end; ++v_it){

		//	// initialize point set
		//	point_set.push_back(mesh_->point(v_it));	
		//}	

	}

	void LoadPoints(std::vector<float>* vec_sdf=0);
	void ComputeNormals(int nMax);
	void SurfaceGeneration();
	void OutputToOpenMesh();

	
private:
	MarchingCubeNameSpace::Vec3fvector triangleVertices;
	MarchingCubeNameSpace::Vec3fvector triangleNormals;
	MarchingCubeNameSpace::Intvector triangleConnection;

	// member variables
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	e1_;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	e2_;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	e3_;
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	ev_;

	OpenMesh::VPropHandleT<float>	fweight_;
	OpenMesh::VPropHandleT<float>	min_fweight_;
	OpenMesh::VPropHandleT<float>	density_;

	OpenMesh::VPropHandleT<int>		scale_;
	OpenMesh::VPropHandleT<int>		nfeature_;
	OpenMesh::VPropHandleT<bool>	feature_candidate_;

};
}
}
/*
//Template를 헤더에서 함수정의 선언을 한꺼번에 해야 하는데 선언과 정의를 헤더와 cpp로 각각 나눌려면 다음과 같이
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(IMPLICIT_SURFACE_RECONSTRUCTION_CC)
#define IMPLICIT_SURFACE_RECONSTRUCTION_TEMPLATES
#include "ImplicitSurfaceReconstruction.cpp"
#endif
//=============================================================================
*/

#endif