/* File Name:   define.h
 * Description: Header file defining global variables.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#ifndef _DEFINE_
#define _DEFINE_

#include <qDebug>

#include <string>
#include <vector>
#include <cassert>
#include "Vector3f.h"
#include "Point.h"
#include "ply.h"

//#include "src/MemoryLeakDetector/DebugNew.h"

#define DEBUG
#undef DEBUG

using namespace std;

extern MarchingCubeNameSpace::Point *POINTCLOUD;      // pointer to the point cloud
extern int POINTSAMOUNT;       // number of points
extern int maxIndex[3];        // index of point that has max x/y/z
extern int minIndex[3];        // index of point that has min x/y/z

extern string pointFileName;    // point cloud file name
extern string meshFileName;     // mesh triangle file name
extern string offMeshFileName;  // off format mesh triangle file name
extern string gridFileName;     // separated points index file name
extern string neighborFileName; // K-neighbor of each point file name
extern string eigenNormalFileName;  // eigen normal file name



namespace MarchingCubeNameSpace
{
	
typedef vector<Vector3f> Vec3fvector;
typedef vector< vector<int> > Intvector;



const float ISOVALUE = 0.0;    // threshold assuming the iso-surface

const int DIMS[3] = {128, 128, 128};    // marching cubs dimension
const int UNIT = 4;                     // how many cubes in each directoin(x/y/z) composing a grid unit
const int UNITDIM[3] = {32, 32, 32};    // how many grids in each direction(x/y/z/)
const float VOXEL[] = {1.0 / (float)(DIMS[0] - 1),    // marching cube size
                       1.0 / (float)(DIMS[1] - 1),
                       1.0 / (float)(DIMS[2] - 1)};
const float e = -0.000001;     // used sometime instead of 0

const float LBF[] = {-0.5, -0.5, -0.5};
const float RTN[] = { 0.5,  0.5,  0.5};

enum ConsistStart {minx, miny, minz, maxx, maxy, maxz};  
const ConsistStart consistStart = maxx;  // start point of consistence func.
const MarchingCubeNameSpace::Vector3f stdNormals[] = { MarchingCubeNameSpace::Vector3f(-1.0, 0.0, 0.0),
MarchingCubeNameSpace::Vector3f(0.0, -1.0, 0.0),
MarchingCubeNameSpace::Vector3f(0.0, 0.0, -1.0),
MarchingCubeNameSpace::Vector3f(1.0, 0.0, 0.0),
MarchingCubeNameSpace::Vector3f(0.0, 1.0, 0.0),
MarchingCubeNameSpace::Vector3f(0.0, 0.0, 1.0) };



const bool useScanNormal = false;   // use normal in the scan data if the it is available

// Vector struct has to be created because of reading ply file
struct Vector{
    float x, y, z;
};

void ReadPoints();
void writeMeshFile(MarchingCubeNameSpace::Vec3fvector, MarchingCubeNameSpace::Vec3fvector, MarchingCubeNameSpace::Intvector);
void writeOffMeshFile(MarchingCubeNameSpace::Vec3fvector, MarchingCubeNameSpace::Intvector);
void createFileName();
bool readPly();

}
#endif
