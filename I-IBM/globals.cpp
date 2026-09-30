#include "../../globals.h"

extern LogviewDockWidget *logviewdockwidget = NULL;

//extern KinectBasic kinect=KinectBasic(); //extern은 반드시 초기화 필요
extern KinectGrabber kinect=KinectGrabber(); //extern은 반드시 초기화 필요
extern bool bConnectionKinect=false;
extern cv::Mat dispTextureIndices = cv::Mat();
extern cv::Mat dispTextureCoordinates = cv::Mat();
extern int count_loadedmodel = 0;
extern int ext_current_model_index = 0;
