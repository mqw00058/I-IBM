#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

// variables for modes and their parameters
static const int paramPickedBodyIndexInit = 255;
static const int paramThresholdDepthInit = 1000;


static bool modePickBodyIndex = false;
static bool modeThresholdDepth = false;
static bool modeSaveAccumulated = false;
static bool modeSaveSelect = false;
static int paramPickedBodyIndex = paramPickedBodyIndexInit;
static int paramThresholdDepth = paramThresholdDepthInit;

static vector<Mat> paramAccPoints;
static vector<Mat> paramAccColors;
static vector<Mat> paramAccDepths;
static vector<Mat> paramAccInfrareds;

// functions for modes
void Toggle_PickBodyIndex();
void Set_PickedBodyIndex(char key);
void Toggle_ThresholdDepth();
void Toggle_SaveAccumulated(string& dispString);
void Toggle_SaveSelect(string& dispString);
void Add_Select(const Mat& mPoint, const Mat& mColor, string& dispString);
void Add_Accumulated(const Mat& mPoint, const Mat& mColor, string& dispString);
void Push_Data(const Mat& mPoint, const Mat& mColor, const string& prefix, string& dispString);
void Save_Image(const string& filename, const Mat& mData);
void Save_Depth(const string& filename, const Mat& mData, const string& tag);
void Save_Point(const string& filename, const Mat& mPoint, const Mat& mColor);
void Save(const Mat& mPoint, const Mat& mColor,  const Mat& mDepth,  const Mat& mInfrared, string& dispString, const string& path = "obj");