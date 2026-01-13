/* File Name:   MarchingCubes.cpp
 * Description: Implementatino of MarchingCubes class.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#include "MarchingCubes.h"
#include "Kruskal.h"

namespace MarchingCubes
{

static int dim = (DIMS[2] + 1) * (DIMS[1] + 1) * (DIMS[0] + 1);


bool scanNormal;


void MarchingCube::MarchingCubes(){
	
	cubeVertices = new float[dim];
    cubeNormals = new Vector3f[dim];
	
	float percent = 0.0;
    int index = 0;
    Vector3f p = Vector3f(0.0, 0.0, 0.0);

    // Sequentially get the iso-value at each vertex
    qDebug() << endl << "getting iso-value at each vertex..." << endl;
    for (int k = 0; k < DIMS[2] + 1; k ++){
	p[2] = LBF[2] + k * VOXEL[2];
	for (int j = 0; j < DIMS[1] + 1; j ++){
            p[1] = LBF[1] + j * VOXEL[1];
            bool rowNormalChanged = false;
            for (int i = 0; i < DIMS[0] + 1; i ++){
		p[0] = LBF[0] + i * VOXEL[0];
		int nearestPoint = getNearestPoint(i, j, k, p);
                getIsoValue(p, nearestPoint, index);
                index ++;
		if ((float)index / (float)((DIMS[0] + 1) * (DIMS[1] + 1) * 
                            (DIMS[2] + 1)) > percent){
                //    qDebug().width(4);
                    percent += 0.01;
                    qDebug() << (int)(percent * 100.0) << "% \r" << flush;
                }
            }
        }
    }
    qDebug() << endl;

    int cubeIndex = 0;
    // Sequentially march cubes
    qDebug() << endl << "Marching Cubes..." << endl;
    for (int k = 0; k < DIMS[2] - 1; k ++){
        p[2] = LBF[2] + k * VOXEL[2];
        for (int j = 0; j < DIMS[1] - 1; j ++){
            p[1] = LBF[1] + j * VOXEL[1];
            for (int i = 0; i < DIMS[0] - 1; i ++){
                p[0] = LBF[0] + i * VOXEL[0];
                Cube cb;
                float cubeValue[8];
                Vector3f cubeNormal[8];
                for (int vertex = 0; vertex < 8; vertex ++){
                    int iidx = (int)cubeVertexOffset[vertex][0] + i;
                    int jidx = ((int)cubeVertexOffset[vertex][1] + j) * 
                        (DIMS[0] + 1);
                    int kidx = ((int)cubeVertexOffset[vertex][2] + k) * 
                        (DIMS[0] + 1) * (DIMS[1] + 1);
                    cubeValue[vertex] = cubeVertices[iidx + jidx + kidx];
                    cubeNormal[vertex] = cubeNormals[iidx + jidx + kidx];
                }
                cb.MarchCube(p, cubeIndex, cubeValue, cubeNormal, 
                        edgeIntersection, triangleVertices, triangleNormals,
                        triangleConnection);
                cubeIndex += 3;
            }
            cubeIndex += 3;
        }
        cubeIndex += 3 * DIMS[0];
    }
}

MarchingCube::~MarchingCube(){
	delete[] units;
	delete[] edgeIntersection;
	delete[] cubeVertices;
	delete[] cubeNormals;
	delete[] pointsCoord;
	delete[] POINTCLOUD;
	triangleVertices.clear();
	triangleNormals.clear();
}

// Seperate the points into grids. When looking for neighbors, it only needs to
// look for points in several grids around. And save the grid information into
// the grid file.
void MarchingCube::seperatingPoints(bool sn){
    scanNormal = sn;
    float percent = 0.0;
    int index = 0;
    qDebug() << endl << "seperating points..." << endl;
    for (int n = 0; n < POINTSAMOUNT; n ++){
        Vector3f p = pointsCoord[n];
        int i = (int)((p[0] - LBF[0]) / (VOXEL[0] * UNIT));
        int j = (int)((p[1] - LBF[1]) / (VOXEL[1] * UNIT));
        int k = (int)((p[2] - LBF[2]) / (VOXEL[2] * UNIT));
        POINTCLOUD[n].setGrid(i, j, k);
        units[k * UNITDIM[1] * UNITDIM[0] + j * UNITDIM[0] + i].push_back(n);
    }
}

// Compute K-nearest neighbors. Sort them according to the normal differences.
// Save the neighbor information into the neighbor file.
void MarchingCube::computeNeighbors(){
    qDebug() << endl << "computing K neighbors" << endl;
    float percent = 0.0;
    ifstream neighborFile(neighborFileName.c_str());
    if (!neighborFile){
	qDebug() << "neighbor file doesn't exist. create it." << endl;
	neighborFile.close();
	neighborFileName="neighbor.txt";
	ofstream neighborFile(neighborFileName.c_str());
        if (!neighborFile){
            qDebug() << "open neighbor file failed!" << endl;
            exit(0);
        }

		ofstream pointcloud("pointcloud.txt");
    for (int n = 0; n < POINTSAMOUNT; n ++){
		pointcloud << POINTCLOUD[n].getPosition()[0] << " " << POINTCLOUD[n].getPosition()[1] << " " << POINTCLOUD[n].getPosition()[2] <<endl;
        int *grid = POINTCLOUD[n].getGrid();
        int nb = 0;
        int nghb[K];
        float dist[K];
        Vector3f center(0.0, 0.0, 0.0);
        Vector3f normal(0.0, 0.0, 0.0);
        for (int i = 0; i < K; i ++){
            nghb[i] = -1;
            dist[i] = 1.0;
        }
        for(int k = max(0, grid[2] - 2); k < min(UNITDIM[2], grid[2] + 2); 
                k ++)
            for (int j = max(0, grid[1] - 2); j < min(UNITDIM[1], 
                        grid[1] + 2); j ++)
                for (int i = max(0, grid[0] - 2); i < min(UNITDIM[0], 
                            grid[0] + 2); i ++)
                    for (int p = 0; p < (int)units[k * UNITDIM[1] * UNITDIM[0]
                            + j * UNITDIM[0] + i].size(); p ++){
                        if (n == units[k * UNITDIM[1] * UNITDIM[0] +
                                j * UNITDIM[0] + i][p])
                            continue;
                        for (int m = 0; m < K; m ++){
                            Vector3f pt = pointsCoord[n];
                            Vector3f pp = pointsCoord[units[k * UNITDIM[1] *
                                UNITDIM[0] + j * UNITDIM[0] + i][p]]; 
                            if ((pt - pp).length() < dist[m]){
                                for (int l = K - 1; l > m; l --){
                                    nghb[l] = nghb[l - 1];
                                    dist[l] = dist[l - 1];
                                }
                                nghb[m] = units[k * UNITDIM[1] * UNITDIM[0] + 
                                    j * UNITDIM[0] + i][p];
                                dist[m] = (pt - pp).length();
                                break;
                            }
                        }
                    }
							neighborFile << n <<"    ";
        for (int i = 0; i < K; i ++)
            neighborFile << nghb[i] << ' ';
        neighborFile << endl;
        POINTCLOUD[n].setKnghb(nghb);
        if (nghb[K - 1] >= 0){
            for (int i = 0; i < K; i ++)
                center = center + pointsCoord[nghb[i]];
    //            center = center + POINTCLOUD[nghb[i]].getPosition();
            center = center / K;
            normal = computeNormal(center, nghb);
        }
        POINTCLOUD[n].setCenter(center);
        POINTCLOUD[n].setEigenNormal(normal);
        if ((float)n / (float)POINTSAMOUNT > percent){
            percent += 0.01;
          //  qDebug().width(4);
            qDebug() << (int)(percent * 100.0) << "% \r" << flush;
        }
    }
	pointcloud.close();

    qDebug() << endl;
    neighborFile.close();
    qDebug() << "created neighbor file: " << neighborFileName.c_str() << endl;
    }
    else{
        qDebug() << "neighbor file exists. read it." << endl;
        for (int n = 0; n < POINTSAMOUNT; n++){
            int nghb[K];
            Vector3f center(0.0, 0.0, 0.0);
            Vector3f normal(0.0, 0.0, 0.0);
            for (int i = 0; i < K; i ++)
                neighborFile >> nghb[i];
            POINTCLOUD[n].setKnghb(nghb);
            if (nghb[K - 1] >= 0){
                for (int i = 0; i < K; i ++)
                    center = center + pointsCoord[nghb[i]];
                center = center / K;
                normal = computeNormal(center, nghb);
            }
            POINTCLOUD[n].setCenter(center);
            POINTCLOUD[n].setEigenNormal(normal);
        }
        neighborFile.close();
    }
}

int MarchingCube::getNearestPoint(int pi, int pj, int pk, Vector3f p){
    int gridx, gridy, gridz;
    gridx = pi / UNIT;
    gridy = pj / UNIT;
    gridz = pk / UNIT;
    int np = -1;
    float dist = 1.0;

    for (int i = max(0, gridz - 1); i < min(UNITDIM[2], gridz + 1); i ++)
        for (int j = max(0, gridy - 1); j < min(UNITDIM[1], gridy + 1); j ++)
            for (int k = max(0, gridx - 1); k < min(UNITDIM[0], gridx + 1);
                    k ++)
		for (int n = 0; n < (int)units[i * UNITDIM[1] * UNITDIM[0] +
                        j * UNITDIM[0] + k].size(); n ++){
                    Vector3f pp = pointsCoord[units[i * UNITDIM[1] * UNITDIM[0]
                        + j * UNITDIM[0] + k][n]];
                    if ((p - pp).length() < dist){
                        np = units[i * UNITDIM[1] * UNITDIM[0] + j * UNITDIM[0]
                            + k][n];
                        dist = (p - pp).length();
                    }
                }
//    if (dist > VOXEL[0])
//        np = -1;
    return np;
}

Vec3fvector MarchingCube::getTriangleVertices(){
    return triangleVertices;
}

Vec3fvector MarchingCube::getTriangleNormals(){
    return triangleNormals;
}

Intvector MarchingCube::getTriangleConnection(){
    return triangleConnection;
} 

void MarchingCube::getIsoValue(Vector3f p, int np, int index){
    float isoValue = -1.0;
    Vector3f normal(0.0, 0.0, 0.0);
    if (np != -1){
        Vector3f center = POINTCLOUD[np].getCenter();
        if (scanNormal && useScanNormal)
            normal = POINTCLOUD[np].getNormal();
        else
            normal = POINTCLOUD[np].getEigenNormal();
        Vector3f v = p - center;
        v.normalize();
        isoValue = (p - center).length() * (normal.dot(v));
    }
    cubeVertices[index] = isoValue;
    cubeNormals[index] = normal;
}


Vector3f MarchingCube::computeNormal(Vector3f center, int *neighbors){ 
    int i, j, k, l, nrot;
    Mat_DP cv(3, 3);
    Vec_DP d(3);
    Mat_DP v(3, 3);
    for (i = 0; i < 3; i ++)
        for (j = 0; j < 3; j ++)
            cv[i][j] = 0.0;

    for (i = 0; i < K; i ++){
	Vector3f point = POINTCLOUD[neighbors[i]].getPosition();
	float a[3];
	for (k = 0; k < 3; k ++)
            a[k] = point[k] - center[k];
	for (k = 0; k < 3; k ++)
            for (l = 0; l < 3; l ++)
		cv[k][l] += (DP)(a[k] * a[l]);
    }
    NR::jacobi(cv, d, v, nrot);
    NR::eigsrt(d, v);
    Vector3f normal1(v[0][2], v[1][2], v[2][2]);
    normal1.normalize();
    Vector3f normal2(v[0][1], v[1][1], v[2][1]);
    normal2.normalize();
    Vector3f normal3(v[0][0], v[1][0], v[2][0]);
    normal3.normalize();

    return normal1;
}

// Propagate the points from the max/min x/y/z to do the normal consistence.
void MarchingCube::normalConsist(){
    // create the Remaining Graph
    Kruskal kg;
    for (int n = 0; n < POINTSAMOUNT; n ++){
        Node node(n);
        kg.addNode(node);
        int *nghb = POINTCLOUD[n].getKnghb();
        int startNode = n;
        for (int i = 0; i < K; i ++){
            int endNode = nghb[i];
            if (endNode >= 0){
                float weight = 1.0 - fabs((POINTCLOUD[n].getEigenNormal()).dot(
                            POINTCLOUD[endNode].getEigenNormal()));
                Edge e(startNode, endNode, weight);
                kg.addEdge(e);
            }
        }
    }
    kg.mtsKruskal();

    qDebug() << endl << "consisting... ";
    if (consistStart > 2){
        qDebug() << "from " << maxIndex[consistStart - 3] << endl;
        consist(maxIndex[consistStart - 3], stdNormals[consistStart], kg);
    }
    else{
        qDebug() << "from " << minIndex[consistStart] << endl;
        consist(minIndex[consistStart], stdNormals[consistStart], kg); 
    } 

    if ((consistStart + 3 ) % 6 > 2){
        qDebug() << "from " << maxIndex[(consistStart + 3) % 6 - 3] << endl;
        consist(maxIndex[(consistStart + 3) % 6 - 3], 
                stdNormals[(consistStart + 3) % 6 - 3], kg);
    }
    else{
        qDebug() << "from " << minIndex[(consistStart + 3) % 6] << endl;
        consist(minIndex[(consistStart + 3) % 6], 
                stdNormals[(consistStart + 3) % 6], kg);
    } 

    qDebug() << "\n" << "creating eigen normal file: " << eigenNormalFileName.c_str()     << "\n";
	eigenNormalFileName="c:\\engenNormal.txt";
    ofstream ef(eigenNormalFileName.c_str());
   // assert(ef);
    for (int i = 0; i < POINTSAMOUNT; i ++){
        ef << POINTCLOUD[i].getEigenNormal()[0] << ' '
           << POINTCLOUD[i].getEigenNormal()[1] << ' '
           << POINTCLOUD[i].getEigenNormal()[2] << endl;
    }
    ef.close();
    qDebug() << "eigen normal file created." << endl;

}

void MarchingCube::consist(int index, Vector3f normal, Kruskal &kg){
    if (POINTCLOUD[index].getNormalConsistent())
        return;
    Vector3f eigenNormal = POINTCLOUD[index].getEigenNormal();
    if (eigenNormal.dot(normal) < -e){
        eigenNormal.negate();
        POINTCLOUD[index].setEigenNormal(eigenNormal);
    }
    POINTCLOUD[index].setNormalConsistent();
    vector<int> connectNodes = kg.getConnectNodes(index);
    for (int i = 0; i < (int)connectNodes.size(); i ++){
        consist(connectNodes[i], eigenNormal, kg);
    }
}

void MarchingCube::OutputMesh(vector<vector<float>>& vlist, vector<vector<float>>& vnlist, vector<vector<int>>& flist)
{
;
}


}//namespace