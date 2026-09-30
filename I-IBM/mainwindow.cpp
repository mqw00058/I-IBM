/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <QtGui>


#include "src/GLWidget/GLOptionWidget.h"
#include "src/VisionWidget/VisionWidget.h"
#include "src/Geometry3D/Geometry3D.h"
#ifdef _WIN32
#include "KinectOptionWidget.h"
#endif
#include "DeformationWidget.h"
#include "mainwindow.h"
#include <opencv2/core/core.hpp>	// OpenCV header file
//#include <GL/freeglut.h>		// OpenGL header files

using namespace cv;
//! [0]


//! [1]
MainWindow::MainWindow()
//! [1] //! [2]
{	
	tabs = new QTabWidget(this);
	tabs->setParent(this);
	myGlWidget = new GLWidget(tabs);
	myGlWidget->setParent(tabs);
	//VisionWidget * myVisionWidget = new VisionWidget(tabs);
	
	tabs->insertTab(0, myGlWidget, "Graphic View");
	//tabs->insertTab(1, myVisionWidget, "Vision View");

	//tabs->setMovable(true);
	tabs->setTabsClosable(true);
	tabs->setTabShape(tabs->TabShape::Rounded);
	//this->resize(sizeHint())
	setCentralWidget(tabs);
	//this->tabifyDockWidget(dock1,dock2);

	//-------Log Dock View---------------------------------------
	logdock = new QDockWidget(tr("Log"), this);

	//MainWindow에서 DockWidget 변수전달
	logviewdockwidget = new LogviewDockWidget(logdock);
	logviewdockwidget->initialText();
	logviewdockwidget->createLogWidget();
	// logdock->setWidget(logviewdockwidget);
	addDockWidget(Qt::BottomDockWidgetArea, logdock);
	//logviewdockwidget->addText("adfadf");
	//---------------------------------------------------------


	//순서중요
	createActions();
	createToolBars();
	createStatusBar();
	createDockWidgets();
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(swichdockwidget(int)));
	createMenus();

	readSettings();

	// setCurrentFile("");
	setUnifiedTitleAndToolBarOnMac(true);

	current_mesh = NULL;
	
}
//! [2]
MainWindow::~MainWindow()
{
	if (current_mesh!=NULL)
		if (current_mesh->m_texture != NULL)
			delete[] current_mesh->m_texture;
}

void MainWindow::swichdockwidget(int index)
{
	/*QMessageBox* msg = new QMessageBox(this->parentWidget());
	msg->setWindowTitle("Hello !");
	msg->setText("Selected Tab index is : "+ QString::number(index));
	msg->show();
	*/

	if (index == 0)//tabs->indexOf(tabs->currentWidget())==1
		GLOptiondock->show();
	else
		GLOptiondock->hide();
}


//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
	if (maybeSave()) {
		setCurrentFile("");
	}
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
	if (maybeSave()) {
		QString selfilter = tr("OBJ Files (*.obj);;"
			"PLY Files (*.ply);;"
			"OFF Files (*.off);;"
			"STL Files (*.stl);;"
			"XML Files (*.xml);;");
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open mesh file"),
			tr("../models/"),
			tr("All Files (*.*);; OBJ Files (*.obj *.OBJ);;PLY Files (*.ply *.PLY);;OFF Files (*.off *.OFF);;STL Files (*.stl *.STL);;XML Files (*.xml)"), &selfilter);

		//---------Set current directory--------------------------------------------------------------
		QFileInfo  file(filePath);
		QDir::setCurrent(file.path());
		//--------------------------------------------------------------------------------------------

		QFileInfo  file_(filePath);
		QString extentsion = file_.suffix();
		
		if (!filePath.isEmpty())
			loadFile(filePath);
	}
}
//! [8]

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
	QString selectedFilter;
	QString filePath = QFileDialog::getSaveFileName(this,"Save",QDir::currentPath(),".obj",&selectedFilter);
	filePath = filePath + selectedFilter;
	if (filePath.isEmpty())
		return false;

	return saveFile(filePath);
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
	QMessageBox::about(this, tr("About Application"),
		tr("The <b>Application</b> example demonstrates how to "
		"write modern GUI applications using Qt, with a menu bar, "
		"toolbars, and a status bar."));
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
	//setWindowModified(textEdit->document()->isModified());
}
//! [16]

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
	newAct = new QAction(QIcon("./images/new.png"), tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	//! [19]
	openAct = new QAction(QIcon("./images/open.png"), tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	//! [18] //! [19]

	saveAct = new QAction(QIcon("./images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	//! [20]
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	//! [20]
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	//! [21]
	cutAct = new QAction(QIcon("./images/cut.png"), tr("Cu&t"), this);
	//! [21]
	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
		"clipboard"));
	//    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

	copyAct = new QAction(QIcon("./images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	// connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

	pasteAct = new QAction(QIcon("./images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	// connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	//! [22]
	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	//! [22]

	bilateralAct = new QAction(tr("&Bilateral filter"), this);
	connect(bilateralAct, SIGNAL(triggered()), this, SLOT(bilateralfilter()));


	//! [23]
	cutAct->setEnabled(false);
	//! [23] //! [24]
	copyAct->setEnabled(false);
	// connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
	// connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

	//! [25]
	implicitsurfacereconAction = new QAction(tr("&Hoppe"), this);
	sdfsurfacereconAction = new QAction(tr("&SDF"), this);

	connect(implicitsurfacereconAction, SIGNAL(triggered()), this, SLOT(menu_implicitsurfacereconHOPPE()));
	connect(sdfsurfacereconAction, SIGNAL(triggered()), this, SLOT(menu_implicitsurfacereconSDF()));

	kinectAction = new QAction(tr("&Kinect"), this);
	connect(kinectAction, SIGNAL(triggered()), this, SLOT(menu_Plugin_Kinect()));

	deformationAct = new QAction(tr("Embedded &Deformation"), this);
	connect(deformationAct, SIGNAL(triggered()), this, SLOT(embeddedDeformation()));
}
//! [24]

//! [25] //! [26]
void MainWindow::createMenus()
//! [25] //! [27]
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	//! [28]
	fileMenu->addAction(openAct);
	//! [28]
	fileMenu->addAction(saveAct);
	//! [26]
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);

	menuBar()->addSeparator();
	////////////////////////////////////////////////////////////////////
	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(logdock->toggleViewAction());
	///////////////////////////////////////////////////////////////////

	meshMenu = menuBar()->addMenu(tr("&Mesh"));
	meshMenu->addAction(deformationAct);

	filterMenu = menuBar()->addMenu(tr("&Filter"));
	filterMenu->addAction(bilateralAct);

	pointMenu = menuBar()->addMenu(tr("&Point"));
	pointMenu->addAction(implicitsurfacereconAction);
	pointMenu->addAction(sdfsurfacereconAction);

	////////////////////////////////////////////////////////////////////
	viewMenu = menuBar()->addMenu(tr("&Plug-in"));
	viewMenu->addAction(GLOptiondock->toggleViewAction());
#ifdef _WIN32
	viewMenu->addAction(kinectAction);
#endif
	///////////////////////////////////////////////////////////////////

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	//! [29] //! [31]
	fileToolBar->addAction(openAct);
	//! [31]
	fileToolBar->addAction(saveAct);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
}
//! [30]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
	statusBar()->showMessage(tr("Ready"));
}
//! [33]

void MainWindow::createDockWidgets()
{
	//------- GLOption Dock -------------------------------------
	GLOptiondock = new QDockWidget(tr("GLOption"), this);

	//MainWindow에서 DockWidget 변수전달
	
	GLOptionWidget * GLOptionwidget= new GLOptionWidget(this);
	GLOptionwidget->setParent(GLOptiondock);
		
	GLOptiondock->setWidget(GLOptionwidget);

	

	//  GLOptionwidget->setFeatures( QDockWidget::AllDockWidgetFeatures );

	GLOptiondock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

	// GLOptionwidget->createGLOptionWidget();

	addDockWidget(Qt::RightDockWidgetArea, GLOptiondock);

	int temp = tabs->currentIndex();
	if (temp != 0)
		GLOptiondock->hide();

	//QAction *docking=	  this->GLOptiondock->toggleViewAction();

	// docking->activate(QAction::Trigger);
	//---------------------------------------------------------



	//QAction *docking=	  this->GLOptiondock->toggleViewAction();

	// docking->activate(QAction::Trigger);
	//---------------------------------------------------------



	//------- GLOption Dock -------------------------------------
	//BundlerOptiondock =new QDockWidget(tr("BundlerOption"), this);

	//MainWindow에서 DockWidget 변수전달
	//BundlerOptionwidget= new BundlerOptionWidgetT<VisionWidget>(myVisionWidget);

	//BundlerOptiondock->setWidget(BundlerOptionwidget);

	//  GLOptionwidget->setFeatures( QDockWidget::AllDockWidgetFeatures );

	//BundlerOptiondock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

	// GLOptionwidget->createGLOptionWidget();

	//addDockWidget(Qt::RightDockWidgetArea, BundlerOptiondock);

	//temp=tabs->currentIndex();
	//if(temp!=1)
	//	GLOptiondock->hide();

	//QAction *docking=	  this->GLOptiondock->toggleViewAction();

	// docking->activate(QAction::Trigger);
	//---------------------------------------------------------
}

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
	QSettings settings("Trolltech", "Application Example");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(300, 300)).toSize();
	resize(size);
	move(pos);
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
	QSettings settings("Trolltech", "Application Example");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
	/* if (textEdit->document()->isModified()) {
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("Application"),
	tr("The document has been modified.\n"
	"Do you want to save your changes?"),
	QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	if (ret == QMessageBox::Save)
	return save();
	else if (ret == QMessageBox::Cancel)
	return false;
	}*/

	return true;
}
//! [41]

//! [42]

void MainWindow::loadFile(const QString &filePath)
//! [42] //! [43]
{
	QFile file(filePath);

	/*	if (!file.exists()) {
		QMessageBox::warning(this, "File Open", "NO File!");
		return;
	}

	QString errMsg;
	QFileDevice::FileError err = QFileDevice::NoError;
*/
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(filePath)
			.arg(file.errorString()));
		QMessageBox::critical(this, "File Open", "Error reading '" + filePath + "'.\n");
		return;
	}
	else
	{


#ifndef QT_NO_CURSOR
		QApplication::setOverrideCursor(Qt::WaitCursor);
#endif


		QFileInfo fileinfo(file.fileName());

		QString m_filePath(fileinfo.filePath());
		// QString m_filePath("fandisk.obj");
		QString extension(fileinfo.suffix());

		

		if (extension == "obj" || extension == "off" || extension == "ply")
		{
			//   m_TriMeshs.InitializeMesh();
			current_mesh = new Geometry3D;
			current_mesh->ReadMeshFile(m_filePath, extension);
			
			if (!current_mesh->m_texture_path.empty())
			{
				cv::Mat im = cv::imread(current_mesh->m_texture_path);
				//cv::imshow("test", im);
				current_mesh->m_texture_width = im.cols;
				current_mesh->m_texture_height = im.rows;
				size_t a = im.total();
				size_t b = im.elemSize();
				int c = im.channels();
				int d = im.depth();

				current_mesh->m_texture = new BYTE[a*b];
				memcpy(current_mesh->m_texture, im.data, a*b*sizeof(BYTE));				im.release();			}

			//mesh 정보 업데이트
			// GLUpdateMesh();
			//m_renderScene.GLUpdateFaceIndices(&this->m_TriMeshs);


			myVec::Vec3f my_bbMin, my_bbMax;

			my_bbMin.value[0] = current_mesh->m_bbMin[0];
			my_bbMin.value[1] = current_mesh->m_bbMin[1];
			my_bbMin.value[2] = current_mesh->m_bbMin[2];
			my_bbMax.value[0] = current_mesh->m_bbMax[0];
			my_bbMax.value[1] = current_mesh->m_bbMax[1];
			my_bbMax.value[2] = current_mesh->m_bbMax[2];

			// 모델을 화면 크기에 맞추기


			
			current_mesh->ModelName = fileinfo.fileName();
			current_mesh->ModelID = count_loadedmodel;
			
		
			GLOptionWidget *GLOptionwidget= (GLOptionWidget *)(GLOptiondock->widget()); 
			GLOptionwidget->setParent(GLOptiondock);
			GLOptionwidget->CreateModelListItem(current_mesh->ModelID, current_mesh->ModelName);


			m_TriMeshs.push_back(current_mesh);

			tabs->setCurrentIndex(0);
			myGlWidget = (GLWidget *)tabs->currentWidget();
			myGlWidget->makeCurrent();
			
			myGlWidget->GetModel(&m_TriMeshs);
			myGlWidget->GLSetTexture(current_mesh->m_texture_width, current_mesh->m_texture_height, current_mesh->m_texture);
			//	myGlWidget->initializeGL();
			//myGlWidget->GLOnViewIso();
			myGlWidget->GLOnViewBottom();

			myGlWidget->GLZoomToFit(&my_bbMin, &my_bbMax);
			//myGlWidget->m_bVertexNormal = true;
			
			
			myGlWidget->updateGL();
			myGlWidget->doneCurrent();
			

			count_loadedmodel++;

		//	delete current_mesh;
			//logviewdockwidget->addText("\n===================Object===========================\n");
			//logviewdockwidget->addText("Opened: " + filePath + "\n");
			statusBar()->showMessage(QString("Opened: %1").arg(filePath), 3000);

		}


#ifndef QT_NO_CURSOR
		QApplication::restoreOverrideCursor();
#endif


		this->repaint();	//equal to Invalidate() in mfc

	}


	setCurrentFile(filePath);
	//statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

void MainWindow::bilateralfilter()
{
	m_TriMeshs[ext_current_model_index]->FilterBilateral();
	GLWidget* myGlWidget = (GLWidget*)tabs->widget(0);
	myGlWidget->makeCurrent();
	myGlWidget->updateGL();
	myGlWidget->doneCurrent();
	myGlWidget->repaint();	//equal to Invalidate() in mfc
}
//! [44]
bool MainWindow::saveFile(const QString &filePath)
//! [44] //! [45]
{

	QFile file(filePath);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(filePath)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

	QFileInfo fileinfo(file.fileName());
	QString m_filePath(fileinfo.filePath());
	// QString m_filePath("fandisk.obj");
	QString extension(fileinfo.suffix());

	bool filesaveopt[3] = { false };

	//if(optDlg.DoModal() == IDOK){

	//	if(optDlg.m_bVertexColor == TRUE)
	//		filesaveopt[0] = true;
	//	if(optDlg.m_bVertexNormal == TRUE)
	filesaveopt[1] = true;
	//	if(optDlg.m_bTextureCoord == TRUE)
	//		filesaveopt[2] = true;
	//}


	m_TriMeshs[current_mesh->ModelID]->WriteMeshFile(filePath, extension, filesaveopt);

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(filePath);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &filePath)
//! [46] //! [47]
{
	curFile = filePath;

	setWindowModified(false);

	QString shownName = curFile;
	if (curFile.isEmpty())
		shownName = "untitled.obj";
	setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
	return QFileInfo(fullFileName).filePath();
}

void MainWindow::menu_implicitsurfacereconHOPPE()
{
	myGlWidget = (GLWidget*)tabs->widget(0);
	myGlWidget->initializeGL();
	myVec::Vec3f my_bbMin, my_bbMax;
	//for (int i = 0; i < m_TriMeshs.size(); i++)
	//{
	m_TriMeshs[ext_current_model_index]->Implicit_Surface_Reconstruction();


	//	}	

	myGlWidget->GetModel(&m_TriMeshs);


	my_bbMin.value[0] = m_TriMeshs[0]->m_bbMin[0];
	my_bbMin.value[1] = m_TriMeshs[0]->m_bbMin[1];
	my_bbMin.value[2] = m_TriMeshs[0]->m_bbMin[2];
	my_bbMax.value[0] = m_TriMeshs[0]->m_bbMax[0];
	my_bbMax.value[1] = m_TriMeshs[0]->m_bbMax[1];
	my_bbMax.value[2] = m_TriMeshs[0]->m_bbMax[2];


//myGlWidget->makeCurrent();
	myGlWidget->GLZoomToFit(&my_bbMin, &my_bbMax);


	myGlWidget->GLOnViewIso();
	
	myGlWidget->updateGL();
	//myGlWidget->doneCurrent();
	//myGlWidget->repaint();	//equal to Invalidate() in mfc
	//myGlWidget->m_bVertexNormal = true;
}

void MainWindow::menu_implicitsurfacereconSDF()
{
	myGlWidget = (GLWidget*)tabs->widget(0);
	myGlWidget->initializeGL();
	myVec::Vec3f my_bbMin, my_bbMax;

	QString selfilter = tr("SDF Files (*.sdf);;");
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open points file"),
		tr("../models/"),
		tr("All Files (*.*);; SDF Files (*.sdf *.SDF)"), &selfilter);

	//---------Set current directory--------------------------------------------------------------
	//	QFileInfo  file_(filePath);
	//QDir::setCurrent(file_.path());
	//--------------------------------------------------------------------------------------------

	QFileInfo  file_(filePath);
	QString extentsion = file_.suffix();

	std::ifstream fs;

	if (!filePath.isEmpty())
		fs.open(filePath.toStdString());
	else
		return;
	
	current_mesh = new Geometry3D;
	OpenMesh::IO::ImporterT<GeoTriMesh> importer(current_mesh->m_mesh);
	OpenMesh::VPropHandleT<float> sdfs;
	current_mesh->m_mesh.add_property(sdfs);

	current_mesh->InitializeMesh();
	
	current_mesh->m_mesh.request_vertex_normals();
	current_mesh->m_mesh.request_vertex_status();
	

	string v;
	GeoTriMesh::Point pt;
	GeoTriMesh::Normal n;
	float sdf;
	std::vector<float> vec_sdf;
	while (!fs.eof())
	{
		fs >> v >> pt[0] >> pt[1] >> pt[2];
		fs >> v >> n[0] >> n[1] >> n[2];
		fs >> v >> sdf;
		vec_sdf.push_back(sdf);
		OpenMesh::VertexHandle vh = importer.add_vertex(OpenMesh::Vec3f(pt[0], pt[1], pt[2]));
		//importer.set_normal(vh, n);
		current_mesh->m_mesh.property(sdfs, vh) = sdf;
	}
	current_mesh->CalcAvgEdgeLen();
	current_mesh->CalculateBoundingBox();
	current_mesh->UpdateMesh();		// Display를 위해 노말을 꼭 업데이트 할 것

	my_bbMin.value[0] = current_mesh->m_bbMin[0];
	my_bbMin.value[1] = current_mesh->m_bbMin[1];
	my_bbMin.value[2] = current_mesh->m_bbMin[2];
	my_bbMax.value[0] = current_mesh->m_bbMax[0];
	my_bbMax.value[1] = current_mesh->m_bbMax[1];
	my_bbMax.value[2] = current_mesh->m_bbMax[2];

	// 모델을 화면 크기에 맞추기



	current_mesh->ModelName = file_.fileName();
	current_mesh->ModelID = count_loadedmodel;


	GLOptionWidget *GLOptionwidget = (GLOptionWidget *)(GLOptiondock->widget());
	GLOptionwidget->setParent(GLOptiondock);
	GLOptionwidget->CreateModelListItem(current_mesh->ModelID, current_mesh->ModelName);


	m_TriMeshs.push_back(current_mesh);

	tabs->setCurrentIndex(0);
	myGlWidget = (GLWidget *)tabs->currentWidget();
	myGlWidget->makeCurrent();

	myGlWidget->GetModel(&m_TriMeshs);
	myGlWidget->GLSetTexture(current_mesh->m_texture_width, current_mesh->m_texture_height, current_mesh->m_texture);
	//	myGlWidget->initializeGL();
	myGlWidget->GLOnViewIso();
	//myGlWidget->GLOnViewBottom();

	myGlWidget->GLZoomToFit(&my_bbMin, &my_bbMax);
	//myGlWidget->m_bVertexNormal = true;


	myGlWidget->updateGL();
	myGlWidget->doneCurrent();


	count_loadedmodel++;

	//	delete current_mesh;
	//logviewdockwidget->addText("\n===================Object===========================\n");
	//logviewdockwidget->addText("Opened: " + filePath + "\n");
	statusBar()->showMessage(QString("Opened: %1").arg(filePath), 3000);


	m_TriMeshs[ext_current_model_index]->Implicit_Surface_Reconstruction(&vec_sdf);


	//	}	

	
}
void MainWindow::menu_Plugin_Kinect()
{
	/*
	//------- GLOption Dock -------------------------------------
	GLOptiondock =new QDockWidget(tr("GLOption"), this);

	//MainWindow에서 DockWidget 변수전달
	GLOptionwidget= new GLOptionWidgetT<GLWidget>(myGlWidget);

	GLOptiondock->setWidget(GLOptionwidget);

	//  GLOptionwidget->setFeatures( QDockWidget::AllDockWidgetFeatures );

	GLOptiondock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

	// GLOptionwidget->createGLOptionWidget();

	addDockWidget(Qt::RightDockWidgetArea, GLOptiondock);

	*/
	//------- GLOption Dock -------------------------------------
#ifdef _WIN32
	QDockWidget* KinectOptiondock = new QDockWidget(tr("KinectOption"), this);
	GLWidget* myGlWidget = (GLWidget*)tabs->widget(0);
	//MainWindow에서 DockWidget 변수전달
	KinectOptionWidgetT<GLWidget> *KinectOption = new KinectOptionWidgetT<GLWidget>(myGlWidget);

	KinectOptiondock->setWidget(KinectOption);

	//  GLOptionwidget->setFeatures( QDockWidget::AllDockWidgetFeatures );

	KinectOptiondock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	KinectOptiondock->setMinimumWidth(300);
	// GLOptionwidget->createGLOptionWidget();

	addDockWidget(Qt::LeftDockWidgetArea, KinectOptiondock);
#endif

}

void MainWindow::embeddedDeformation()
{
	// set deformable mesh.

	QDockWidget* Deformationdock = new QDockWidget(tr("DeformationOption"), this);
	DeformationWidget *Deformationwidget;
	GLWidget* myGlWidget = (GLWidget*)tabs->widget(0);
	Deformationwidget = new DeformationWidget(this);

	Deformationdock->setWidget(Deformationwidget);
	Deformationdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
	Deformationdock->setMinimumWidth(300);
	addDockWidget(Qt::LeftDockWidgetArea, Deformationdock);


}