#ifndef KINECTOPTIONWIDGET_CPP
#define KINECTOPTIONWIDGET_CPP

#include "KinectOptionWidget.h"


//#include <vld.h> 

using namespace std;
using namespace cv;


template <class T>
KinectOptionWidgetT<T>::KinectOptionWidgetT(T *Ptr) : visionView(Ptr)
{
	ui.setupUi(this);

	dispTextureIndices = cv::Mat::zeros(kinect.cColorHeight, kinect.cColorWidth, CV_32SC1);
	dispTextureCoordinates = cv::Mat::zeros(kinect.cColorHeight, kinect.cColorWidth, CV_32FC2);
}

template <class T>
KinectOptionWidgetT<T>::~KinectOptionWidgetT()
{
	dispTextureIndices.release();
	dispTextureCoordinates.release();

	cvDestroyWindow("Color");
	cvDestroyWindow("Depth");
	//MemoryLeak::stop();
}
template <class T>
void KinectOptionWidgetT<T>::InitializeTextureInfo()
{
	// initialize texture matrices
	int index = 0;
	for(int rr = 0; rr < kinect.cColorHeight; rr++)
	{
		for(int cc = 0; cc < kinect.cColorWidth; cc++, index++)
		{
			dispTextureIndices.at<int>(rr, cc) = index;
			dispTextureCoordinates.at<cv::Vec2f>(rr, cc) =
				cv::Vec2f(static_cast<float>(cc) / kinect.cColorWidth, static_cast<float>(rr) / kinect.cColorHeight);
		}
	}

}

static int cnt =0;
template <class T>
void KinectOptionWidgetT<T>::redraw()
{
	//EnterCriticalSection(&(kinect.mCriticalSection));
	//kinect.ProcessThreadInternal();
	//if(WaitForSingleObject(hStopEvent,1) != WAIT_OBJECT_0)
	/*	kinect.Update();
	visionView->updateGL();
	//LeaveCriticalSection(&(kinect.mCriticalSection));
	//this->repaint();
	logviewdockwidget->addText(QString("%1\n").arg(cnt++));*/

		try {
		Mat image, depth, depth_colored;
		
		
	//	while(1) {
			//grabber.GetNextFrame();
			kinect.GetDepth(depth);
			kinect.GetColor(image);
			if(!image.empty())
			{
				IplImage temp =image;
				//imshow("image",image);
					cvShowImage("Color",&temp);
			}
			//else
			//	cout << "empty image" << endl;
			
			if(!depth.empty()) 
			{
				

				double min;
				double max;
				cv::minMaxIdx(depth, &min, &max);
				cv::Mat adjMap;
				// expand your range to 0..255. Similar to histEq();
				depth.convertTo(adjMap,CV_8UC1, 255 / (max-min), -min); 

				// this is great. It converts your grayscale image into a tone-mapped one, 
				// much more pleasing for the eye
				// function is found in contrib module, so include contrib.hpp 
				// and link accordingly
				cv::Mat falseColorsMap;
				applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_RAINBOW);

				//cv::imshow("Out", falseColorsMap);

				IplImage temp=falseColorsMap;//applyColorMap(depth,depth_colored,COLORMAP_JET);
				//imshow("depth",depth);
				cvShowImage("Depth",&temp);

				falseColorsMap.release();
				adjMap.release();
			} 
			//else
			//	cout << "empty depth" << endl;

			//	cvWaitKey(100);
			//	}

			visionView->updateGL();
			//kinect.mCameraSpacePoint.release();
			image.release();
			depth.release();
	} catch (std::exception &e) {
		cout << e.what() << endl;
	}
	cin.get();
}

template <class T>
void KinectOptionWidgetT<T>::ConnectKinect()
{
	
	/*InitializeCriticalSection(&(kinect.mCriticalSection));
	HRESULT hr = E_FAIL;
	while(!SUCCEEDED(hr))
	{
		hr = kinect.InitializeDefaultSensor();
		bConnectionKinect = true;
	}

	dispTextureIndices = cv::Mat::zeros(KinectBasic::nColorHeight, KinectBasic::nColorWidth, CV_32SC1);
	dispTextureCoordinates = cv::Mat::zeros(KinectBasic::nColorHeight, KinectBasic::nColorWidth, CV_32FC2);
	kinect_width =  KinectBasic::nColorWidth;
	kinect_height = KinectBasic::nColorHeight;

	InitializeTextureInfo();
	kinect.Update();
	visionView->updateGL();
	//Add_Accumulated(kinect.mCameraSpacePoint, kinect.mColor, dispString);

//	KinectOptiondock->hide();
	*/
	




	InitializeTextureInfo();
	kinect.start();
	bConnectionKinect=true;
	cvNamedWindow("Color", CV_WINDOW_NORMAL);
	cvNamedWindow("Depth", CV_WINDOW_NORMAL);

	QTimer * timer = new QTimer (this); 
	connect(timer, SIGNAL (timeout()), this, SLOT (redraw())); 
	timer-> start (uint(1000/20)); 
}

template <class T>
void KinectOptionWidgetT<T>::StopKinect()
{
	kinect.stop();
}
#endif 