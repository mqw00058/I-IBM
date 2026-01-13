#ifndef KINECTBASIC_H
#define KINECTBASIC_H


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <kinect.h>

//#include "MemoryLeakDetector/DebugNew.h"

#undef MIN 
#undef MAX
#undef min
#undef max
using namespace std;

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class KinectBasic
{
public:
	KinectBasic();
	~KinectBasic();

	IKinectSensor* pKinectSensor;
	IMultiSourceFrameReader* pMultiSourceFrameReader;
	ICoordinateMapper* pCoordinateMapper;

	unsigned short* pDepthBuffer;
	unsigned char* pDepthData;
	unsigned char* pColorData;
	unsigned short* pInfraredBuffer;
	unsigned char* pInfraredData;
	//unsigned char* pBodyIndexData;
	RGBQUAD* pColorBuffer;

	CameraSpacePoint* pCameraSpacePoints; 
	DepthSpacePoint* pDepthSpacePoints; 
	ColorSpacePoint* pColorSpacePoints; 

	cv::Mat mDepth;
	cv::Mat mDepthLarge;
	cv::Mat mColor;
	cv::Mat mCameraSpacePoint; 
	cv::Mat mInfrared;

	HANDLE hStopEvent, hKinectThread, hDepthMutex, hColorMutex, hInfraredMutex/*, hBodyIndexMutex*/;
	CRITICAL_SECTION mCriticalSection;

	INT64 nStartTime;
	INT64 nFrameCounter;

	//bool oPickBodyIndex;
	bool oThresholdDepth;
	bool oThresholdInfrared;
	bool oRemoveFlyingPixels;
	bool oApplyNLMFilter;

	//int iPickedBodyIndex;
	int iThresholdDepth;
	int iThresholdInfrared;
	int iThresholdNormalAngle;  // use vertex normal to remove flying pixels.

	static const int nDepthWidth;
	static const int nDepthHeight;
	static const int nColorWidth;
	static const int nColorHeight;
	static const int nInfraredWidth;
	static const int nInfraredHeight;
	static const int nDepthCount;
	static const int nColorCount;
	static const int nInfraredCount;

	static const string strWindowDepth;
	static const string strWindowColor;
	static const string strWindowInfrared;

	HRESULT InitializeDefaultSensor();
	void ProcessThreadInternal();

	void ProcessFrame(
		INT64 nTime,
		const UINT16* pDepthSrc,
		const UINT16* pInfraredSrc/*,
		const BYTE* pBodyIndexSrc*/);
	void Update();

	//void Toggle_PickBodyIndex(string& dispString);
	void Toggle_ThresholdDepthMode();
	void Toggle_ThresholdInfraredMode();
	void Toggle_RemoveFlyingPixelsMode();
	void Toggle_ApplyNLMFilterMode();
	//void Set_PickedBodyIndex(const char bodyIndex, string& dispString);


	WAITABLE_HANDLE hFrameEvent;
};
#endif