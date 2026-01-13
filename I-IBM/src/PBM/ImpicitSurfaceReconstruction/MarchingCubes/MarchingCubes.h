/* File Name:   MarchingCubes.h
 * Description: Header file defining MarchingCubes class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#ifndef _MARCHING_CUBES_
#define _MARCHING_CUBES_

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "define.h"
#include "Cube.h"
#include "Kruskal.h"

using namespace std;




namespace MarchingCubes
{

typedef vector<int> IntVector;


class MarchingCube{
private:
    Vec3fvector triangleVertices;
    Vec3fvector triangleNormals;
    Intvector triangleConnection;
    int *edgeIntersection;
    vector<int> *units; // need to be dynamic allocation;
    Vector3f *pointsCoord;
public:
	float *cubeVertices;
	Vector3f *cubeNormals;

    MarchingCube(){
        edgeIntersection = new int[3 * (DIMS[0]) * (DIMS[1]) * (DIMS[2])];
        for (int i = 0; i < 3 * (DIMS[0]) * (DIMS[1]) * (DIMS[2]); i ++)
            edgeIntersection[i] = -1;
        units = new vector<int>[UNITDIM[2] * UNITDIM[1] * UNITDIM[0]];
        pointsCoord = new Vector3f[POINTSAMOUNT];
        for (int i = 0; i < POINTSAMOUNT; i ++)
            pointsCoord[i] = POINTCLOUD[i].getPosition();
    };
    ~MarchingCube();
    void MarchingCubes();
    Vec3fvector getTriangleVertices();
    Vec3fvector getTriangleNormals();
    Intvector getTriangleConnection();
    void seperatingPoints(bool);
    void getIsoValue(const Vector3f, int, int);
    Vector3f computeNormal(const Vector3f, int*);
    int getNearestPoint(int, int, int, const Vector3f);
    void normalConsist();
    void consist(int, Vector3f, Kruskal &);
    void computeNeighbors();

	void OutputMesh(vector<vector<float>>&vlist, vector<vector<float>>&vnlist, vector<vector<int>>&flist);
};	
}
#endif /* _MARCHING_CUBES_ */
