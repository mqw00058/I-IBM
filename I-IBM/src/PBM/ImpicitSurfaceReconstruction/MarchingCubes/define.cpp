/* File Name:   define.cpp
 * Description: Implementing the read point cloud functions.
 * Author:      Bo Gao
 * Date:        September 27, 2007
 */

#include "define.h"

#define setMin(coord, index){ \
	if (coord < minp[index]){ \
	minp[index] = coord; \
	minIndex[index] = j; \
	} \
};

#define setMax(coord, index){ \
	if (coord > maxp[index]){ \
	maxp[index] = coord; \
	maxIndex[index] = j; \
	} \
};

MarchingCubes::Point *POINTCLOUD;
int POINTSAMOUNT;
int maxIndex[] = {0, 0, 0};
int minIndex[] = {0, 0, 0};

string pointFileName;    // point cloud file name
string meshFileName;     // mesh triangle file name
string offMeshFileName;  // off format mesh triangle file name
string gridFileName;     // separated points index file name
string neighborFileName; // K-neighbor of each point file name
string eigenNormalFileName;  // eigen normal file name

namespace MarchingCubes
{


// list of property information for a vertex
PlyProperty vert_props[] = {
    {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, x), 0, 0, 0, 0},
    {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, y), 0, 0, 0, 0},
    {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, z), 0, 0, 0, 0},
};

// list of property information for a normal 
PlyProperty normal_props[] = {
    {"nx", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, x), 0, 0, 0, 0},
    {"ny", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, y), 0, 0, 0, 0},
    {"nz", PLY_FLOAT, PLY_FLOAT, offsetof(Vector, z), 0, 0, 0, 0},
};

string pointFileName;    // point cloud file name
string meshFileName;     // mesh triangle file name
string offMeshFileName;  // off format mesh triangle file name
string gridFileName;     // separated points index file name
string neighborFileName; // K-neighbor of each point file name
string eigenNormalFileName; // eigen normal file name


void ReadPoints(){
    string dummy;
    float p[3];
    int pointNumber;
    float minp[] = {1.0, 1.0, 1.0};
    float maxp[] = {-1.0, -1.0, -1.0};

    // read point file;
    ifstream sfp(pointFileName.c_str());
    if(sfp.fail()){
        qDebug() << "Point file not found." << endl;
        exit(0);
    }

    getline(sfp, dummy);
    sfp >> pointNumber;
    qDebug() << "Point number: " << pointNumber << endl;
    Vector3f *points = new Vector3f[pointNumber];
    Vector3f *normals = new Vector3f[pointNumber];

    for (int i = 0; i < pointNumber; i ++){
	sfp >> p[0] >> p[1] >> p[2];
        for (int j = 0; j < 3; j ++){
            if (p[j] < minp[j]){
                minp[j] = p[j];
                minIndex[j] = i;
            }
            if (p[j] > maxp[j]){
                maxp[j] = p[j];
                maxIndex[j] = i;
            }
        }
	points[i] = Vector3f(p);
    }
    for (int i = 0; i < pointNumber; i ++){
        sfp >> p[0] >> p[1] >> p[2];
        normals[i] = Vector3f(p) + points[i];
    } 
    sfp.close();
//    qDebug() << "finished reading point file." << endl;

    // transfer the center of the points to (0, 0, 0); scale the points to
    // (-0.5, -0.5, -0.5) ~ (0.5, 0.5, 0.5); store the points.
    float trans[3];
    float scaleArray[3];
    for (int i = 0; i < 3; i++){
        trans[i] = (maxp[i] + minp[i]) / 2.0;
        scaleArray[i] = (maxp[i] - minp[i]) / 1.0;
    }

    float scale = (scaleArray[0] > scaleArray[1]) ? scaleArray[0] : scaleArray[1];
    scale = (scale > scaleArray[2]) ? scale : scaleArray[2];

    POINTSAMOUNT = pointNumber;
    POINTCLOUD = new Point[pointNumber];

    for (int i = 0; i < pointNumber; i++){
        float newp[3];
        for (int j = 0; j < 3; j ++)
            newp[j] = (points[i][j] - trans[j]) / scale;
            
        Point p = Point(Vector3f(newp));
        for (int j = 0; j < 3; j ++)
            newp[j] = (normals[i][j] - trans[j]) / scale;
        Vector3f normal = Vector3f(newp);
        normal = normal - p.getPosition();
        normal.normalize();
        p.setNormal(normal); 
        POINTCLOUD[i] = p;
    }

    qDebug() << "finished storing poings." << endl;
    delete[] points;
    delete[] normals;
}

bool readPly(){
    PlyFile *ply;
    int nelems;
    char **elist;
    int file_type;
    float version;
    int nprops;
    int num_elems;
    PlyProperty **plist;
    char *elem_name;
    int pointNumber;
    int normalNumber;
    float minp[] = {1.0, 1.0, 1.0};
    float maxp[] = {-1.0, -1.0, -1.0};
    Vector *points, *normals;
    char plyFileName[20];
    int length = pointFileName.copy(plyFileName, pointFileName.size());
    plyFileName[length] = '\0';
    bool scanNormal = false;

    // open a PLY file for reading
    ply = ply_open_for_reading(plyFileName, &nelems, &elist, &file_type, &version);
    // print what we found out about the file
    qDebug() << "point cloud file: " << plyFileName << endl;
    qDebug() << "version " << version << endl;
    qDebug() << "type " << file_type << endl;

    // go through each kind of element that we learned is in the file
    // and read them
    for (int i = 0; i < nelems; i++){
        // get the description of the first element
	elem_name = elist[i];
	plist = ply_get_element_description(ply, elem_name, &num_elems, &nprops);

	// print the name of the element, for debugging
        qDebug() << "element " << elem_name << ' ' << num_elems << endl;
	
	// if we're on vertex elements, read them in
	if (equal_strings ("vertex", elem_name)){
	    // create a vertex list to hold all the vertices
            pointNumber = num_elems;
            points = new Vector[pointNumber];

	    // set up for getting vertex elements
	    ply_get_property(ply, elem_name, &vert_props[0]);
	    ply_get_property(ply, elem_name, &vert_props[1]);
	    ply_get_property(ply, elem_name, &vert_props[2]);

	    // grab all the vertex elements
	    for (int j = 0; j < num_elems; j++){
	        // grab an element from the file
                Vector *point = new Vector;
		ply_get_element(ply, (void *)point);
//                qDebug() << "point[" << j << "] : (" << point->x << ' ' << point->y << ' ' << point->z << endl;
                setMin(point->x, offsetof(Vector, x) / 4);
                setMin(point->y, offsetof(Vector, y) / 4);
                setMin(point->z, offsetof(Vector, z) / 4);
                setMax(point->x, offsetof(Vector, x) / 4);
                setMax(point->y, offsetof(Vector, y) / 4);
                setMax(point->z, offsetof(Vector, z) / 4);
                points[j].x = point->x;
                points[j].y = point->y;
                points[j].z = point->z;
                delete point;
	    }
	}
        if (equal_strings ("normal", elem_name)){
            // create a normal list to hold all the normals
            scanNormal = true;
            normalNumber = num_elems;
            normals = new Vector[normalNumber];

            // set up for getting vertex elements
	    ply_get_property(ply, elem_name, &normal_props[0]);
	    ply_get_property(ply, elem_name, &normal_props[1]);
	    ply_get_property(ply, elem_name, &normal_props[2]);

	    // grab all the vertex elements
	    for (int j = 0; j < num_elems; j++){
	        // grab an element from the file
                Vector *normal = new Vector;
		ply_get_element(ply, (void *)normal);
//                qDebug() << "normal[" << j << "] : (" << normal->x << ' ' << normal->y << ' ' << normal->z << endl;
                normals[j].x = normal->x + points[j].x;
                normals[j].y = normal->y + points[j].y;
                normals[j].z = normal->z + points[j].z;
                delete normal;
	    }
	}
    }
    // close the PLY file
    ply_close (ply);

    // transfer the center of the points to (0, 0, 0); scale the points to
    // (-0.5, -0.5, -0.5) ~ (0.5, 0.5, 0.5); store the points.
    float trans[3];
    float scaleArray[3];
    for (int i = 0; i < 3; i++){
        trans[i] = (maxp[i] + minp[i]) / 2.0;
        scaleArray[i] = (maxp[i] - minp[i]) / 1.0;
    }

    float scale = (scaleArray[0] > scaleArray[1]) ? scaleArray[0] : scaleArray[1];
    scale = (scale > scaleArray[2]) ? scale : scaleArray[2];

    POINTSAMOUNT = pointNumber;
    POINTCLOUD = new Point[pointNumber];

    for (int i = 0; i < pointNumber; i++){
        float newp[3];
        newp[0] = (points[i].x - trans[0]) / scale;
        newp[1] = (points[i].y - trans[1]) / scale;
        newp[2] = (points[i].z - trans[2]) / scale;
//        qDebug() << "point[" << i << "] : (" << newp[0] << ", " << newp[1] << ", " << newp[2] << ")" << endl; 
        Point p = Point(Vector3f(newp));
        if(scanNormal){
            newp[0] = (normals[i].x - trans[0]) / scale;
            newp[1] = (normals[i].y - trans[1]) / scale;
            newp[2] = (normals[i].z - trans[2]) / scale;

            Vector3f normal = Vector3f(newp);
            normal = normal - p.getPosition();
            normal.normalize();
            p.setNormal(normal);
        } 
        POINTCLOUD[i] = p;
    }

    qDebug() << "finished storing points." << endl;
    delete[] points;
    if (scanNormal)
        delete[] normals;

    return scanNormal;
}

void writeOffMeshFile(Vec3fvector triangleVertices, Intvector triangleConnection){
	offMeshFileName="mesh.off";
    ofstream triangleFile(offMeshFileName.c_str());
    if(!triangleFile){
	qDebug() << "Cannot open off mesh triangle file." << endl;
	return;
    }
    qDebug() << "created " << (int)triangleConnection.size() << " triangles" << endl;
    triangleFile << "OFF" << endl;
    triangleFile << (int)triangleVertices.size() << ' ' << (int)triangleConnection.size() << ' ' << 0 << endl;
    for (int i = 0; i < (int)triangleVertices.size(); i ++){
        triangleFile << triangleVertices[i][0] << ' '
                     << triangleVertices[i][1] << ' '
                     << triangleVertices[i][2] << endl;
    }
    for (int i = 0; i < (int)triangleConnection.size(); i ++)
        triangleFile << 3 << ' '
                     << triangleConnection[i][0] << ' '
                     << triangleConnection[i][1] << ' '
                     << triangleConnection[i][2] << endl;
    triangleFile.close();
}

void writeMeshFile(Vec3fvector triangleVertices, Vec3fvector triangleNormals,
        Intvector triangleConnection){
    ofstream triangleFile(meshFileName.c_str());
    if(!triangleFile){
	qDebug() << "Cannot open mesh triangle file." << endl;
	return;
    }
    triangleFile << (int)triangleVertices.size() << ' ' << (int)triangleConnection.size() << endl;
    for (int i = 0; i < (int)triangleVertices.size(); i ++){
        triangleFile << triangleVertices[i][0] << ' '
                     << triangleVertices[i][1] << ' '
                     << triangleVertices[i][2] << endl;
        triangleFile << triangleNormals[i][0] << ' '
                     << triangleNormals[i][1] << ' '
                     << triangleNormals[i][2] << endl;
    }
    for (int i = 0; i < (int)triangleConnection.size(); i ++)
        triangleFile << 3 << ' '
                     << triangleConnection[i][0] << ' '
                     << triangleConnection[i][1] << ' '
                     << triangleConnection[i][2] << endl;
    triangleFile.close();
}

void createFileName(){
    while ((int)pointFileName.size() == 0){
        cerr << "Please input the point cloud file name: ";
        cin >> pointFileName;
    }
    string pureFileName = pointFileName;
    string::size_type loc = pointFileName.find(".ply", 0);
    if (loc != string::npos)
        pureFileName.erase(loc);
    meshFileName = pureFileName + "_mesh";
    offMeshFileName = pureFileName + ".off";
    gridFileName = pureFileName + "_grid";
    neighborFileName = pureFileName + "_neighbor";
    eigenNormalFileName = pureFileName + "_eigenNormal";
}


}