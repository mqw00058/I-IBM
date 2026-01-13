#ifndef GLOPTIONWIDET_CPP
#define GLOPTIONWIDET_CPP

#pragma once

#include "GLOptionWidget.h"

GLOptionWidgetT::GLOptionWidgetT(GLWidget* Ptr) : visionView(Ptr)
{
	QGroupBox *ModelViewOptionGroup = new QGroupBox(tr("Model View Options"),visionView);

		QVBoxLayout *ModelViewSelectionLayout = new QVBoxLayout(ModelViewOptionGroup);
			QWidget *ModelViewVerticalLayoutWidget1 = new QWidget(ModelViewOptionGroup);
			CreateModelViewOptions(ModelViewVerticalLayoutWidget1);
			QWidget *ModelViewVerticalLayoutWidget2 = new QWidget(ModelViewOptionGroup);
			CreateModelSelectionOptions(ModelViewVerticalLayoutWidget2);				
		
		ModelViewSelectionLayout->addWidget(ModelViewVerticalLayoutWidget1);
		ModelViewSelectionLayout->addWidget(ModelViewVerticalLayoutWidget2);

	ModelViewOptionGroup->setLayout(ModelViewSelectionLayout);
	

	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	QGroupBox *RenderSceneOptionGroup = new QGroupBox(tr("Render Scene Options"));
		QVBoxLayout *RenderSceneOptionLayout = new QVBoxLayout(RenderSceneOptionGroup);
				CreateRenderSceneOptions(RenderSceneOptionLayout);
		RenderSceneOptionGroup->setLayout(RenderSceneOptionLayout);


			//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á

QVBoxLayout *mainLayout = new QVBoxLayout;
mainLayout->addWidget(ModelViewOptionGroup);
mainLayout->addWidget(RenderSceneOptionGroup);
mainLayout->addSpacing(12);
mainLayout->addStretch(1);

setLayout(mainLayout);
}


GLOptionWidgetT::~GLOptionWidgetT()
{
}




//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
void GLOptionWidgetT::ONOFF_DrawNormal(bool onff)
{
	visionView->m_bVertexNormal = onff;
    visionView->update();
}

void GLOptionWidgetT::ONOFF_DrawEdge(bool onff)
{
	visionView->m_bEdge = onff;
    visionView->update();
}

void GLOptionWidgetT::ONOFF_DrawVertex(bool onff)
{
	visionView->m_bVertex = onff;
    visionView->update();
}


void GLOptionWidgetT::ONOFF_DrawFace(bool onff)
{
	visionView->m_bFace = onff;
    visionView->update();
}


void GLOptionWidgetT::ONOFF_Centering(bool onff)
{
	visionView->m_bCenter = onff;
	visionView->GLOnViewCenter();
    visionView->update();
}

void GLOptionWidgetT::ONOFF_SelectPointsMode(bool onff)
{
	if (onff)
		visionView->m_nSelectionMode.bVerSelection=true;
	else
		visionView->m_nSelectionMode.bVerSelection=false;
    visionView->update();
}

void GLOptionWidgetT::ONOFF_SelectFacesMode(bool onff)
{
	if (onff)
		visionView->m_nSelectionMode.bTriSelection=true;
	else
		visionView->m_nSelectionMode.bTriSelection=false;

    visionView->update();
}


void GLOptionWidgetT::ONOFF_DrawBoundingBox(bool onff)
{
	if (onff)
		visionView->m_bBoundingBox=true;
	else
		visionView->m_bBoundingBox=false;
    visionView->update();
}

void GLOptionWidgetT::ONOFF_DrawWorkspace(bool onff)
{
	visionView->m_bDrawWorkspace = onff;
    visionView->update();
}

void GLOptionWidgetT::ONOFF_DrawAxis(bool onff)
{
	visionView->m_bDrawAxis = onff;
    visionView->update();
}
//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á




void GLOptionWidgetT::CreateModelViewOptions(QWidget  * ModelViewVerticalLayoutWidget1)
{

	QVBoxLayout *ModelViewOptionLayout = new QVBoxLayout(ModelViewVerticalLayoutWidget1);

	QCheckBox *bVertexNormalCheckBox = new QCheckBox(tr("Show Vertex Normal"),ModelViewVerticalLayoutWidget1);
	QCheckBox *bVertexCheckBox = new QCheckBox(tr("Show Vertex"),ModelViewVerticalLayoutWidget1);
	QCheckBox *bEdgeCheckBox = new QCheckBox(tr("Show Edge"),ModelViewVerticalLayoutWidget1);
	QCheckBox *bFaceCheckBox = new QCheckBox(tr("Show Face"),ModelViewVerticalLayoutWidget1);
	QCheckBox *bCenterCheckBox = new QCheckBox(tr("Centering"),ModelViewVerticalLayoutWidget1);
	bFaceCheckBox->setChecked(true);//bFaceCheckBox->toggle();

	ModelViewOptionLayout->addWidget(bVertexNormalCheckBox);
	ModelViewOptionLayout->addWidget(bVertexCheckBox);
	ModelViewOptionLayout->addWidget(bEdgeCheckBox);
	ModelViewOptionLayout->addWidget(bFaceCheckBox);
	ModelViewOptionLayout->addWidget(bCenterCheckBox);

	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	connect(bVertexNormalCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawNormal(bool)));
	connect(bEdgeCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawEdge(bool)));
	connect(bVertexCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawVertex(bool)));
	connect(bFaceCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawFace(bool)));
	connect(bCenterCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_Centering(bool)));	
}

void GLOptionWidgetT::CreateModelSelectionOptions(QWidget  * ModelViewVerticalLayoutWidget2)
{

QIcon icon[3];
icon[0].addFile(QString::fromUtf8("./images/select_points.png"), QSize(), QIcon::Normal, QIcon::Off);	 
QPushButton *buttonSelectPoints = new QPushButton(ModelViewVerticalLayoutWidget2);
buttonSelectPoints->setMaximumSize(40,40);
buttonSelectPoints->setIcon(icon[0]);
buttonSelectPoints->setIconSize(QSize(32, 32));
buttonSelectPoints->setCheckable(true);	

icon[1].addFile(QString::fromUtf8("./images/select_edges.png"), QSize(), QIcon::Normal, QIcon::Off);
QPushButton *buttonSelectEdgess = new QPushButton(ModelViewVerticalLayoutWidget2);
buttonSelectEdgess->setMaximumSize(40,40);
buttonSelectEdgess->setIcon(icon[1]);
buttonSelectEdgess->setIconSize(QSize(32, 32));
buttonSelectEdgess->setCheckable(true);	

icon[2].addFile(QString::fromUtf8("./images/select_faces.png"), QSize(), QIcon::Normal, QIcon::Off);
QPushButton *buttonSelectFaces = new QPushButton(ModelViewVerticalLayoutWidget2);
buttonSelectFaces->setMaximumSize(40,40);
buttonSelectFaces->setIcon(icon[2]);
buttonSelectFaces->setIconSize(QSize(32, 32));
buttonSelectFaces->setCheckable(true);	


QHBoxLayout *ModelSelectionOptionLayout = new QHBoxLayout(ModelViewVerticalLayoutWidget2);
ModelSelectionOptionLayout->addWidget(buttonSelectPoints);	 
ModelSelectionOptionLayout->addWidget(buttonSelectEdgess);	 
ModelSelectionOptionLayout->addWidget(buttonSelectFaces);	




connect(buttonSelectPoints, SIGNAL(toggled(bool)), this, SLOT(ONOFF_SelectPointsMode(bool)));
connect(buttonSelectFaces, SIGNAL(toggled(bool)), this, SLOT(ONOFF_SelectFacesMode(bool)));
}

void GLOptionWidgetT::CreateRenderSceneOptions(QLayout  * RenderSceneOptionLayout)
{
	QCheckBox *bBoundingBoxCheckBox = new QCheckBox(tr("Show Bounding Box"));
	QCheckBox *bWorkspaceCheckBox = new QCheckBox(tr("Show Workspace"));
	bWorkspaceCheckBox->setChecked(true);
	QCheckBox *bAxisCheckBox = new QCheckBox(tr("Show Axis"));
	bAxisCheckBox->setChecked(true);

	RenderSceneOptionLayout->addWidget(bBoundingBoxCheckBox);
	RenderSceneOptionLayout->addWidget(bWorkspaceCheckBox);
	RenderSceneOptionLayout->addWidget(bAxisCheckBox);

	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á

	connect(bBoundingBoxCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawBoundingBox(bool)));
	connect(bWorkspaceCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawWorkspace(bool)));
	connect(bAxisCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawAxis(bool)));
}
#endif 
