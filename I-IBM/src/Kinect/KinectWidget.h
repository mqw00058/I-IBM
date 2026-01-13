#ifndef KINECTWIDGET_H
#define KINECTWIDGET_H

#include <QWidget>
#include <QtGui>
#include "KinectBasic.h"
#include <opencv2/core/core.hpp>


class KinectWidget : public QWidget
{
	Q_OBJECT

public:
	KinectWidget(){};
	~KinectWidget(){};


public slots:
    	virtual inline void ConnectKinect(){};
// 		virtual inline void ONOFF_DrawEdge(bool){};
// 		virtual inline void ONOFF_DrawVertex(bool){};
// 		virtual inline void ONOFF_DrawFace(bool){};
// 		virtual inline void ONOFF_Centering(bool){};
// 		virtual inline void ONOFF_DrawBoundingBox(bool){};
// 		virtual inline void ONOFF_DrawWorkspace(bool){};
// 		virtual inline void ONOFF_DrawAxis(bool){};
// 
// 		virtual inline void ONOFF_SelectPointsMode(bool){};
// 		virtual inline void ONOFF_SelectFacesMode(bool){};
};


template <class T>
class KinectWidgetT : public KinectWidget
{

public:
	T*			visionView;

	KinectWidgetT(T *Ptr);
	~KinectWidgetT();

// 	void CreateModelViewOptions(QWidget *);
// 	void CreateModelSelectionOptions(QWidget*);
// 	void CreateRenderSceneOptions(QLayout* RenderSceneOptionLayout);

public:
	void ConnectKinect();
	void InitializeTextureInfo();
// 	void ONOFF_DrawEdge(bool onff);
// 	void ONOFF_DrawVertex(bool onff);
// 	void ONOFF_DrawFace(bool onff);
// 	void ONOFF_Centering(bool onff);
// 	void ONOFF_SelectPointsMode(bool onff);
// 	void ONOFF_SelectFacesMode(bool onff);
// 
// 	void ONOFF_DrawBoundingBox(bool onff);
// 	void ONOFF_DrawWorkspace(bool onff);
// 	void ONOFF_DrawAxis(bool onff);

	


	Mat dispTextureIndices;
	Mat dispTextureCoordinates;
	int width;
	int height;

};

//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(KINECTWIDGET_CPP)
#define KINECTWIDGET_TEMPLATES
#include "KinectOptionWidget.cpp"
#endif
//============================================================================
#endif // GLOPTIONWIDGET_H
