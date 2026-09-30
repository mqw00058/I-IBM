#ifndef DEFORMATIONWIDGET_H
#define DEFORMATIONWIDGET_H

#include <QWidget>
#include "mainwindow.h"
#include "ui_DeformationWidget.h"
#include "src/Geometry3D/Geometry3D.h"
#include "src/EmbeddedDeform/DeformableMesh3d.h"
#include "src/EmbeddedDeform/DeformationGraph.h"




class DeformationWidget : public QWidget
{
	Q_OBJECT

public:	
//	enum { MOUSE_PICK, MOUSE_DEFORM };

	DeformationWidget(MainWindow *parent);
	~DeformationWidget();
	//meshtalent::DeformableMesh3d* pdmesh_;
	//meshtalent::DeformationGraph* pdgraph_;
	
protected:
	Ui::DeformationWidget ui;


	struct ModelIdVmap
	{
		int ModelId;
		cv::Mat vmap;
	};

	MainWindow* mainwindow;

	/*MainWindow* getMainWindow()
	{
		MainWindow* mainwindow = qobject_cast<MainWindow*>(parentWidget());
		return mainwindow;		
	}*/


	//void select_currentModel();

	std::map<std::string, ModelIdVmap> map_filename_ModelIdVmap;

	typedef std::vector<std::pair<OpenMesh::VertexHandle, OpenMesh::VertexHandle>> MYPAIR;
	MYPAIR pairs;

	std::map<int, cv::Mat> map_matWorldCoord;
	int deforming_ModelID, src_ModelId;

public slots:
	void gen_graph();
	void mouse_pick();
	void mouse_deform();
	void slot_correspondence();
	void slot_dodeform();
	void ONOFF_DrawGraph();
	void slot_SaveDeformedPointCloud();
	void slot_OpenXMLDir();

};



#endif // DEFORMATIONWIDGET_H
