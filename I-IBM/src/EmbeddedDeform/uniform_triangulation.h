#pragma once
#include <opencv2/core/core.hpp>
// -------------------- OpenMesh

#include "../src/Geometry3D/Geometry3D.h"

class Index2
{
public:
	int i, j;
	Index2() { i = j = -1; }
	Index2(int i_, int j_) { i = i_; j = j_; }
};

class Index3
{
public:
	int i, j, k;
	Index3() { i = j = k = -1; }
	Index3(int i_, int j_, int k_) { i = i_; j = j_; k = k_; }
};

class TriangleMesh
{
public:
	std::vector<cv::Vec3f> vertices;
	std::vector<Index3> triangles;
	cv::Mat vmap;
public:
	TriangleMesh(){}
	TriangleMesh(std::vector<cv::Vec3f> vertices, std::vector<Index3> triangles, cv::Mat vmap)
	{
		this->vertices = vertices;
		this->triangles = triangles;
		this->vmap = vmap;
	}
};


class uniform_triangulation
{
public:
	uniform_triangulation();
	TriangleMesh triangulate(cv::Mat& pointcloud, int stepSampling=1, float minEdgeLength=0.15);
	~uniform_triangulation();
	void open_xml_dir(const char* dir);
	void from_TriangleMesh_to_OpenMesh(TriangleMesh *trimesh, GeoTriMesh &mesh);
	void save(std::string filename);
private:
		std::vector<cv::Vec3f> vertices;
		std::vector<Index3> triangles;

		//std::vector<cv::Vec3f> vertices;
		//std::vector<Index3> triangles;
		cv::Mat vmap;
};

