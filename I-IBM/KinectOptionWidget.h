#ifndef KINECTOPTIONWIDGET_H
#define KINECTOPTIONWIDGET_H

#include <QWidget>
#include "ui_KinectOptionWidget.h"
#include "../../globals.h"
#ifdef _WIN32
#include <opencv2/contrib/contrib.hpp>
//#include <Kinect/Mode.h>
#include <winnt.h>
#else
#include <opencv2/opencv.hpp>
typedef void* HANDLE;
#endif

#include <QTimer>


class KinectOptionWidget : public QWidget
{
	Q_OBJECT

public:
	KinectOptionWidget(QWidget *parent = 0){};
	~KinectOptionWidget(){};

protected:
	Ui::KinectOptionWidget ui;

public slots:
	virtual void ConnectKinect(){};
	virtual void StopKinect(){};
	virtual void redraw(){};
};


template <class T>
class KinectOptionWidgetT : public KinectOptionWidget
{

public:
	T*			visionView;
	KinectOptionWidgetT(T *Ptr);
	~KinectOptionWidgetT();
	void ConnectKinect();
	void StopKinect();
	void InitializeTextureInfo();
	void redraw();

	void Start();
	void Stop();

private:
	std::deque<cv::Mat*> imgs;
	std::deque<cv::Mat*> depths;
	HANDLE hCaptureThread, hSaveThread, hDoneSaving;
	KinectGrabber *kc;
	bool kinect_init;
	bool quit;
	int count;
	
};
//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(KINECTOPTIONWIDGET_CPP)
#define  KINECTOPTIONWIDGET_TEMPLATES
#include "KinectOptionWidget.cpp"
#endif
//============================================================================
#endif // KINECTOPTIONWIDGET_H
