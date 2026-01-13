//=============================================================================
// Multi-Scale Feature-Aware Filtering
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2013.01.25 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//=============================================================================
#pragma once
#include <src/pbm/Common/NormalEstimation.h>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <math.h>


using namespace OpenMesh;
typedef PBM::NormalEstimation	NE;

template <class Mesh>
class MKFilter
{
public:
	MKFilter(Mesh& _mesh);
	 ~MKFilter(void);

	void initialize();
	void compute_density();
	void compute_feature_map(int k);
	void compute_normalspace();
	void filtering(float sigma1_, float sigma2_);
	void color_coding();

	void apply()
	{
		//filtering(0.f, 0.f);
	}

public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::Point					Point;


private:

	// member variables
	OpenMesh::VPropHandleT<Vec3f>	e1_;
	OpenMesh::VPropHandleT<Vec3f>	e2_;
	OpenMesh::VPropHandleT<Vec3f>	e3_;
	OpenMesh::VPropHandleT<Vec3f>	ev_;

	OpenMesh::VPropHandleT<float>	fweight_;
	OpenMesh::VPropHandleT<float>	min_fweight_;
	OpenMesh::VPropHandleT<float>	density_;

	OpenMesh::VPropHandleT<int>		scale_;
	OpenMesh::VPropHandleT<int>		nfeature_;
	OpenMesh::VPropHandleT<bool>	feature_candidate_;


	int *knn;		// k-nearest neighbor index;
	int *nn;		// the number of neighbor


protected:
	Mesh& mesh_;
	vector<Vec3f>	point_set;	// point list (data structure)
	NE *ne;

	int nPts;

};


