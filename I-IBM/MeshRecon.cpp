//////////////include 순서 중요!//////////////////
#include "src/PoissonRecon/MemoryUsage.h"
#include "src/PoissonRecon/SparseMatrix.h"
#include "src/PoissonRecon/Octree.h"
#include "src/PoissonRecon/MultiGridOctreeData.h"
//////////////////////////////////////////////////
#include "MeshRecon.h"


MeshRecon::MeshRecon()
{
}


MeshRecon::~MeshRecon()
{
}


void MeshRecon::PoissonRecon(const char* inputplypointfile, const char* outputplymeshfile)
{
#define Real float

	Real isoValue = 0;
	Octree<Real> tree;
	tree.threads = 1;
	OctNode<TreeNodeData>::SetAllocator(MEMORY_ALLOCATOR_BLOCK_SIZE);

	Octree< Real >::PointInfo* pointInfo = new Octree< Real >::PointInfo();
	Octree< Real >::NormalInfo* normalInfo = new Octree< Real >::NormalInfo();
	std::vector< Real >* kernelDensityWeights = new std::vector< Real >();
	std::vector< Real >* centerWeights = new std::vector< Real >();
	PointStream< float >* pointStream;
	XForm4x4< Real > xForm, iXForm;
	xForm = XForm4x4< Real >::Identity();
	iXForm = xForm.inverse();
	pointStream = new PLYPointStream< float >(inputplypointfile);

	int pointCount = tree.template SetTree<float>(pointStream, 0, 8, 5, 6, 1.0, 1.10000002, false, false, 4.0, 1, *pointInfo, *normalInfo, *kernelDensityWeights, *centerWeights, 1, xForm, false);
	delete kernelDensityWeights, kernelDensityWeights = NULL;


	Pointer(Real) constraints = tree.SetLaplacianConstraints(*normalInfo);
	delete normalInfo;

	Pointer(Real) solution = tree.SolveSystem(*pointInfo, constraints, false, 8, 8, 0, 0.00100000005);
	delete pointInfo;
	FreePointer(constraints);


	CoredFileMeshData< PlyVertex< float > > mesh;
	isoValue = tree.GetIsoValue(solution, *centerWeights);
	delete centerWeights;


	tree.GetMCIsoSurface(kernelDensityWeights ? GetPointer(*kernelDensityWeights) : NullPointer< Real >(), solution, isoValue, mesh, true, true, false);

	PlyWritePolygons((char*)outputplymeshfile, &mesh, PLY_ASCII, NULL, 0, iXForm);
	FreePointer(solution);
}