#pragma once
#ifndef IMPLICIT_SURFACE_RECONSTRUCTION_HH
#define IMPLICIT_SURFACE_RECONSTRUCTION_HH

//== INCLUDES =================================================================


#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <PBM/Common/NormalEstimation.h>
#include <vector>


#include <math.h>
//== FORWARDDECLARATIONS ======================================================
#include <PBM/ImpicitSurfaceReconstruction/MarchingCubes/MarchingCubes.h>
#include <MemoryLeakDetector/DebugNew.h>
//== NAMESPACES ===============================================================
//typedef Point<3> Point3;

using namespace OpenMesh;
typedef PBM::NormalEstimation	NE;

namespace PBM{
	namespace ImplicitSurfaceReconstruction{

template <class Mesh>
class ImplicitSurfaceReconstructionT
{
public:
	ImplicitSurfaceReconstructionT(Mesh& _mesh);
	virtual ~ImplicitSurfaceReconstructionT();

	void LoadPoints();
	MarchingCubes::Vec3fvector triangleVertices;
	MarchingCubes::Vec3fvector triangleNormals;
	MarchingCubes::Intvector triangleConnection;
	
	//void ComputeNeighbors();
	void ComputeNormals(int num_Neighbors);
	void SurfaceGeneration();

	void OutputToOpenMesh();


public:
	void initialize();
	
private:

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




protected:
	Mesh& mesh_;
	vector<OpenMesh::Vec3f>	point_set;	// point list (data structure)

};
}
}
//Template를 헤더에서 함수정의 선언을 한꺼번에 해야 하는데 선언과 정의를 헤더와 cpp로 각각 나눌려면 다음과 같이
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(IMPLICIT_SURFACE_RECONSTRUCTION_CC)
#define IMPLICIT_SURFACE_RECONSTRUCTION_TEMPLATES
#include "ImplicitSurfaceReconstruction.cc"
#endif
//=============================================================================

#endif