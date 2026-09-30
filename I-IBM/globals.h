#ifndef GLOBALS_H
#define GLOBALS_H

#include "src/Logview/logviewdockwidget.h"
//#include "src/Kinect/KinectBasic.h"
#include "src/Kinect/Microsoft_grabber2.h"
#include <opencv2/core/core.hpp>
extern  LogviewDockWidget *logviewdockwidget;   //로그뷰창(logviewdockwidget) 클래스
//extern KinectBasic kinect;
extern KinectGrabber kinect;
extern bool bConnectionKinect;
extern cv::Mat dispTextureIndices;
extern cv::Mat dispTextureCoordinates;
extern int count_loadedmodel;
extern int ext_current_model_index;


#endif