#ifndef GLOPTIONWIDET_CPP
#define GLOPTIONWIDET_CPP

#include "GLOptionWidget.h"


GLOptionWidget::GLOptionWidget(MainWindow* parent) 
{
	mainwindow = (MainWindow*)parent;
	//MainWindow *mainwindow = qobject_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget());
	glView = mainwindow->myGlWidget;
	//glView = qobject_cast<GLWidget*>(this->parentWidget());
	//QTabWidget *test = qobject_cast<QTabWidget*>(this->parentWidget()->parentWidget());

	QGroupBox *ModelListViewGroup = new QGroupBox(tr("Model List"));
	QVBoxLayout *ModelListViewLayout = new QVBoxLayout(ModelListViewGroup);
	ModelListViewLayout->setSpacing(0);
	ModelListViewLayout->setContentsMargins(0, 0, 0, 0);

	QWidget *ModelListViewWidget = new QWidget(ModelListViewGroup);
	CreateModelListView(ModelListViewWidget);
	ModelListViewLayout->addWidget(ModelListViewWidget);
	ModelListViewGroup->setLayout(ModelListViewLayout);
	

	QGroupBox *ModelViewOptionGroup = new QGroupBox(tr("Model View Options"), glView);

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
	mainLayout->addWidget(ModelListViewGroup);
	mainLayout->addWidget(ModelViewOptionGroup);
	mainLayout->addWidget(RenderSceneOptionGroup);
	mainLayout->addSpacing(12);
	mainLayout->addStretch(1);

	setLayout(mainLayout);
}

GLOptionWidget::~GLOptionWidget()
{
}


//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á

void GLOptionWidget::ONOFF_SmoothFlatMode(bool onoff)
{
	glView->m_bSmooth = !glView->m_bSmooth;

	glView->updateGL();

}

void GLOptionWidget::OnOFF_TextureMode(bool onoff)
{
	glView->m_bTexture = !glView->m_bTexture;

	glView->updateGL();
}

void GLOptionWidget::ONOFF_Centering(bool onff)
{
	glView->m_bCenter = onff;
	glView->GLOnViewCenter();
	glView->updateGL();
}

void GLOptionWidget::ONOFF_SelectPointsMode(bool onff)
{
	if (onff)
		glView->m_nSelectionMode.bVerSelection = true;
	else
		glView->m_nSelectionMode.bVerSelection = false;
	glView->updateGL();
}

void GLOptionWidget::ONOFF_SelectFacesMode(bool onff)
{
	if (onff)
		glView->m_nSelectionMode.bTriSelection = true;
	else
		glView->m_nSelectionMode.bTriSelection = false;

	glView->updateGL();
}

void GLOptionWidget::RemoveSelectedElements()
{
	if (mainwindow->current_mesh->ModelID >= 0)
	{
 		std::vector<int> vlist = mainwindow->current_mesh->m_verPickList;
		mainwindow->current_mesh->DeleteVertices(vlist);
	}
	glView->updateGL();
}

void GLOptionWidget::ONOFF_DrawWorkspace(bool onff)
{
	glView->m_bDrawWorkspace = onff;
	glView->updateGL();
}


void GLOptionWidget::ONOFF_DrawAxis(bool onff)
{
	glView->m_bDrawAxis = onff;
	glView->updateGL();
}

//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á


void GLOptionWidget::ONOFF_DrawBoundingBox(int modelnumber)
{
	glView->m_bDrawingOptions[modelnumber].m_bBoundingBox = !glView->m_bDrawingOptions[modelnumber].m_bBoundingBox;
	glView->updateGL();
}


void GLOptionWidget::ONOFF_DrawVertex(int modelnumber)
{
	glView->m_bDrawingOptions[modelnumber].m_bVertex = !glView->m_bDrawingOptions[modelnumber].m_bVertex;
	glView->updateGL();
}

void GLOptionWidget::ONOFF_DrawEdge(int modelnumber)
{
	glView->m_bDrawingOptions[modelnumber].m_bEdge = !glView->m_bDrawingOptions[modelnumber].m_bEdge;
	glView->updateGL();
}

void GLOptionWidget::ONOFF_DrawFace(int modelnumber)
{
	glView->m_bDrawingOptions[modelnumber].m_bFace = !glView->m_bDrawingOptions[modelnumber].m_bFace;
	glView->updateGL();
}

void GLOptionWidget::ONOFF_DrawNormal(int modelnumber)
{
	glView->m_bDrawingOptions[modelnumber].m_bVertexNormal = !glView->m_bDrawingOptions[modelnumber].m_bVertexNormal;
	glView->updateGL();
}


//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á


void GLOptionWidget::select_currentModel()
{

	ext_current_model_index = listWidget->currentRow();
	if(mainwindow->m_TriMeshs.size() > ext_current_model_index)
		mainwindow->current_mesh = mainwindow->m_TriMeshs[ext_current_model_index];
}


void GLOptionWidget::CreateModelListView(QWidget  * ModelListViewWidget)
{
	QVBoxLayout *ModelListViewLayout = new QVBoxLayout(ModelListViewWidget);

	listWidget = new QListWidget(ModelListViewWidget);
	//listWidget->setMaximumWidth(200);
	listWidget->setMaximumWidth(QWidget::maximumWidth());

	ModelListViewLayout->addWidget(listWidget);


	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á

	QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(select_currentModel()));
}


void GLOptionWidget::CreateModelListItem(int modelnumber, QString modelname)
{
	QString str = QString("%1 : %2").arg(modelnumber).arg(modelname);
	QLabel *label = new QLabel(str);


	QToolBar* rendertb = new QToolBar();
	QAction* action1 = new QAction(QIcon("./images/bbox.png"), "BoundingBox", rendertb);
	QAction* action2 = new QAction(QIcon("./images/vertex.png"), "Vertex", rendertb);
	QAction* action3 = new QAction(QIcon("./images/edge.png"), "Edge", rendertb);
	QAction* action4 = new QAction(QIcon("./images/face.png"), "Face", rendertb);
	QAction* action5 = new QAction(QIcon("./images/normal.png"), "Normal", rendertb);

	action1->setCheckable(true);
	action2->setCheckable(true);
	action3->setCheckable(true);
	action4->setCheckable(true);
	action5->setCheckable(true);

	action4->setChecked(true);

	rendertb->addAction(action1);
	rendertb->addAction(action2);
	rendertb->addAction(action3);
	rendertb->addAction(action4);
	rendertb->addAction(action5);

	//QIcon icon = QIcon::fromTheme("edit-undo");
	QListWidgetItem *item = new QListWidgetItem();
	//item->setIcon(icon);
	//item->setSizeHint(QSize(200, 200));
	item->setBackgroundColor(QColor(100, 255, 200));
	listWidget->addItem(item);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);


	//item = <label | rendertb>, rendertb = <action1 | action2 | action3>
	layout->addWidget(label);
	layout->addWidget(rendertb);

	QWidget *widget = new QWidget();
	widget->setLayout(layout);
	item->setSizeHint(widget->sizeHint());

	listWidget->setItemWidget(item, widget);

	listWidget->setCurrentItem(item);
	//GLOptionwidget->listWidget->setCurrentRow(modelnumber);
	QSignalMapper *signalmapper1 = new QSignalMapper(this);
	QSignalMapper *signalmapper2 = new QSignalMapper(this);
	QSignalMapper *signalmapper3 = new QSignalMapper(this);
	QSignalMapper *signalmapper4 = new QSignalMapper(this);
	QSignalMapper *signalmapper5 = new QSignalMapper(this);

	connect(action1, SIGNAL(triggered()), signalmapper1, SLOT(map()));
	connect(action2, SIGNAL(triggered()), signalmapper2, SLOT(map()));
	connect(action3, SIGNAL(triggered()), signalmapper3, SLOT(map()));
	connect(action4, SIGNAL(triggered()), signalmapper4, SLOT(map()));
	connect(action5, SIGNAL(triggered()), signalmapper5, SLOT(map()));

	signalmapper1->setMapping(action1, modelnumber);
	QObject::connect(signalmapper1, SIGNAL(mapped(int)), this, SLOT(ONOFF_DrawBoundingBox(int)));
	signalmapper2->setMapping(action2, modelnumber);
	QObject::connect(signalmapper2, SIGNAL(mapped(int)), this, SLOT(ONOFF_DrawVertex(int)));
	signalmapper3->setMapping(action3, modelnumber);
	QObject::connect(signalmapper3, SIGNAL(mapped(int)), this, SLOT(ONOFF_DrawEdge(int)));
	signalmapper4->setMapping(action4, modelnumber);
	QObject::connect(signalmapper4, SIGNAL(mapped(int)), this, SLOT(ONOFF_DrawFace(int)));
	signalmapper5->setMapping(action5, modelnumber);
	QObject::connect(signalmapper5, SIGNAL(mapped(int)), this, SLOT(ONOFF_DrawNormal(int)));


	listWidget->setMaximumWidth(QWidget::maximumWidth());
}

void GLOptionWidget::CreateModelViewOptions(QWidget  * ModelViewVerticalLayoutWidget1)
{

	QVBoxLayout *ModelViewOptionLayout = new QVBoxLayout(ModelViewVerticalLayoutWidget1);

	QHBoxLayout *ModelRenderOptionLayout = new QHBoxLayout();
	QPushButton *bSmoothFlatButton = new QPushButton();	
	QIcon *ico = new QIcon();
	ico->addPixmap(QPixmap("./images/smooth.png"), QIcon::Normal, QIcon::On);
	ico->addPixmap(QPixmap("./images/face.png"), QIcon::Normal, QIcon::Off);
	bSmoothFlatButton->setIcon(*ico);
	bSmoothFlatButton->setMaximumSize(40, 40);
	bSmoothFlatButton->setIconSize(QSize(32, 32));
	bSmoothFlatButton->setShortcut(QKeySequence(Qt::Key_Control + Qt::Key_F));
	bSmoothFlatButton->setCheckable(true);
	bSmoothFlatButton->setToolTip(tr("Flat & Smooth Render"));
	bSmoothFlatButton->setStatusTip(tr("Flat & Smooth Render"));

	QPushButton *bTextureButton = new QPushButton();
	QIcon *ico11 = new QIcon(); 
	ico11->addFile("./images/textures.png", QSize(32, 32));
	bTextureButton->setMaximumSize(40, 40);
	bTextureButton->setIconSize(QSize(32, 32));
	bTextureButton->setIcon(*ico11);
	bTextureButton->setCheckable(true);
	ModelRenderOptionLayout->addWidget(bSmoothFlatButton);
	ModelRenderOptionLayout->addWidget(bTextureButton);

	QCheckBox *bCenterCheckBox = new QCheckBox(tr("Centering"), ModelViewVerticalLayoutWidget1);
	
	ModelViewOptionLayout->addLayout(ModelRenderOptionLayout);
	ModelViewOptionLayout->addWidget(bCenterCheckBox);

	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	connect(bSmoothFlatButton, SIGNAL(toggled(bool)), this, SLOT(ONOFF_SmoothFlatMode(bool)));
	connect(bTextureButton, SIGNAL(toggled(bool)), this, SLOT(OnOFF_TextureMode(bool)));
	connect(bCenterCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_Centering(bool)));
}


void GLOptionWidget::CreateModelSelectionOptions(QWidget  * ModelViewVerticalLayoutWidget2)
{

	QIcon icon[3];
	icon[0].addFile(QString::fromUtf8("./images/select_vertex.png"), QSize(), QIcon::Normal, QIcon::Off);
	QPushButton *buttonSelectPoints = new QPushButton(ModelViewVerticalLayoutWidget2);
	buttonSelectPoints->setMaximumSize(40, 40);
	buttonSelectPoints->setIcon(icon[0]);
	buttonSelectPoints->setIconSize(QSize(32, 32));
	buttonSelectPoints->setCheckable(true);


	icon[1].addFile(QString::fromUtf8("./images/select_face.png"), QSize(), QIcon::Normal, QIcon::Off);
	QPushButton *buttonSelectFaces = new QPushButton(ModelViewVerticalLayoutWidget2);
	buttonSelectFaces->setMaximumSize(40, 40);
	buttonSelectFaces->setIcon(icon[1]);
	buttonSelectFaces->setIconSize(QSize(32, 32));
	buttonSelectFaces->setCheckable(true);


	icon[2].addFile(QString::fromUtf8("./images/delete_vert.png"), QSize(), QIcon::Normal, QIcon::Off);
	QPushButton *buttonSelectEdgess = new QPushButton(ModelViewVerticalLayoutWidget2);
	buttonSelectEdgess->setMaximumSize(40, 40);
	buttonSelectEdgess->setIcon(icon[2]);
	buttonSelectEdgess->setIconSize(QSize(32, 32));
	//buttonSelectEdgess->setCheckable(true);


	QHBoxLayout *ModelSelectionOptionLayout = new QHBoxLayout(ModelViewVerticalLayoutWidget2);
	ModelSelectionOptionLayout->addWidget(buttonSelectPoints);
	ModelSelectionOptionLayout->addWidget(buttonSelectEdgess);
	ModelSelectionOptionLayout->addWidget(buttonSelectFaces);


	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	connect(buttonSelectPoints, SIGNAL(toggled(bool)), this, SLOT(ONOFF_SelectPointsMode(bool)));
	connect(buttonSelectFaces, SIGNAL(toggled(bool)), this, SLOT(ONOFF_SelectFacesMode(bool)));
	connect(buttonSelectEdgess, SIGNAL(clicked()), this, SLOT(RemoveSelectedElements()));
}


void GLOptionWidget::CreateRenderSceneOptions(QLayout  * RenderSceneOptionLayout)
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

	connect(bBoundingBoxCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawBoundingBox(int)));
	connect(bWorkspaceCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawWorkspace(bool)));
	connect(bAxisCheckBox, SIGNAL(toggled(bool)), this, SLOT(ONOFF_DrawAxis(bool)));
}

void GLOptionWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_Delete:
		{

						   this->RemoveSelectedElements();

		break;
		}

	}
}
#endif 