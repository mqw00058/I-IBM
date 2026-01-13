#ifndef GLOPTIONWIDGET_H
#define GLOPTIONWIDGET_H

#include "GLWidget.h"
#include <QPushButton>
#include <QProgressBar>
#include <QGridLayout>
 #include <QtGui>
#include <QString>
#include <QGroupBox>
#include <QCheckBox>

// class GLOptionWidget  : public QWidget
// {
// 	Q_OBJECT
// public :
// 	GLOptionWidget(){};
// 	~GLOptionWidget(){};

// public slots:
// 	virtual inline void ONOFF_DrawNormal(bool){};
// 	virtual inline void ONOFF_DrawEdge(bool){};
// 	virtual inline void ONOFF_DrawVertex(bool){};
// 	virtual inline void ONOFF_DrawFace(bool){};
// 	virtual inline void ONOFF_Centering(bool){};
// 	virtual inline void ONOFF_DrawBoundingBox(bool){};
// 	virtual inline void ONOFF_DrawWorkspace(bool){};
// 	virtual inline void ONOFF_DrawAxis(bool){};

// 	virtual inline void ONOFF_SelectPointsMode(bool){};
// 	virtual inline void ONOFF_SelectFacesMode(bool){};
// };

class GLOptionWidgetT : public QWidget
{
    Q_OBJECT
public:
    GLWidget *		visionView;

    explicit GLOptionWidgetT(GLWidget *Ptr);
	~GLOptionWidgetT();

	void CreateModelViewOptions(QWidget *);
	void CreateModelSelectionOptions(QWidget*);
	void CreateRenderSceneOptions(QLayout* RenderSceneOptionLayout);

public slots:
	void ONOFF_DrawNormal(bool onff);
	void ONOFF_DrawEdge(bool onff);
	void ONOFF_DrawVertex(bool onff);
	void ONOFF_DrawFace(bool onff);
	void ONOFF_Centering(bool onff);
	void ONOFF_SelectPointsMode(bool onff);
	void ONOFF_SelectFacesMode(bool onff);
	void ONOFF_DrawBoundingBox(bool onff);
	void ONOFF_DrawWorkspace(bool onff);
	void ONOFF_DrawAxis(bool onff);
};

// //=============================================================================
//#if defined(OM_INCLUDE_TEMPLATES) && !defined(GLOPTIONWIDET_CPP)
//#define GLOPTIONWIDGET_H
//#include "GLOptionWidget.cpp"
//#endif
// //============================================================================
#endif // GLOPTIONWIDGET_H
