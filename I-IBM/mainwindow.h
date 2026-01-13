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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>


#include "globals.h"
//#include "src/Logview/logviewdockwidget.h"
#include "src/GLWidget/GLOptionwidget.h"
//#include "src/GLWidget/GLWidget.h"
#include "src/VisionWidget/VisionWidget.h"
#include "src/Geometry3D/Geometry3D.h"
//#include "BundlerOptionWidget.h"
//#include "src/GLWidget/GL3DDoc.h"
//#include "KinectOptionWidget.h"


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
	void bilateralfilter();
	void menu_Plugin_Kinect();
	void swichdockwidget(int);
	void menu_implicitsurfacereconHOPPE();
	
private:
    void createActions();
    void createMenus();
	 void createMenus1();
    void createToolBars();
    void createStatusBar();

    void createDockWidgets();

    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &filePath);
    bool saveFile(const QString &filePath);
    void setCurrentFile(const QString &filePath);
    QString strippedName(const QString &fullFileName);

  
    QString curFile;
    
public:
	QTabWidget *tabs;

	VisionWidget *myVisionWidget;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;

	QMenu *filterMenu;
	QAction *bilateralAct;

	QMenu *pointMenu;
	QAction *implicitsurfacereconAction;

	
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

	// Plug-in 메뉴 옵션
    QAction * GLOptionAct;
	QAction * kinectAction;

    ///////////////////////////////////////////////////////////////////////////////
    QDockWidget *logdock, *GLOptiondock, *KinectOptiondock;  //MainWindow에서 DockWidget 변수선언
//	KinectOptionWidgetT<GLWidget> * KinectOption; //Kinect 그리기 옵션창(KinectWidgetT) 클래스
	GLWidget *myGlWidget;
    GLOptionWidgetT *GLOptionwidget;   //그리기 옵션창(GLOptionWidgetT) 클래스

	//BundlerOptionWidgetT<VisionWidget> *BundlerOptionwidget;

	Geometry3D  m_currentTriMesh;
};
//! [0]

#endif
