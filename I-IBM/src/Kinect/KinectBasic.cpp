#include <gl\freeglut.h>		// OpenGL header files
#include "KinectBasic.h"
#include "NLMFilter.h"

const int KinectBasic::nDepthWidth = 512;
const int KinectBasic::nDepthHeight = 424;
const int KinectBasic::nColorWidth = 1920;
const int KinectBasic::nColorHeight = 1080;
const int KinectBasic::nInfraredWidth = KinectBasic::nDepthWidth;
const int KinectBasic::nInfraredHeight = KinectBasic::nDepthHeight;
const int KinectBasic::nDepthCount = nDepthWidth * nDepthHeight;
const int KinectBasic::nColorCount = nColorWidth * nColorHeight;
const int KinectBasic::nInfraredCount = nInfraredWidth * nInfraredHeight;

const string KinectBasic::strWindowDepth = "Depth";
const string KinectBasic::strWindowColor = "Color";
const string KinectBasic::strWindowInfrared = "Infrared";

KinectBasic::KinectBasic() :
	pKinectSensor(NULL),
	pCoordinateMapper(NULL),
	pMultiSourceFrameReader(NULL),
	pDepthBuffer(NULL),
	pDepthData(NULL),
	pColorBuffer(NULL),
	pColorData(NULL),
	pInfraredBuffer(NULL),
	pInfraredData(NULL),
	pCameraSpacePoints(NULL),
	pColorSpacePoints(NULL),
	pDepthSpacePoints(NULL),
	/*oPickBodyIndex(false),*/
	oThresholdDepth(true),
	oThresholdInfrared(true),
	oRemoveFlyingPixels(true),
	oApplyNLMFilter(false),
	/*iPickedBodyIndex(255),*/
	iThresholdDepth(1200),
	iThresholdInfrared(4000),
	iThresholdNormalAngle(70),
	nStartTime(0),
	nFrameCounter(0)
{
	pDepthBuffer = new unsigned short[nDepthCount];
	pDepthData = new unsigned char[nDepthCount];
	pColorBuffer = new RGBQUAD[nColorCount];
	pColorData = new unsigned char[nColorCount * 3];
	pInfraredBuffer = new unsigned short[nInfraredCount];
	pInfraredData = new unsigned char[nInfraredCount];

	pCameraSpacePoints = new CameraSpacePoint[nColorCount];
	pDepthSpacePoints = new DepthSpacePoint[nDepthCount];

	memset(pDepthBuffer, 0, sizeof(unsigned short) * nDepthCount);
	memset(pDepthData, 0, sizeof(unsigned char) * nDepthCount);
	memset(pColorBuffer, 0, sizeof(RGBQUAD) * nColorCount);
	memset(pColorData, 0, sizeof(unsigned char) * nColorCount * 3);
	memset(pInfraredBuffer, 0, sizeof(unsigned short) * nInfraredCount);
	memset(pInfraredData, 0, sizeof(unsigned char) * nInfraredCount);
	memset(pCameraSpacePoints, 0, sizeof(CameraSpacePoint) * nColorCount);
	memset(pDepthSpacePoints, 0, sizeof(DepthSpacePoint) * nDepthCount);

	mDepth = Mat(nDepthHeight, nDepthWidth, CV_8UC1, pDepthData);
	mDepthLarge = Mat::zeros(nColorHeight, nColorWidth, CV_8UC1);
	mColor = Mat(nColorHeight, nColorWidth, CV_8UC3, pColorData);
	mCameraSpacePoint = Mat(nColorHeight, nColorWidth, CV_32FC3, pCameraSpacePoints);
	mInfrared = Mat(nInfraredHeight, nInfraredWidth, CV_8UC1, pInfraredData);
}

KinectBasic::~KinectBasic()
{
	if(hStopEvent != NULL) {
		//signal the process to stop
		SetEvent(hStopEvent);

		if(hKinectThread != NULL) {
			WaitForSingleObject(hKinectThread,INFINITE);
			CloseHandle(hKinectThread);
			hKinectThread = NULL;
		}
		CloseHandle(hStopEvent);
		hStopEvent = NULL;

		CloseHandle(hDepthMutex);
		hDepthMutex = NULL;

		CloseHandle(hColorMutex);
		hColorMutex = NULL;

		CloseHandle(hInfraredMutex);
		hColorMutex = NULL;
	}

	if(pDepthBuffer != NULL)	delete [] pDepthBuffer;
	if(pDepthData != NULL)		delete [] pDepthData;
	if(pColorBuffer != NULL)	delete [] pColorBuffer;
	if(pColorData != NULL)		delete [] pColorData;
	if(pInfraredBuffer != NULL)	delete [] pInfraredBuffer;
	if(pInfraredData != NULL)	delete [] pInfraredData;

	if(pCameraSpacePoints != NULL)	delete [] pCameraSpacePoints;
	if(pDepthSpacePoints != NULL)	delete [] pDepthSpacePoints;

	SafeRelease(pCoordinateMapper);
	SafeRelease(pMultiSourceFrameReader);

	if(pKinectSensor != NULL)	pKinectSensor->Close();
	SafeRelease(pKinectSensor);
}

DWORD ProcessThread(LPVOID pParam) {
	KinectBasic *p = (KinectBasic*) pParam;
	p->ProcessThreadInternal();
	return 0;
}

void KinectBasic::ProcessThreadInternal()
{
	bool quit = false;
	while(!quit) {
		// Wait for any of the events to be signalled
		if(WaitForSingleObject(hStopEvent,1) == WAIT_OBJECT_0)
			quit = true;
		else {
			//Get the newest frame info
			Update();
		}
	}
}

HRESULT KinectBasic::InitializeDefaultSensor()
{
	HRESULT hr;

	hr = GetDefaultKinectSensor(&pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the fream reader
		if(SUCCEEDED(hr))
		{
			hr = pKinectSensor->get_CoordinateMapper(&pCoordinateMapper);
		}

		if(SUCCEEDED(hr))
		{
			hr = pKinectSensor->Open();
		}

		if(SUCCEEDED(hr))
		{
			hr = pKinectSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth |
				FrameSourceTypes::FrameSourceTypes_Color |
				FrameSourceTypes::FrameSourceTypes_Infrared |
				FrameSourceTypes::FrameSourceTypes_BodyIndex,
				&pMultiSourceFrameReader);
		}
	}

	if (pKinectSensor == NULL || FAILED(hr))
	{
		cerr << "No ready Kinect found" << endl;
		return E_FAIL;
	}

	hDepthMutex = CreateMutex(NULL,false,NULL);
	if(hDepthMutex == NULL)
		throw exception("Could not create depth mutex");

	hColorMutex = CreateMutex(NULL,false,NULL);
	if(hColorMutex == NULL)
		throw exception("Could not create color mutex");

	hInfraredMutex = CreateMutex(NULL,false,NULL);
	if(hInfraredMutex == NULL)
		throw exception("Could not create infrared mutex");

	hStopEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	//hKinectThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&ProcessThread, this, 0, NULL );


	InitializeCriticalSection(&mCriticalSection);

	return hr;
}

void KinectBasic::ProcessFrame(
	INT64 nTime, 
	const UINT16* pDepthSrc,
	const UINT16* pInfraredSrc/*,
	const BYTE* pBodyIndexSrc*/)
{
	// process time
	if(nStartTime == 0) nStartTime = nTime;
	else if(nStartTime != nTime)
	{
		nFrameCounter++;
	}

	// copy depth data
	//if(oPickBodyIndex)
	//{
	//	const unsigned char pickedBodyIndex = static_cast<unsigned char>(iPickedBodyIndex);
	//	vector<char> registeredIndices;
	//	for(int ii = 0; ii < nDepthCount; ii++)
	//	{
	//		if(pBodyIndexSrc[ii] == pickedBodyIndex)
	//			pDepthBuffer[ii] = pDepthSrc[ii];
	//		else pDepthBuffer[ii] = 0;
	//	}
	//}
	//else memcpy(pDepthBuffer, pDepthSrc, sizeof(UINT16) * nDepthCount);

	WaitForSingleObject(hDepthMutex,INFINITE);
	memcpy(pDepthBuffer, pDepthSrc, sizeof(UINT16) * nDepthCount);
	ReleaseMutex(hDepthMutex);

	// process color: convert to RGB
	EnterCriticalSection(&mCriticalSection); 
	int idx_char = 0;
	int idx_quad = 0;
	for(int rr = 0; rr < nColorHeight; rr++)
	{
		for(int cc = 0; cc < nColorWidth; cc++, idx_quad++)
		{
			this->pColorData[idx_char++] = pColorBuffer[idx_quad].rgbBlue;
			this->pColorData[idx_char++] = pColorBuffer[idx_quad].rgbGreen;
			this->pColorData[idx_char++] = pColorBuffer[idx_quad].rgbRed;
		}
	}

	memcpy(pDepthBuffer, pDepthSrc, sizeof(UINT16) * nDepthCount);

	// copy infrared data
	memcpy(pInfraredBuffer, pInfraredSrc, sizeof(UINT16) * nInfraredCount);
	LeaveCriticalSection(&mCriticalSection); 


	if(oApplyNLMFilter)
	{
		Mat mDepthData = Mat(nDepthHeight, nDepthWidth, CV_16UC1, pDepthBuffer);
		Mat mDepthDataNLM = Mat(nDepthHeight, nDepthWidth, CV_16UC1);
		nonlocalMeansFilter16U(mDepthData, mDepthDataNLM, 3, 5, 20 );

		mDepthDataNLM.copyTo(mDepthData);
	}

	// thresholding depth data by depth
	if(oThresholdDepth)
	{
		for(int ii = 0; ii < nDepthCount; ii++)
			if(pDepthBuffer[ii] < 0 || pDepthBuffer[ii] > iThresholdDepth)
				pDepthBuffer[ii] = 0;
	}

	// thresholding depth data by infrared
	if(oThresholdInfrared)
	{
		for(int ii = 0; ii < nDepthCount; ii++)
		{
			if( pInfraredBuffer[ii] < iThresholdInfrared )
				pDepthBuffer[ii] = 0;
		}
	}

	HRESULT hr;
	//remove flying pixels using the angle between the camera ray and vertex normal
	if(oRemoveFlyingPixels)
	{

		DepthSpacePoint *pDepthPoints = new DepthSpacePoint[ nDepthCount ];
		for(int rr = 1; rr < nDepthHeight-1; rr++)
			for(int cc = 1; cc < nDepthWidth-1; cc++)
			{
				pDepthPoints[ rr * nDepthWidth + cc ].X =(float)cc;
				pDepthPoints[ rr * nDepthWidth + cc ].Y =(float) rr;
			}

			CameraSpacePoint* pCameraSpacePoints2 = new CameraSpacePoint[ nDepthCount ]; 
			hr = pCoordinateMapper->MapDepthPointsToCameraSpace( nDepthCount, pDepthPoints, nDepthCount, pDepthBuffer, nDepthCount, pCameraSpacePoints2 );

			// remove flying pixels using their normals
			//http://www.lighthouse3d.com/opengl/terrain/index.php3?normals
			for(int rr = 1; rr < nDepthHeight-1; rr++)
				for(int cc = 1; cc < nDepthWidth-1; cc++)
				{
					//    p1
					// p2 p0 p4
					//    p3

					CameraSpacePoint cp0, cp1, cp2, cp3, cp4;

					cp0 = pCameraSpacePoints2[ rr*nDepthWidth + cc ];

					if( cp0.Z > 0 )
					{
						Vec3f p0 = Vec3f(cp0.X, cp0.Y, cp0.Z);

						cp1 = pCameraSpacePoints2[ (rr-1)*nDepthWidth + cc ];
						Vec3f p1 = Vec3f(cp1.X, cp1.Y, cp1.Z);

						cp2 = pCameraSpacePoints2[ rr*nDepthWidth + cc-1 ];
						Vec3f p2 = Vec3f(cp2.X, cp2.Y, cp2.Z);

						cp3 = pCameraSpacePoints2[ (rr+1)*nDepthWidth + cc ];
						Vec3f p3 = Vec3f(cp3.X, cp3.Y, cp3.Z);

						cp4 = pCameraSpacePoints2[ rr*nDepthWidth + cc+1 ];
						Vec3f p4 = Vec3f(cp4.X, cp4.Y, cp4.Z);

						if( p0.val[2] > 0 && p1.val[2] > 0 && p2.val[2] > 0 && p3.val[2] > 0 && p4.val[2] > 0)
						{      
							Vec3f v1 = p1 - p0;
							Vec3f v2 = p2 - p0;
							Vec3f v3 = p3 - p0;
							Vec3f v4 = p4 - p0;

							Vec3f v12 = v1.cross(v2);
							Vec3f v23 = v2.cross(v3);
							Vec3f v34 = v3.cross(v4);
							Vec3f v41 = v4.cross(v1);

							Vec3f normal = (v12 + v23 + v34 + v41)/norm(v12 + v23 + v34 + v41);
							float angle = acos(normal.dot(Vec3f(0,0,1)));

							if( angle > M_PI / 180.0 * iThresholdNormalAngle )
								pDepthBuffer[rr*nDepthWidth + cc] = 0;
						}
						else
							pDepthBuffer[rr*nDepthWidth + cc] = 0;
					}
				}

				delete []pCameraSpacePoints2;

		delete []pDepthPoints;
	}

	// convert points to camera space
	hr = pCoordinateMapper->MapColorFrameToCameraSpace(
		nDepthCount,
		pDepthBuffer,
		nColorCount,
		pCameraSpacePoints);


	// recompute x, y using z and focal length
	//focal length [ 1063.018  1065.133 ] ¡¾ [ 1.880  1.889 ]
	//principal point [ 962.373  526.689 ] ¡¾ [ 1.085  0.885 ]
	//distortion [ 0.042369  -0.037696  -0.002894  0.000978 ] ¡¾ [ 0.002178  0.009347  0.000238  0.000308 ]
	float fl_x = (float)1063.018;
	float fl_y = (float)1065.133;
	float pp_x = (float)962.373;
	float pp_y = (float)526.689;

	EnterCriticalSection(&mCriticalSection); 
	for(int rr = 0; rr < nColorHeight; rr++)
		for(int cc = 0; cc < nColorWidth; cc++)
		{
			float Z = mCameraSpacePoint.at<float>(rr, cc*3+2);
			if( Z > 0 )
			{
				//x_p = f * x / z - cx
				//x = (x_p - cx ) * z / f
				mCameraSpacePoint.at<float>(rr, cc*3+0) = (cc - pp_x) * Z / fl_x;
				mCameraSpacePoint.at<float>(rr, cc*3+1) = -(rr - pp_y) * Z / fl_y;
			}        
		}
		LeaveCriticalSection(&mCriticalSection); 


		EnterCriticalSection(&mCriticalSection); 
		if (SUCCEEDED(hr))
		{
			// process depth
			for(int ii = 0; ii < nDepthCount; ii++)
			{
				pDepthBuffer[ii] = (pDepthBuffer[ii] & 0xfff8) >> 3;
				pDepthData[ii] = pDepthBuffer[ii] % 256;
			}

			// process infrared
			for(int ii = 0; ii < nInfraredCount; ii++)
			{
				pInfraredBuffer[ii] = pInfraredBuffer[ii] >> 8;
				pInfraredData[ii] = pInfraredBuffer[ii] % 256;
			}

			flip(mDepth, mDepth,1);
			imshow(strWindowDepth, mDepth);
			Mat mColorRz;
			resize(mColor, mColorRz, Size(mColor.cols/2, mColor.rows/2));
			flip(mColorRz, mColorRz,1);
			imshow(strWindowColor, mColorRz);
			flip(mInfrared, mInfrared,1);
			imshow(strWindowInfrared, mInfrared);
		}
		else cout << "ProcessFrame failed." << endl;
		LeaveCriticalSection(&mCriticalSection); 
}

void KinectBasic::Update()
{
	if (pMultiSourceFrameReader == NULL)
	{
		return;
	}

	IMultiSourceFrame* pMultiSourceFrame = NULL;
	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;
	IInfraredFrame* pInfraredFrame = NULL;
	IBodyIndexFrame* pBodyIndexFrame = NULL;

	EnterCriticalSection(&mCriticalSection); 
	HRESULT hr = pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);
	LeaveCriticalSection(&mCriticalSection); 
	BOOLEAN test;
	pMultiSourceFrameReader->get_IsPaused(&test);
	if(test)
		return;

	if (FAILED(hr))
		printf("AcquireLatestFrame(&pMultiSourceFrame) failed.\n");

	// acquire depth frame
	if (SUCCEEDED(hr))
	{
		IDepthFrameReference* pDepthFrameReference = NULL;

		EnterCriticalSection(&mCriticalSection); 
		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		LeaveCriticalSection(&mCriticalSection); 
		if (FAILED(hr))
			printf("get_DepthFrameReference(&pDepthFrameReference) failed.\n");

		if (SUCCEEDED(hr))
		{
			EnterCriticalSection(&mCriticalSection); 
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
			LeaveCriticalSection(&mCriticalSection); 

			if (FAILED(hr))
				printf("AcquireFrame(&pDepthFrame) failed.\n");
		}

		SafeRelease(pDepthFrameReference);
	}

	// acquire color frame
	if (SUCCEEDED(hr))
	{
		IColorFrameReference* pColorFrameReference = NULL;

		EnterCriticalSection(&mCriticalSection); 
		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		LeaveCriticalSection(&mCriticalSection); 
		if (FAILED(hr))
			printf("get_ColorFrameReference(&pColorFrameReference) failed.\n");

		if (SUCCEEDED(hr))
		{
			EnterCriticalSection(&mCriticalSection); 
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
			LeaveCriticalSection(&mCriticalSection); 
			if (FAILED(hr))
				printf("AcquireFrame(&pColorFrame) failed.\n");
		}

		SafeRelease(pColorFrameReference);
	}

	// acquire infrared frame
	if(SUCCEEDED(hr))
	{
		IInfraredFrameReference* pInfraredFrameReference = NULL;

		EnterCriticalSection(&mCriticalSection); 
		hr = pMultiSourceFrame->get_InfraredFrameReference(&pInfraredFrameReference);
		LeaveCriticalSection(&mCriticalSection); 
		if (FAILED(hr))
			printf("get_InfraredFrameReference(&pInfraredFrameReference) failed.\n");

		if(SUCCEEDED(hr))
		{
			EnterCriticalSection(&mCriticalSection); 
			hr = pInfraredFrameReference->AcquireFrame(&pInfraredFrame);
			LeaveCriticalSection(&mCriticalSection); 
			if (FAILED(hr))
				printf("AcquireFrame(&pInfraredFrame) failed.\n");
		}

		SafeRelease(pInfraredFrameReference);
	}

	// acquire body index frame
	/*if (SUCCEEDED(hr))
	{
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;

		hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
		}

		SafeRelease(pBodyIndexFrameReference);
	}*/

	if (SUCCEEDED(hr))
	{
		INT64 nDepthTime = 0;
		IFrameDescription* pDepthFrameDescription = NULL;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxReliableDistance = 0;
		int nDepthWidth = 0;
		int nDepthHeight = 0;
		UINT nDepthBufferSize = 0;
		UINT16 *pDepthBuffer = NULL;

		IFrameDescription* pColorFrameDescription = NULL;
		int nColorWidth = 0;
		int nColorHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;

		IFrameDescription* pInfraredFrameDescription = NULL;
		int nInfraredWidth = 0;
		int nInfraredHeight = 0;
		UINT nInfraredBufferSize = 0;
		UINT16* pInfraredBuffer = NULL;

		/*IFrameDescription* pBodyIndexFrameDescription = NULL;
		int nBodyIndexWidth = 0;
		int nBodyIndexHeight = 0;*/
		UINT nBodyIndexBufferSize = 0;
		BYTE* pBodyIndexBuffer = NULL;

		// get depth frame data
		{
			hr = pDepthFrame->get_RelativeTime(&nDepthTime);

			if (SUCCEEDED(hr))
				hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);

			if (SUCCEEDED(hr))
				hr = pDepthFrameDescription->get_Width(&nDepthWidth);

			if (SUCCEEDED(hr))
				hr = pDepthFrameDescription->get_Height(&nDepthHeight);

			if (SUCCEEDED(hr))
				hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);

			if (SUCCEEDED(hr))
				hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxReliableDistance);

			if (SUCCEEDED(hr))
			{
				EnterCriticalSection(&mCriticalSection); 
				hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
				LeaveCriticalSection(&mCriticalSection); 
				if (FAILED(hr))
					printf("AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer) failed.\n");

			}
		}

		// get color frame data
		{
			if (SUCCEEDED(hr))
				hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);

			if (SUCCEEDED(hr))
				hr = pColorFrameDescription->get_Width(&nColorWidth);

			if (SUCCEEDED(hr))
				hr = pColorFrameDescription->get_Height(&nColorHeight);

			if (SUCCEEDED(hr))
				hr = pColorFrame->get_RawColorImageFormat(&imageFormat);

			if (SUCCEEDED(hr))
			{
				EnterCriticalSection(&mCriticalSection); 
				hr = pColorFrame->CopyConvertedFrameDataToArray(
					nColorCount * 4,
					reinterpret_cast<BYTE*>(pColorBuffer),
					ColorImageFormat_Bgra);
				LeaveCriticalSection(&mCriticalSection); 
				if (FAILED(hr))
					printf("CopyConvertedFrameDataToArray(pColorBuffer) failed.\n");
			}
		}

		// get infrared frame data
		{
			if(SUCCEEDED(hr))
				hr = pInfraredFrame->get_FrameDescription(&pInfraredFrameDescription);

			if(SUCCEEDED(hr))
				hr = pInfraredFrameDescription->get_Width(&nInfraredWidth);

			if(SUCCEEDED(hr))
				hr = pInfraredFrameDescription->get_Height(&nInfraredHeight);

			if(SUCCEEDED(hr))
			{
				EnterCriticalSection(&mCriticalSection); 
				hr = pInfraredFrame->AccessUnderlyingBuffer(&nInfraredBufferSize, &pInfraredBuffer);
				LeaveCriticalSection(&mCriticalSection); 
				if (FAILED(hr))
					printf("AccessUnderlyingBuffer(&nInfraredBufferSize, &pInfraredBuffer) failed.\n");
			}
		}

		// get body index frame data
		/*{
			if (SUCCEEDED(hr))
			{
				hr = pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
			}

			if (SUCCEEDED(hr))
			{
				hr = pBodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
			}

			if (SUCCEEDED(hr))
			{
				hr = pBodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
			}

			if (SUCCEEDED(hr))
			{
				hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
			}
		}*/

		if (SUCCEEDED(hr))
		{
			ProcessFrame(
				nDepthTime,
				pDepthBuffer,
				pInfraredBuffer/*,
				pBodyIndexBuffer*/);
		}
		else cout << "bad" << endl;

		SafeRelease(pDepthFrameDescription);
		SafeRelease(pColorFrameDescription);
		SafeRelease(pInfraredFrameDescription);
		//SafeRelease(pBodyIndexFrameDescription);
	}

	SafeRelease(pDepthFrame);
	SafeRelease(pColorFrame);
	SafeRelease(pInfraredFrame);
	SafeRelease(pBodyIndexFrame);
	SafeRelease(pMultiSourceFrame);

}

/*	
void KinectBasic::Toggle_PickBodyIndex(string& dispString)
{
this->oPickBodyIndex = !this->oPickBodyIndex;
	if(this->oPickBodyIndex == true)
		Set_PickedBodyIndex('0', dispString);
	else dispString = "";
}
*/


void KinectBasic::Toggle_ThresholdDepthMode()
{
	this->oThresholdDepth = !this->oThresholdDepth;
}

void KinectBasic::Toggle_ThresholdInfraredMode()
{
	this->oThresholdInfrared = !this->oThresholdInfrared;
}

void KinectBasic::Toggle_RemoveFlyingPixelsMode()
{
	this->oRemoveFlyingPixels = !this->oRemoveFlyingPixels;
}

void KinectBasic::Toggle_ApplyNLMFilterMode()
{
	this->oApplyNLMFilter = !this->oApplyNLMFilter;
}

/*
void KinectBasic::Set_PickedBodyIndex(const char bodyKey, string& dispString)
{
	if(bodyKey >= '0' && bodyKey <= '9' && this->oPickBodyIndex)
	{
		this->iPickedBodyIndex = bodyKey - '0';
		char buff[1024];
		sprintf_s(buff, "PickedBodyIndex: %d", bodyKey - '0');
		dispString = buff;
	}
}*/
