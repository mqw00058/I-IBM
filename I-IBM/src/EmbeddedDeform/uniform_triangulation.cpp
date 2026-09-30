
#include "../../globals.h"

#include <fstream>
#include <boost/filesystem.hpp>

#include "uniform_triangulation.h"



uniform_triangulation::uniform_triangulation()
{
}

void uniform_triangulation::open_xml_dir(const char* dirpath)
{
	boost::filesystem::path dir(dirpath);

	cv::Mat matWorldCoord;
	cv::FileStorage fs;

	if (boost::filesystem::exists(dir))
	{
		if (boost::filesystem::is_directory(dir))
		{
			boost::filesystem::directory_iterator end;
			for (boost::filesystem::directory_iterator dir_iter(dir); dir_iter != end; ++dir_iter)
			{
				if (dir_iter->path().extension().string() == ".xml")
				{
					if (fs.open(dir_iter->path().string(), cv::FileStorage::READ))
					{
						fs["WorldCoordinate"] >> matWorldCoord;

						logviewdockwidget->addText(QString::fromStdString(dir_iter->path().string()) + QString("\n"));
						logviewdockwidget->addText(QString::number(matWorldCoord.rows) + " " + QString::number(matWorldCoord.cols) + QString("\n"));
						
						TriangleMesh trimesh = triangulate(matWorldCoord,5, 100);
						boost::filesystem::path outfile = dir_iter->path();
						outfile.replace_extension(boost::filesystem::path("ply"));
						save(outfile.string());
					}
					fs.release();
				}
			}
		}
	}

}

void uniform_triangulation::from_TriangleMesh_to_OpenMesh(TriangleMesh *trimesh, GeoTriMesh &mesh)
{
	mesh.release_face_normals();
	mesh.release_vertex_normals();
	mesh.release_vertex_colors();
	mesh.release_face_status();
	mesh.release_edge_status();
	mesh.release_halfedge_status();
	mesh.release_vertex_status();

	// request and update normals
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.request_face_status();
	mesh.request_edge_status();
	mesh.request_vertex_status();
	mesh.request_halfedge_status();

	OpenMesh::VertexHandle* v_handles = new OpenMesh::VertexHandle[trimesh->vertices.size()];

	for (int v = 0; v < trimesh->vertices.size(); ++v)
	{
		v_handles[v] = mesh.add_vertex(GeoTriMesh::Point(trimesh->vertices[v][0], trimesh->vertices[v][1], trimesh->vertices[v][2]));
	}
	for (int f = 0; f < trimesh->triangles.size(); ++f)
	{
		std::vector<OpenMesh::VertexHandle> face_vhandles;
		face_vhandles.clear();
		face_vhandles.push_back(v_handles[trimesh->triangles[f].i]);
		face_vhandles.push_back(v_handles[trimesh->triangles[f].j]);
		face_vhandles.push_back(v_handles[trimesh->triangles[f].k]);

		mesh.add_face(face_vhandles);
		qDebug() << mesh.n_vertices();
	}
	delete[] v_handles;

	GeoTriMesh::ConstVertexIter v_iter;

	for (v_iter = mesh.vertices_begin(); v_iter != mesh.vertices_end(); ++v_iter)
	{
		GeoTriMesh::VertexHandle vh(v_iter);
		GeoTriMesh::Normal n = mesh.calc_vertex_normal(vh);
		mesh.set_normal(vh, n);
	}

	GeoTriMesh::ConstFaceIter f_iter;

	for (f_iter = mesh.faces_begin(); f_iter != mesh.faces_end(); ++f_iter)
	{
		GeoTriMesh::FaceHandle fh(f_iter);
		GeoTriMesh::Normal n = mesh.calc_face_normal(fh);
		mesh.set_normal(fh, n);
	}
	mesh.garbage_collection();
	mesh.update_normals();


}
float dist(cv::Vec3f p0, cv::Vec3f p1)
{
	cv::Vec3f diff = p0 - p1;
	return sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
}

TriangleMesh uniform_triangulation::triangulate(cv::Mat& pointcloud, int stepSampling, float minEdgeLength)
{
	cv::Mat_<cv::Vec3f> _mat = pointcloud;

	if (vertices.size() > 0) vertices.clear();
	if (triangles.size() > 0) triangles.clear();
	if (vmap.size().width >0)  vmap.release();
	vmap = cv::Mat(pointcloud.size(), CV_32S); // vertex index map in the image
	vmap = cv::Scalar(-1);

	//FILE *fp;
	//fp = fopen("test.txt", "w");
	

	for (int i = 0; i < (pointcloud.rows-stepSampling); i=i+stepSampling)
	{
		for (int j = 0; j < (pointcloud.cols - stepSampling); j=j+stepSampling)
		{
			if (_mat(i, j)[2] >0.0)
			{
				Index2 i0 = Index2(i, j);
				Index2 i1 = Index2(i, j + 5);
				Index2 i2 = Index2(i + 5, j);
				Index2 i3 = Index2(i + 5, j + 5);

				cv::Vec3f p0 = _mat(i0.i, i0.j);
				cv::Vec3f p1 = _mat(i1.i, i1.j);;
				cv::Vec3f p2 = _mat(i2.i, i2.j);
				cv::Vec3f p3 = _mat(i3.i, i3.j);

				int v0i = vmap.at< int >(i0.i, i0.j);
				int v1i = vmap.at< int >(i1.i, i1.j);
				int v2i = vmap.at< int >(i2.i, i2.j);
				int v3i = vmap.at< int >(i3.i, i3.j);

				// p0 - p1
				//  | / 
				// p2
				if (minEdgeLength && dist(p1, p2) < minEdgeLength && dist(p2, p0) < minEdgeLength)
				{
					if (v0i == -1)
					{
						v0i = vertices.size();
						vertices.push_back(p0);
						vmap.at< int >(i0.i, i0.j) = v0i;

//						fprintf(fp,"%d,%d=%d\n", i0.i, i0.j, vmap.at< int >(i0.i, i0.j));
						
					}
					if (v1i == -1)
					{
						v1i = vertices.size();
						vertices.push_back(p1);
						vmap.at< int >(i1.i, i1.j) = v1i;
	//					fprintf(fp, "%d,%d=%d\n", i1.i, i1.j, vmap.at< int >(i1.i, i1.j));
					}
					if (v2i == -1)
					{
						v2i = vertices.size();
						vertices.push_back(p2);
						vmap.at< int >(i2.i, i2.j) = v2i;
			//			fprintf(fp, "%d,%d=%d\n", i2.i, i2.j, vmap.at< int >(i2.i, i2.j));
					}

					triangles.push_back(Index3(v0i, v2i, v1i));
					
				}


				//      p1
				//    / |
				// p2 - p3
				if (dist(p1, p2) < minEdgeLength && dist(p2, p3) < minEdgeLength && dist(p3, p1) < minEdgeLength )
				{
					if (v1i == -1)
					{
						v1i = vertices.size();
						vertices.push_back(p1);
						vmap.at< int >(i1.i, i1.j) = v1i;
				//		fprintf(fp, "%d,%d=%d\n", i1.i, i1.j, vmap.at< int >(i1.i, i1.j));
					}
					if (v3i == -1)
					{
						v3i = vertices.size();
						vertices.push_back(p3);
						vmap.at< int >(i3.i, i3.j) = v3i;
					//	fprintf(fp, "%d,%d=%d\n", i3.i, i3.j, vmap.at< int >(i3.i, i3.j));
					}
					if (v2i == -1)
					{
						v2i = vertices.size();
						vertices.push_back(p2);
						vmap.at< int >(i2.i, i2.j) = v2i;
					//	fprintf(fp, "%d,%d=%d\n", i2.i, i2.j, vmap.at< int >(i2.i, i2.j));
					}
					triangles.push_back(Index3(v1i, v2i, v3i));
				}							
			}
		}
	}

	//fclose(fp);

	TriangleMesh a(vertices, triangles, vmap);

	//int pi = vmap.at< int >(795, 352);
	//int qi = vmap.at<int>(830, 341);

	return a;
}

void uniform_triangulation::save(std::string filename)
{

	std::ofstream ply_file;
	std::ofstream vmap_file;
	ply_file.open(filename);
	std::string::size_type findpos = filename.find(std::string(".ply"));
	filename.replace(findpos, std::string(".ply").size(), std::string(".vmap"));
	vmap_file.open(filename);

	ply_file << "ply" << std::endl
		<< "format ascii 1.0" << std::endl
		<< "element vertex " << vertices.size() << std::endl
		<< "property float x" << std::endl
		<< "property float y" << std::endl
		<< "property float z" << std::endl
		<< "element face " << triangles.size() << std::endl
		<< "property list uchar int vertex_indices" << std::endl
		<< "end_header" << std::endl;

	for (int i = 0; i < vertices.size(); i++)
	{
		ply_file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;
	}

	for (int i = 0; i < triangles.size(); i++)
		ply_file << "3 " << triangles[i].i << " " << triangles[i].j << " " << triangles[i].k << std::endl;

	ply_file.clear();

	for (int i = 0; i < vmap.rows; i++)
	{
		for (int j = 0; j < vmap.cols; j++)
		{
			vmap_file << i << " , " << j << " " << vmap.at<int>(i, j) << std::endl;

		}
	}
	vmap_file.close();
}

uniform_triangulation::~uniform_triangulation()
{
}
