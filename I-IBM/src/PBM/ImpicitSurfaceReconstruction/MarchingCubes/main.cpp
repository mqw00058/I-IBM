/* File Name:   main.cpp
 * Description: main function.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#include <fstream>
#include <string>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>

#include "define.h"
#include "MarchingCubes.h"

using namespace std;
using namespace MarchingCubes;

int main(int argc, char **argv){
    if (argc == 2)
        pointFileName = argv[1];
    createFileName();
//    ReadPoints();
    bool scanNormal = readPly();
    MarchingCube cube = MarchingCube();
    cube.seperatingPoints(scanNormal);
    cube.computeNeighbors();
    if (!(scanNormal && useScanNormal))
        cube.normalConsist(); 
    cube.MarchingCubes();
    writeOffMeshFile(cube.getTriangleVertices(), cube.getTriangleConnection());
    writeMeshFile(cube.getTriangleVertices(), cube.getTriangleNormals(),
            cube.getTriangleConnection());
    
    return 0;
}
