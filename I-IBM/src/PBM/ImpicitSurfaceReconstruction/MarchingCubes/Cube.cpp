/* File Name:   Cube.cpp
 * Description: Implementation of Cube class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 *
 * This program is based on Marching Cubes Example Program by Cory Bloyd
 * (corysama@yahoo.com)
 * A simple, portable and complete implementation of the Marching Cubes
 * and Marching Tetrahedrons algorithms in a single source file.
 * There are many ways that this code could be made faster, but the 
 * intent is for the code to be easy to understand.
 *
 * Also some good ideas such as marching edges from Josh Grant 
 * (cereal@wheatchex.com) at http://www.wheatchex.com are really helpful.
 *
 * For a description of the algorithm go to
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
 * For a description of the implementation go to my homepage at
 * http://www.ccs.neu.edu/home/gaob/marchingcubes.html
 */

#include "Cube.h"

namespace MarchingCubes{
int Cube::MarchCube(Vector3f p, int index, float *cubeValue, Vector3f *cubeNormals,
        int *(&edgeIntersection), Vec3fvector &triangleVertices, 
        Vec3fvector &triangleNormals, Intvector &triangleConnection){
    int corner, vertex, edge, triangle;
    int flagIndex;
    float offset;
    Vector3f edgeNormal[12];
    Vector3f edgeVertex[12];

    // Find which vertices are inside of the surface and which are outside
    flagIndex = 0;
    for (vertex = 0; vertex < 8; vertex ++)
        if (cubeValue[vertex] < 0.0)
            flagIndex |= 1 << vertex;
    edgeFlags = cubeEdgeFlags[flagIndex];
    if (edgeFlags == 0)
        return -1;

    flagIndex = 0;
    for (vertex = 0; vertex < 8; vertex ++)
        if (cubeValue[vertex] < ISOVALUE && cubeValue[vertex] != -1.0)
            flagIndex |= 1 << vertex;
	
    // Find which edges are intersected by the surface
    edgeFlags = cubeEdgeFlags[flagIndex];

    // If the cube is entirely inside or outside of the surface, the there will
    // be no intersections
    if (edgeFlags == 0)
	return -1;

    for (edge = 0; edge < 12; edge ++){
	// if there is an intersection on this edge
	if (edgeFlags & (1 << edge)){
            offset = getOffset(cubeValue[cubeEdgeConnection[edge][0]],
                               cubeValue[cubeEdgeConnection[edge][1]]);
            float x = p[0] + (cubeVertexOffset[cubeEdgeConnection[edge][0]][0] +
                      offset * cubeEdgeDirection[edge][0]) * VOXEL[0];
            float y = p[1] + (cubeVertexOffset[cubeEdgeConnection[edge][0]][1] +
                      offset * cubeEdgeDirection[edge][1]) * VOXEL[1];
            float z = p[2] + (cubeVertexOffset[cubeEdgeConnection[edge][0]][2] +
                      offset * cubeEdgeDirection[edge][2]) * VOXEL[2];

            edgeVertex[edge] = Vector3f(x, y, z);
            edgeNormal[edge] = cubeNormals[cubeEdgeConnection[edge][0]] + 
                cubeNormals[cubeEdgeConnection[edge][1]] * offset;
            edgeNormal[edge].normalize();

            if (edgeIntersection[edgeIndex[edge] + index] > -1){
#ifdef DEBUG
                // debug
                if (index % (DIMS[0] * DIMS[1]) == 0){
                    qDebug() << "the intersection exists >> ";
                    Vector3f intsect = triangleVertices[edgeIntersection[edgeIndex[edge] + index]];
                    qDebug() << index << " : " << edge << " : " << (intsect.equals(edgeVertex[edge], 0.00001)?"Equal":"NOT Equal") << endl;
                }
#endif
                triangleVertices[edgeIntersection[edgeIndex[edge] + index]] = 
                    (triangleVertices[edgeIntersection[edgeIndex[edge] + index]] + edgeVertex[edge]) / 2.0; 
                triangleNormals[edgeIntersection[edgeIndex[edge] + index]] = 
                    (triangleNormals[edgeIntersection[edgeIndex[edge] + index]] + edgeNormal[edge]) / 2.0;
            }
            if (edgeIntersection[edgeIndex[edge] + index] == -1){
                triangleVertices.push_back(edgeVertex[edge]);
                triangleNormals.push_back(edgeNormal[edge]);
                edgeIntersection[edgeIndex[edge] + index] = (int)triangleVertices.size() - 1;
            }
	}
    }

    // Draw the triangles that were found. There can be up to four per cube
    for (triangle = 0; triangle < 5; triangle ++){
	if (triangleConnectionTable[flagIndex][3 * triangle] < 0)
            break;
        vector<int> connection(3);
	for (corner = 0; corner < 3; corner ++){
            vertex = triangleConnectionTable[flagIndex][3 * triangle + corner];
            connection[2 - corner] = (edgeIntersection[edgeIndex[vertex] + index]);
        }

        for (int i = 0; i < 2; i ++)
            for (int j = i + 1; j < 3; j ++){
                if ((triangleVertices[connection[i]] - triangleVertices[connection[j]]).length() < -e){
                    triangleVertices[connection[i]] = (triangleVertices[connection[i]] + triangleVertices[connection[j]]) / 2.0;
                    triangleVertices[connection[j]] = triangleVertices[connection[i]];
                }
            } 
        triangleConnection.push_back(connection); 
    }
    return 1;
}

// GetOffset finds the approximate point of intersection of the surface
// between two points with the values value1 and value2
float Cube::getOffset(float value1, float value2){
	double delta = value2 - value1;
	
	if( delta == 0.0 )
		return 0.5;

	return ( ISOVALUE - value1 ) / delta;
}

}