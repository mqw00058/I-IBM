//=============================================================================
// Mesh saliency
//-----------------------------------------------------------------------------
//                                                                            
//   $Date: 2012.12.18 $
//   $Created by Min Ki Park (minkp@gist.ac.kr)
//   $Referecend by Mesh Bilateral Filter                                                                           
//=============================================================================

#pragma once


//=============================================================================
//
//  CLASS BilateralFilterT
//
//=============================================================================
#ifndef OPENMESH_MESHCLEAN_HH
#define OPENMESH_MESHCLEAN_HH



//== INCLUDES =================================================================


#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>

#include <vector>


//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================
//typedef Point<3> Point3;

using namespace OpenMesh;


namespace OpenMesh {
namespace Utils {

//== CLASS DEFINITION =========================================================
template <class Mesh>
class MeshCleanT
{
public:
	// Type Definition
	typedef typename Mesh::Scalar					Scalar;
	typedef typename Mesh::Point					Point;

public:

	// constructor & destructor
	MeshCleanT(Mesh& _mesh);
	virtual ~MeshCleanT();

	bool findIrregularTriangles(std::set<int> &irregularfacelist); //Extract Irregular Triangles 
	int ConnectedComponents(std::vector < std::set<int>> &CCF); // Extract Connected Component Cluster
	std::pair<int, int> RemoveSmallConnectedComponentsSize(int maxCCSize); // Remove Isolated Pieces wrt Diameter algorithm

private:


protected:
	Mesh& mesh_;



};


//=============================================================================
} // namespace Filter
} // namespace BilaterFilter
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(OPENMESH_MESHCLEAN_CC)
#define OPENMESH_MESHCLEAN_HH
#include "MeshClean.cc"
#endif
//=============================================================================
#endif // OPENMESH_FILTER_BILATERALFILTER_HH defined
//=============================================================================

