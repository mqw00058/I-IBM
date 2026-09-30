// Linux build stub for the Windows-only Kinect v2 grabber (Microsoft_grabber2.h).
// The Kinect for Windows SDK 2.0 has no Linux version, so capture is disabled;
// the rest of I-IBM (mesh loading, reconstruction, deformation, texturing) is unaffected.
#pragma once

#ifndef __OPENCV_MICROSOFT_GRABBER__
#define __OPENCV_MICROSOFT_GRABBER__

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>

#define COLOR_PIXEL_TYPE CV_8UC4
#define DEPTH_PIXEL_TYPE CV_16UC1

class KinectGrabber
{
public:
	KinectGrabber(const int instance = 0) : CameraSettingsSupported(false) {}
	~KinectGrabber() throw() {}

	void start() { std::cerr << "[I-IBM] Kinect v2 capture is only available on Windows (Kinect SDK 2.0)." << std::endl; }
	void stop() {}
	bool isRunning() const { return false; }
	std::string getName() const { return "KinectGrabber (stub)"; }
	float getFramesPerSecond() const { return 0.f; }

	bool CameraSettingsSupported;

	void GetColor(cv::Mat &color) { color = cv::Mat::zeros(cColorHeight, cColorWidth, COLOR_PIXEL_TYPE); }
	void GetDepth(cv::Mat &depth) { depth = cv::Mat::zeros(cDepthHeight, cDepthWidth, DEPTH_PIXEL_TYPE); }

	cv::Mat mCameraSpacePoint;
	cv::Mat mColor;

	static const int cColorWidth  = 1920;
	static const int cColorHeight = 1080;
	static const int cDepthWidth  = 512;
	static const int cDepthHeight = 424;
};

#endif
