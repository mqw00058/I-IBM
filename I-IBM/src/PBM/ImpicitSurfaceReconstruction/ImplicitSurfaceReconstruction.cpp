#ifndef IMPLICIT_SURFACE_RECONSTRUCTION_CC
#define IMPLICIT_SURFACE_RECONSTRUCTION_CC

#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);


#include "ImplicitSurfaceReconstruction.h"
//#include <pcl/io/pcd_io.h>

//#include <pcl/common/io.h>
//#include <pcl/keypoints/sift_keypoint.h>

//

namespace PBM {
	namespace ImplicitSurfaceReconstruction{
			//-----------------------------------------------------------------------------
		void ImplicitSurfaceReconstructionT::LoadPoints(std::vector<float>* vec_sdf)
		{
			int i = 0;
			float p[3];
			float minp[] = { 1.0, 1.0, 1.0 };
			float maxp[] = { -1.0, -1.0, -1.0 };
			// transfer the center of the points to (0, 0, 0); scale the points to
			// (-0.5, -0.5, -0.5) ~ (0.5, 0.5, 0.5); store the points.
			POINTSAMOUNT = mesh_->n_vertices();
			POINTCLOUD = new MarchingCubeNameSpace::Point[POINTSAMOUNT];

			// geometry property setting
			Mesh::VertexIter	v_it, v_end(mesh_->vertices_end());
	
			for (v_it = mesh_->vertices_begin(); v_it != v_end; ++v_it){

				p[0] = mesh_->point(v_it)[0];
				p[1] = mesh_->point(v_it)[1];
				p[2] = mesh_->point(v_it)[2];

				for (int j = 0; j < 3; j++){
					if (p[j] < minp[j]){
						minp[j] = p[j];
						minIndex[j] = i;
					}
					if (p[j] > maxp[j]){
						maxp[j] = p[j];
						maxIndex[j] = i;
					}
				}
				MarchingCubeNameSpace::Point pp = MarchingCubeNameSpace::Point(MarchingCubeNameSpace::Vector3f(p));
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   				if (vec_sdf->size() > 0)
				pp.sdf = (*vec_sdf)[i];
				pp.inputsdf = true;
				POINTCLOUD[i] = pp;
				i++;
			}



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

			//ifstream a("c:\\pointcloud.txt");
			//if(a)
			//	qDebug() << "a opened"<<endl;


			cloud->width = POINTSAMOUNT;
			cloud->height = 1;
			cloud->is_dense = true;
			cloud->points.resize(cloud->width * cloud->height);

			for (int i = 0; i < POINTSAMOUNT; i++){
				float newp[3];
				MarchingCubeNameSpace::Vector3f p = POINTCLOUD[i].getPosition();
				for (int j = 0; j < 3; j++)
					newp[j] = (p[j] - trans[j]) / scale;

				//								a >> newp[0] >>  newp[1] >>  newp[2];
				POINTCLOUD[i] = MarchingCubeNameSpace::Point(MarchingCubeNameSpace::Vector3f(newp));
				point_set.push_back(Vec3f(newp));

				cloud->points[i].x = newp[0];
				cloud->points[i].y = newp[1];
				cloud->points[i].z = newp[2];

			}
			//a.close();
		}

		//void ComputeNeighbors();
		void ImplicitSurfaceReconstructionT::ComputeNormals(int nMax)
		{
			////////////////////////////////////////////////////////////
			/*int nPts = POINTSAMOUNT;
			NE *ne	= new NE(point_set);

			// for neighborhood
			//nMax = 8;

			int *knn;		// k-nearest neighbor index;
			int *nn;		// the number of neighbor
			knn		= new int [nMax*nPts];
			nn		= new int [nPts];


			for(int i=0; i<nPts; ++i){
			// neighborhood computation
			//nn[i] = ne->k_nearest_neighbor(i, nMax, knn + i*nMax);
			ne->k_nearest_neighbor(i, nMax,  knn + i*nMax);

			for(int j=0; j<nMax; j++)
			{
			nn[j] = knn[i*nMax+j];
			}

			POINTCLOUD[i].setKnghb(nn);
			}



			ne->compute_normal(nMax);
			for(int i=0; i<nPts; ++i)
			{
			MarchingCubeNameSpace::Vector3f n;
			n[0]=ne->normalVector(i)[0];
			n[1]=ne->normalVector(i)[1];
			n[2]=ne->normalVector(i)[2];

			POINTCLOUD[i].setCenter(MarchingCubeNameSpace::Vector3f(ne->centroid[i].x[0], ne->centroid[i].x[1], ne->centroid[i].x[2]));
			POINTCLOUD[i].setEigenNormal(n);
			}

			delete [] ne;
			delete [] nn;
			delete [] knn;*/
			/////////////////////////////////////////////////////////////////////////////

			// Create the normal estimation class, and pass the input dataset to it
			pcl::NormalEstimation<pcl::PointXYZ, pcl::PointNormal> ne;
			ne.setInputCloud(cloud);

			// Create an empty kdtree representation, and pass it to the normal estimation object.
			// Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
			pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
			ne.setSearchMethod(tree);

			// Output datasets
			pcl::PointCloud<pcl::PointNormal>::Ptr cloud_normals(new pcl::PointCloud<pcl::PointNormal>);
			// Use all neighbors in a sphere of radius 3cm
			//ne.setRadiusSearch(0.03);
			ne.setKSearch(8);
			// Compute the features
			ne.compute(*cloud_normals);

			Eigen::Vector4f centroid;
			pcl::compute3DCentroid(*cloud, centroid);


			// cloud_normals->points.size () should have the same size as the input cloud->points.size ()*
			MarchingCubeNameSpace::Vector3f n;
			for (int i = 0; i < cloud_normals->points.size(); i++)
			{
				POINTCLOUD[i].setCenter(MarchingCubeNameSpace::Vector3f(centroid[0], centroid[1], centroid[2]));
				n[0] = cloud_normals->points[i].normal_x;
				n[1] = cloud_normals->points[i].normal_y;
				n[2] = cloud_normals->points[i].normal_z;

				POINTCLOUD[i].setEigenNormal(n);
			}
		}
		
		void ImplicitSurfaceReconstructionT::SurfaceGeneration()
		{
			MarchingCubeNameSpace::MarchingCube cube = MarchingCubeNameSpace::MarchingCube();
			cube.seperatingPoints(false);
			cube.computeNeighbors();
			// if (!(scanNormal && useScanNormal))
			//	 cube.normalConsist(); 


			//ofstream centernormal("centernormals.txt");
			//for(int i=0; i<POINTSAMOUNT; i++)
			//{
			//	centernormal << POINTCLOUD[i].getEigenNormal()[0] << "," << POINTCLOUD[i].getEigenNormal()[1] << "," << POINTCLOUD[i].getEigenNormal()[2] << \
								//		"    " << POINTCLOUD[i].getCenter()[0] << "," << POINTCLOUD[i].getCenter()[1] << "," << POINTCLOUD[i].getCenter()[2] << endl;
			//}
			//centernormal.close();

			//ComputeNeighbors();
			//ComputeNormals(8);
			

			cube.normalConsist();
			cube.MarchingCubes();


			triangleVertices = cube.getTriangleVertices();
			triangleNormals = cube.getTriangleNormals();
			triangleConnection = cube.getTriangleConnection();

			OpenMesh::IO::ImporterT<Mesh> importer(*mesh_);

			for (int i = 0; i < (int)triangleVertices.size(); i++)
			{
				VertexHandle Vh = importer.add_vertex(OpenMesh::Vec3f(triangleVertices[i][0], triangleVertices[i][1], triangleVertices[i][2]));
				importer.set_normal(Vh, OpenMesh::Vec3f(triangleNormals[i][0], triangleNormals[i][1], triangleNormals[i][2]));
			}

			for (int i = 0; i < (int)triangleConnection.size(); i++)
			{
				vector<VertexHandle> vhandles;

				vhandles.push_back(VertexHandle(triangleConnection[i][2]));
				vhandles.push_back(VertexHandle(triangleConnection[i][1]));
				vhandles.push_back(VertexHandle(triangleConnection[i][0]));

				importer.add_face(vhandles);
			}

			//	mesh_->garbage_collection();

			//	mesh_->request_vertex_status();
			//	mesh_->request_face_status();

			//std::map<VertexHandle,OpenMesh::Vec3f>::iterator v_nMap_it;
			//	for(v_nMap_it=v_nMap.begin(); v_nMap_it!=v_nMap.end(); ++v_nMap_it)
			//	mesh_->set_normal(v_nMap_it->first,v_nMap_it->second);
			//cube.OutputMesh(vlist, vnlist, flist);
			//for (int i = 0; i < (int)triangleNormals.size(); i++)



			/*meshFileName = "c:\\mesh";
			offMeshFileName = "c:\\mesh.off";
			MarchingCube::writeOffMeshFile(cube.getTriangleVertices(), cube.getTriangleConnection());
			MarchingCube::writeMeshFile(cube.getTriangleVertices(), cube.getTriangleNormals(),
			cube.getTriangleConnection());
			*/
		}


		void ImplicitSurfaceReconstructionT::OutputToOpenMesh()
		{

			/*	for(int i=0; i<vlist.size(); ++i)
			{
			vector<float> v=vlist[i];

			VertexHandle Vh=mesh_->add_vertex(OpenMesh::Vec3f(v[0],v[1],v[2]));
			vector<float> vn=vnlist[i];

			mesh_->set_normal(Vh,OpenMesh::Vec3f(vn[0],vn[1],vn[2]));
			}


			for(int j=0; j<flist.size(); ++j)
			{
			vector<VertexHandle> vhandles;

			vector<int> v1v2v3=flist[j];
			vhandles.push_back(VertexHandle(v1v2v3[0]));
			vhandles.push_back(VertexHandle(v1v2v3[1]));
			vhandles.push_back(VertexHandle(v1v2v3[2]));

			mesh_->add_face(vhandles);


			}*/
		}


		/*	template <class Mesh>
		void ImplicitSurfaceReconstructionT<Mesh>::ComputeNeighbors()
		{

		nPts	= POINTSAMOUNT;
		ne		= new NE(point_set);

		// for neighborhood
		nMax = 8;
		knn		= new int [nMax*nPts];
		nn		= new int [nPts];

		ofstream nb("nbs.txt");
		for(int i=0; i<nPts; ++i){
		// neighborhood computation
		//nn[i] = ne->k_nearest_neighbor(i, nMax, knn + i*nMax);
		ne->k_nearest_neighbor(i, nMax,  knn + i*nMax);
		nb << i << "    ";
		for(int j=0; j<nMax; j++)
		{
		nn[j] = knn[i*nMax+j];
		nb << " " << nn[j]; 
		}
		nb << endl;
		POINTCLOUD[i].setKnghb(nn);

		}
		nb.close();
		}*/

	
		

	}//namespace ImplicitSurfaceReconstruction
}//namespace PBM
#endif