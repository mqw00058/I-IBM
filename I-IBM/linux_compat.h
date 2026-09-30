// Force-included on Linux builds (see I-IBM_linux.pro): fills in what MSVC 2013 provided implicitly.
#pragma once
#ifndef _WIN32
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cfloat>
#ifdef __cplusplus
#include <cmath>
#include <algorithm>
#include <ext/hash_map>
namespace stdext { using __gnu_cxx::hash_map; }
// moc_predefs.h is generated with this header but without -I paths, hence __has_include
#if __has_include(<opencv2/core.hpp>)
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/flann.hpp>
// OpenCV C headers define MAX/MIN macros that break the Numerical Recipes MAX/MIN templates
#undef MAX
#undef MIN
#endif
#ifndef CV_RGB
#define CV_RGB(r, g, b) cv::Scalar((b), (g), (r), 0)
#endif
#endif
// Windows SDK typedefs used throughout the code base
typedef unsigned char  BYTE;
typedef unsigned int   UINT;
typedef unsigned long  DWORD;
typedef unsigned short WORD;
typedef int            BOOL;
#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif
#ifdef __cplusplus
#endif
#endif
