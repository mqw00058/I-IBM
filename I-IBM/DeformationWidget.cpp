#ifndef DEFORMATIONWIDGET_CPP
#define DEFORMATIONWIDGET_CPP

//#include <pcl/point_types.h>
//#include <pcl/features/normal_3d.h>
#define NOMINMAX

#include "src/GLWidget/GLOptionwidget.h"

#include "src/EmbeddedDeform/genGraphDialog.h"
#include "src/EmbeddedDeform/uniform_triangulation.h"


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "CorrespondenceOpenCV.h"

//#include "MeshRecon.h"
#include "DeformationWidget.h"



//currentTriMesh = std::vector<Geometry>
DeformationWidget::DeformationWidget(MainWindow* parent) 
{
	ui.setupUi(this);
	this->setParent(parent);

	mainwindow = (MainWindow*)parent;
	//int a = mainwindow->m_TriMeshs.size();
	//int b = getMainWindow()->m_TriMeshs.size();
	for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
	{
		mainwindow->m_TriMeshs[i]->convert_Geo3DMesh_DeformableMesh3d();
	}
	deforming_ModelID = -1;
	src_ModelId = -1;

	mainwindow->GetGlWidget()->m_bDrawWorkspace = false;
	//glView->m_bDrawAxis = false;
	mainwindow->GetGlWidget()->setFocusPolicy(Qt::ClickFocus);
}

DeformationWidget::~DeformationWidget()
{

}
/*
template<class T, class T2>
void DeformationWidgetT<T, T2>::select_currentModel()
{
	current_model_number = ui.listWidget->currentRow();

	ext_current_model_index = current_model_number;
}
*/

void DeformationWidget::slot_OpenXMLDir()
{
	QString dirpath = QFileDialog::getExistingDirectory(this, "Open XMLs", "../models", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	

	QDir dir;
	dir.setPath(dirpath);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	dir.setNameFilters(QStringList() << "*.xml");

	QStringList fileList = dir.entryList();

	uniform_triangulation * ut = new uniform_triangulation;

	cv::Mat matWorldCoord;
	cv::FileStorage fs;
	std::string texture_image;
	for (int i = 0; i < fileList.size(); i++)
	{	
		QString newpath = QString("%1/%2").arg(dir.absolutePath()).arg(fileList.at(i));
		//qDebug() << newpath;
		if (fs.open(newpath.toStdString(), cv::FileStorage::READ))
		{
   			fs["texture_image"] >> texture_image;
			if (texture_image.empty())
			{
				QMessageBox::information(this, "xml error", "NO texture image");
				return;
			}

			fs["WorldCoordinate"] >> matWorldCoord;
			if(matWorldCoord.empty())
				fs["KinectCoordinate"] >> matWorldCoord;
			if (matWorldCoord.empty())
				fs["LotatedXml"] >> matWorldCoord;
			
			//logviewdockwidget->addText(QString::fromStdString(dir_iter->path().string()) + QString("\n"));
			logviewdockwidget->addText(QString::number(matWorldCoord.rows) + " " + QString::number(matWorldCoord.cols) + QString("\n"));

			TriangleMesh trimesh = ut->triangulate(matWorldCoord,5,0.01);
		
			
			Geometry3D * mygeom = new Geometry3D;
			mygeom->InitializeMesh();
			mygeom->m_mesh.request_face_normals();
			mygeom->m_mesh.request_face_colors();
			mygeom->m_mesh.request_vertex_normals();
			mygeom->m_mesh.request_vertex_colors();
			mygeom->m_mesh.request_vertex_texcoords2D();
			ut->from_TriangleMesh_to_OpenMesh(&trimesh, mygeom->m_mesh);
			for (int i = 0; i < trimesh.vmap.rows; i++)	{
				for (int j = 0; j < trimesh.vmap.cols; j++){
					if (trimesh.vmap.at<int>(i, j) != -1){
						OpenMesh::VertexHandle vh(trimesh.vmap.at<int>(i, j));
						GeoTriMesh::TexCoord2D vt;
						vt[1] = float(i) / float(trimesh.vmap.rows);
						vt[0]=float( j) / float(trimesh.vmap.cols);
						mygeom->m_mesh.set_texcoord2D(vh, vt);
					}
				}
			}
			

			mygeom->m_mesh.garbage_collection();
			mygeom->m_mesh.update_normals();

			mygeom->CalcAvgEdgeLen();
			mygeom->CalculateBoundingBox();
			mygeom->UpdateMesh();		// Display를 위해 노말을 꼭 업데이트 할 것
			
			mygeom->ModelID = count_loadedmodel;

			map_matWorldCoord[count_loadedmodel]=matWorldCoord;

			ext_current_model_index = mygeom->ModelID;
			QString filename = fileList.at(i);
			mygeom->ModelName = filename.replace(QString(".xml"), QString(".obj"));
			
			ModelIdVmap mv;
			mv.ModelId = mygeom->ModelID;
			mv.vmap = trimesh.vmap;
			map_filename_ModelIdVmap[texture_image] = mv;


			GLOptionWidget *GLOptionwidget = (GLOptionWidget *)(mainwindow->GLOptiondock->widget());
			GLOptionwidget->CreateModelListItem(mygeom->ModelID, mygeom->ModelName);

			mainwindow->m_TriMeshs.push_back(mygeom);
			mainwindow->current_mesh = mygeom;
			
			mainwindow->myGlWidget->makeCurrent();
			mainwindow->myGlWidget->GetModel(&mainwindow->m_TriMeshs);


			QString qs = texture_image.c_str();
			QString texpath = QString("%1/%2").arg(dir.absolutePath()).arg(qs);
			cv::Mat image = cv::imread(texpath.toStdString());
			//cv::imshow("a", image);
			int size = image.total() * image.elemSize();
			mygeom->m_texture = new byte[size];  // you will have to delete[] that later
			mygeom->m_texture_height = image.cols;
			mygeom->m_texture_width = image.rows;
			std::memcpy(mygeom->m_texture, image.data, size * sizeof(byte));
			mainwindow->myGlWidget->GLSetTexture(image.cols, image.rows, mygeom->m_texture);
			image.release();

			mainwindow->myGlWidget->GLOnViewBottom();
			mainwindow->myGlWidget->updateGL();
			mainwindow->myGlWidget->doneCurrent();
					
			mygeom->convert_Geo3DMesh_DeformableMesh3d();

			count_loadedmodel++;
		}
		fs.release();	
	}
	
	
	
	//ut->open_xml_dir(dirpath.toStdString().c_str());
}

void DeformationWidget::gen_graph()
{
	for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
	//if(deforming_ModelID != -1)
	{
		if (!mainwindow->m_TriMeshs.at(i)->graphGened()) {
			int ret = QMessageBox::warning(this, tr("show the graph"),
				tr("The graph has not been generated\nWould you like to generate it now?"),
				QMessageBox::Ok | QMessageBox::Cancel);
			if (ret) {


				mainwindow->m_TriMeshs.at(i)->gen_graph_query();
			}
			else {
				return;
			}
		}
	}

}


void DeformationWidget::mouse_pick()
{
	if (ui.deformSelButton->isChecked())
		mainwindow->myGlWidget->m_nSelectionMode.bDeformSelection = true;
	else
		mainwindow->myGlWidget->m_nSelectionMode.bDeformSelection = false;
		
	if (mainwindow->myGlWidget->m_nSelectionMode.bDeformSelection)
		mainwindow->myGlWidget->m_nManiMode = GLWidget::None;
	else
		mainwindow->myGlWidget->m_nManiMode = GLWidget::Trackball;

	mainwindow->myGlWidget->updateGL();

	//glView->setMouseMode(DeformationWidget::MOUSE_PICK);
}

void DeformationWidget::mouse_deform()
{
	mainwindow->myGlWidget->m_nSelectionMode.bDeformMove = !mainwindow->myGlWidget->m_nSelectionMode.bDeformMove;

	if (mainwindow->myGlWidget->m_nSelectionMode.bDeformMove)
	{
		mainwindow->myGlWidget->m_nManiMode = GLWidget::None;
		ui.deformSelButton->setChecked(false);		
		ui.deformSelButton->clicked(false);
		ui.deformSelButton->setEnabled(false);

		for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
			mainwindow->m_TriMeshs.at(i)->getHandles();
	}
	else
	{
		mainwindow->myGlWidget->m_nManiMode = GLWidget::Trackball;
		ui.deformSelButton->setEnabled(true);
	}



	mainwindow->myGlWidget->updateGL();
}


void DeformationWidget::slot_correspondence()
{
	QString src_filePath = QFileDialog::getOpenFileName(this, tr("Open source image file"),
		tr("../models/"),
		tr("JPEG Files (*.jpg);;"
		"All Files (*.*)"));

	QFileInfo fi(src_filePath);
	QString src_filename = fi.fileName();

	if (src_filename.isEmpty())
	{
		QMessageBox::information(this, "No Image", "No Src Image");
		return;
	}

	QString dst_filePath = QFileDialog::getOpenFileName(this, tr("Open target image file"),
		tr("../models/"),
		tr("JPEG Files (*.jpg);;"
		"All Files (*.*)"));
	fi=QFileInfo(dst_filePath);
	QString dst_filename = fi.fileName();

	if (dst_filename.isEmpty())
	{
		QMessageBox::information(this, "No Image", "No Dst Image");
		return;
	}

	ModelIdVmap src_vmap = map_filename_ModelIdVmap[src_filename.toStdString()];
	ModelIdVmap dst_vmap = map_filename_ModelIdVmap[dst_filename.toStdString()];

	cv::Mat vmap = src_vmap.vmap;
	cv::Mat vmap1 = dst_vmap.vmap;

	//int pi = vmap.at< int >(795, vmap.rows-352);
	//int qi= vmap.at<int>(830, vmap.rows-341);
	CorrespondenceOpenCV *clickcorress = new CorrespondenceOpenCV(&vmap,&vmap1);

	std::vector < CORRESPONDENCE > corr = clickcorress->ExtractPairs(src_filePath.toStdString(), dst_filePath.toStdString());
	cv::Mat texture = cv::imread(src_filePath.toStdString());
	int size = (int)texture.total()*texture.elemSize();
	mainwindow->m_TriMeshs[src_vmap.ModelId]->m_texture = new BYTE[size];
	std::memcpy(mainwindow->m_TriMeshs[src_vmap.ModelId]->m_texture, texture.data, size*sizeof(BYTE));
	
	texture.release();
	texture = cv::imread(dst_filePath.toStdString());
	size = (int)texture.total()*texture.elemSize();

	mainwindow->m_TriMeshs[dst_vmap.ModelId]->m_texture = new BYTE[size];
	std::memcpy(mainwindow->m_TriMeshs[dst_vmap.ModelId]->m_texture, texture.data, size*sizeof(BYTE));

	std::vector<cv::Point2i> vmap_list;
	for (int i = 0; i < map_matWorldCoord[src_vmap.ModelId].rows; i++)
		for (int j = 0; j < map_matWorldCoord[src_vmap.ModelId].cols; j++)
		if (vmap.at<int>(i, j) != -1)
			vmap_list.push_back(cv::Point2i(j, i));
/*

	cv::Mat1f train1(mainwindow->m_TriMeshs.at(src_vmap.ModelId)->m_mesh.n_vertices(), 3, 0.0f);
	for (int rr = 0; rr < mainwindow->m_TriMeshs.at(src_vmap.ModelId)->m_mesh.n_vertices(); ++rr)
	for (int cc = 0; cc < 3; ++cc)
		train1[rr][cc] = mainwindow->m_TriMeshs.at(src_vmap.ModelId)->m_mesh.point(GeoTriMesh::VertexHandle(rr))[cc];
	cv::flann::Index index1(train1, cv::flann::KDTreeIndexParams(1));
*/

		cv::Mat1f train1(vmap_list.size(), 2, 0.0);
		for (int rr = 0; rr < vmap_list.size(); ++rr)
		{
			train1[rr][0] = vmap_list[rr].x;
			train1[rr][1] = vmap_list[rr].y;
		}
		cv::flann::Index index1(train1, cv::flann::KDTreeIndexParams(1));
			
		vmap_list.clear();
		for (int i = 0; i < map_matWorldCoord[dst_vmap.ModelId].rows; i++)
		for (int j = 0; j < map_matWorldCoord[dst_vmap.ModelId].cols; j++)
		if (vmap1.at<int>(i, j) != -1)
			vmap_list.push_back(cv::Point2i(j, i));

/*
	cv::Mat1f train2(mainwindow->m_TriMeshs.at(dst_vmap.ModelId)->m_mesh.n_vertices(), 3, 0.0f);
	for (int rr = 0; rr < mainwindow->m_TriMeshs.at(dst_vmap.ModelId)->m_mesh.n_vertices(); ++rr)
	    for (int cc = 0; cc < 3; ++cc)
			train2[rr][cc] = mainwindow->m_TriMeshs.at(dst_vmap.ModelId)->m_mesh.point(GeoTriMesh::VertexHandle(rr))[cc];
	cv::flann::Index index2(train2, cv::flann::KDTreeIndexParams(1));
	*/
		cv::Mat1f train2(vmap_list.size(), 2, 0.0);
		for (int rr = 0; rr < vmap_list.size(); ++rr)
		{
			train2[rr][0] = vmap_list[rr].x;
			train2[rr][1] = vmap_list[rr].y;
		}
		cv::flann::Index index2(train2, cv::flann::KDTreeIndexParams(1));
   
		cv::Mat1f query(1, 2, 0.0);
	cv::Mat1i indices(1, 1, 0);
	cv::Mat1f distances(1, 1, 0.f);

	mainwindow->m_TriMeshs[dst_vmap.ModelId]->pdmesh_->getSVSet().clear();
	for (int i = 0; i < corr.size(); i++)
	{
		//cv::Vec3f p3d = map_matWorldCoord[src_vmap.ModelId].at<cv::Vec3f>(corr[i].p_2dpoint[1], corr[i].p_2dpoint[0]);
		//query[0][0] = p3d[0];  query[0][1] = p3d[1];  query[0][2] = p3d[2];
		query[0][0] = corr[i].p_2dpoint[0]; query[0][1] = corr[i].p_2dpoint[1];
		index1.knnSearch(query, indices, distances, 1);
		OpenMesh::VertexHandle vh1(vmap.at<int>((int)train1[indices[0][0]][1], (int)train1[indices[0][0]][0]));

		//OpenMesh::VertexHandle vh1(corr[i].p);
		//mainwindow->m_TriMeshs[a.ModelId]->pdmesh_->getSVSet().insert(vh1);
	//	cv::Vec3f q3d=map_matWorldCoord[dst_vmap.ModelId].at<cv::Vec3f>(corr[i].q_2dpoint[1], corr[i].q_2dpoint[0]);
	//	query[0][0] = q3d[0];  query[0][1] = q3d[1];  query[0][2] = q3d[2];
		
		query[0][0] = corr[i].q_2dpoint[0]; query[0][1] = corr[i].q_2dpoint[1];

        index2.knnSearch(query, indices, distances, 1);
		OpenMesh::VertexHandle vh2(vmap1.at<int>((int)train2[indices[0][0]][1], (int)train2[indices[0][0]][0]));
        //OpenMesh::VertexHandle vh2(corr[i].q);
		if (!mainwindow->m_TriMeshs[src_vmap.ModelId]->m_mesh.is_valid_handle(vh1) || !mainwindow->m_TriMeshs[dst_vmap.ModelId]->m_mesh.is_valid_handle(vh2))
			continue;
		GeoTriMesh::Point ap = mainwindow->m_TriMeshs[src_vmap.ModelId]->m_mesh.point(vh1);
		GeoTriMesh::Point aq = mainwindow->m_TriMeshs[dst_vmap.ModelId]->m_mesh.point(vh2);
		mainwindow->m_TriMeshs[dst_vmap.ModelId]->pdmesh_->getSVSet().insert(vh2);
		pairs.push_back(std::make_pair(vh1, vh2));
	}
 	src_ModelId = src_vmap.ModelId;
 	deforming_ModelID = dst_vmap.ModelId;


/*	
	std::ostringstream str;
	str << src_filePath.toStdString() << "_" << dst_filePath.toStdString();
	map_vindex_corres[str.str()] = corr;
*/

/*	glView->m_bCorrespondence = !glView->m_bCorrespondence;


	if (ext_current_model_index >=1)
	{
		//		mainwindow->m_TriMeshs.at(ext_current_model_index - 1).selectedHandleIDs.clear();
		//		mainwindow->m_TriMeshs.at(ext_current_model_index).selectedHandleIDs.clear();

		int ctrpts_size = mainwindow->m_TriMeshs.at(ext_current_model_index - 1).selectedHandleIDs.size();
		for (int i = 0; i < ctrpts_size; i++)
		{
			OpenMesh::VertexHandle vh1 = mainwindow->m_TriMeshs.at(ext_current_model_index - 1).selectedHandleIDs[i];
			OpenMesh::VertexHandle vh2 = mainwindow->m_TriMeshs.at(ext_current_model_index).selectedHandleIDs[i];
			pairs.push_back(std::make_pair(vh1, vh2));
		}
	}*/



}

void DeformationWidget::slot_dodeform()
{
	//mainwindow->m_TriMeshs.at(ext_current_model_index - 1)->getHandles(); //source handles
	//mainwindow->m_TriMeshs.at(ext_current_model_index)->getHandles(); //target handles

	//for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
		mainwindow->m_TriMeshs.at(deforming_ModelID)->getHandles();

	//mainwindow->m_TriMeshs.at(ext_current_model_index).dodeform();
	
	std::vector<GeoTriMesh::VertexHandle> handleIDs;
	
	std::vector<OpenMesh::VertexHandle> trg_ctrpts;
	typedef meshtalent::math::Vector3d<double> V3d; 
	std::vector<V3d> vec_trans_ctrpts;

	
	for (int i = 0; i < pairs.size(); i++)
	{
		GeoTriMesh::VertexHandle vh1 = pairs[i].first; //src_vertex
		GeoTriMesh::VertexHandle vh2 = pairs[i].second;//dst_vertex

		if (std::find(mainwindow->m_TriMeshs.at(deforming_ModelID)->pdmesh_->getSVSet().begin(), mainwindow->m_TriMeshs.at(deforming_ModelID)->pdmesh_->getSVSet().end(), vh2) != mainwindow->m_TriMeshs.at(deforming_ModelID)->pdmesh_->getSVSet().end())
		{
			mainwindow->m_TriMeshs.at(deforming_ModelID)->selectedHandleIDs.clear();
			mainwindow->m_TriMeshs.at(deforming_ModelID)->selectedHandleIDs.push_back(vh2);
		}
	
	//	trg_ctrpts.push_back(vh2);

		logviewdockwidget->addText(QString("1dx : %1 2dx: %2\n").arg(vh1.idx()).arg(vh2.idx()));

		
		GeoTriMesh::Point p1 = mainwindow->m_TriMeshs.at(src_ModelId)->m_mesh.point(vh1);
		GeoTriMesh::Point p2 = mainwindow->m_TriMeshs.at(deforming_ModelID)->m_mesh.point(vh2);

		V3d v(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
		//V3d v(0.0, 0.0, -0.05);
		vec_trans_ctrpts.push_back(v);
		
		logviewdockwidget->addText(QString("v : %1 %2 %3\n").arg(v[0]).arg(v[1]).arg(v[2]));

		mainwindow->m_TriMeshs.at(deforming_ModelID)->pdmesh_->translate(mainwindow->m_TriMeshs.at(deforming_ModelID)->selectedHandleIDs, vec_trans_ctrpts); //source  모델의 control points를 translate
		//trg_ctrpts.clear();
		vec_trans_ctrpts.clear();
	}
	
	pairs.clear();
	mainwindow->m_TriMeshs[deforming_ModelID]->pdmesh_->getSVSet().clear();
	mainwindow->myGlWidget->updateGL();

}

void DeformationWidget::ONOFF_DrawGraph()
{
	mainwindow->myGlWidget->bShowGraph = !mainwindow->myGlWidget->bShowGraph;
	mainwindow->myGlWidget->updateGL();
}

void DeformationWidget::slot_SaveDeformedPointCloud()
{

	//	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	std::ofstream file;
	file.open("../models/deformed_points.ply");

	file << "ply" << std::endl;
	file << "format ascii 1.0" << std::endl;

	int n_element_vertex=0;
	for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
	{
		n_element_vertex += mainwindow->m_TriMeshs.at(i)->m_mesh.n_vertices();
	
	}

	file << "element vertex " << n_element_vertex << std::endl;
	file << "property float x" << std::endl;
	file << "property float y" << std::endl;
	file << "property float z" << std::endl;
	file << "property float nx" << std::endl;
	file << "property float ny" << std::endl;
	file << "property float nz" << std::endl;
	file << "element face 0" << std::endl;
	file << "property list uchar int vertex_indices" << std::endl;
	file << "end_header" << std::endl;


	vector<OpenMesh::Vec3f> point_set;
	


	for (int i = 0; i < mainwindow->m_TriMeshs.size(); i++)
	{
		mainwindow->m_TriMeshs.at(i)->m_mesh.update_normals();
		GeoTriMesh::VertexIter			v_it(mainwindow->m_TriMeshs.at(i)->m_mesh.vertices_begin());
		GeoTriMesh::VertexIter			v_end(mainwindow->m_TriMeshs.at(i)->m_mesh.vertices_end());

		for (v_it; v_it != v_end; ++v_it)
		{

			GeoTriMesh::Point p = mainwindow->m_TriMeshs.at(i)->m_mesh.point(v_it);
				GeoTriMesh::Normal n = mainwindow->m_TriMeshs.at(i)->m_mesh.normal(v_it);
				file << p[0] << " " << p[1] << " " << p[2] << " " << n[0] << " " << n[1] << " " << n[2] << std::endl;
				//point_set.push_back(p);

		}

/*	PBM::NormalEstimation *ne(new PBM::NormalEstimation(point_set));

		int *knn;		// k-nearest neighbor index;
		int nPts = mainwindow->m_TriMeshs.at(i)->m_mesh.n_vertices();
		int nMax = 8;
		knn = new int[nMax*nPts];
		

		for (int i = 0; i < nPts; ++i){
			// neighborhood computation
			//nn[i] = ne->k_nearest_neighbor(i, nMax, knn + i*nMax);
			ne->k_nearest_neighbor(i, nMax, knn + i*nMax);
		}

		ne->compute_normal(8);
		
		OpenMesh::Vec3f n;
	int j= 0;
	v_it=mainwindow->m_TriMeshs.at(i)->m_mesh.vertices_begin();
	for (v_it, j; v_it != v_end; ++v_it, j++)
		{

			GeoTriMesh::Point p = mainwindow->m_TriMeshs.at(i)->m_mesh.point(v_it);
			n[0] = ne->normalVector(j)[0];
			n[1] = ne->normalVector(j)[1];
			n[2] = ne->normalVector(j)[2];

			file << p[0] << " " << p[1] << " " << p[2] << " " << n[0] << " " << n[1] << " " << n[2] << std::endl;
 		}

		delete ne;
		delete []  knn;
	*/}

	file.close();

	const char* inputplypointfile = "../models/deformed_points.ply";
	const char* outputplymeshfile = "../models/deformed_mesh.ply";

	//MeshRecon::PoissonRecon(inputplypointfile, outputplymeshfile);

	mainwindow->loadFile(QString(outputplymeshfile));

}




#endif

