#include <time.h>
#include "Mode.h"

void Toggle_PickBodyIndex()
{
	modePickBodyIndex = !modePickBodyIndex;
	if(modePickBodyIndex == false)
		paramPickedBodyIndex = paramPickedBodyIndexInit;
}

void Set_PickedBodyIndex(char key)
{
	int ikey = key - '0';
	if(ikey < 0 || ikey > 9) return ;
	paramPickedBodyIndex = ikey;
}

void Toggle_ThresholdDepth()
{
	modeThresholdDepth = !modeThresholdDepth;
	if(modeThresholdDepth == false)
		paramThresholdDepth = paramThresholdDepthInit;
}

void Toggle_SaveAccumulated(string& dispString)
{
	modeSaveAccumulated = !modeSaveAccumulated;
	if(modeSaveAccumulated == false)
	{
		paramAccColors.clear();
		paramAccPoints.clear();
		dispString = "";
	}
}

void Toggle_SaveSelect(string& dispString)
{
	modeSaveSelect = !modeSaveSelect;
	if(modeSaveSelect == false)
	{
		paramAccColors.clear();
		paramAccPoints.clear();
		dispString = "";
	}
}

void Add_Select(const Mat& mPoint, const Mat& mColor, string& dispString)
{
	if(modeSaveSelect)
		Push_Data(mPoint, mColor, "SaveSelect", dispString);
}

void Add_Accumulated(const Mat& mPoint, const Mat& mColor, string& dispString)
{
	if(modeSaveAccumulated)
		Push_Data(mPoint, mColor, "SaveAccumulated", dispString);	
}

void Push_Data(const Mat& mPoint, const Mat& mColor, const string& prefix, string& dispString)
{
	paramAccPoints.push_back(mPoint.clone());
	paramAccColors.push_back(mColor.clone());
		
	char buff[1024];
	sprintf_s(buff, "%s: %03d", prefix.c_str(), paramAccPoints.size());
	dispString = buff;
}

void Save_Image(const string& filename, const Mat& mData)
{
	if(mData.empty()) return ;

  if(mData.type() == CV_16UC1)
    mData.convertTo(mData, CV_16UC3);
	imwrite(filename, mData);
}

void Save_Depth(const string& filename, const Mat& mData, const string& tag)
{
	if(mData.empty()) return ;
	FileStorage fs;
	fs.open(filename, FileStorage::WRITE);
	fs << tag << mData;
	fs.release();
}

void Save_Point(const string& filename, const Mat& mPoint, const Mat& mColor)
{
	if(mPoint.rows != mColor.rows || mPoint.cols != mColor.cols) return ;
	if(mPoint.type() != CV_32FC3 || mColor.type() != CV_8UC3) return ;

  ostringstream oss;
	for(int rr = 0; rr < mPoint.rows; rr++)
	{
		for(int cc = 0; cc < mPoint.cols; cc++)
		{
			const Vec3f& point = mPoint.at<Vec3f>(rr, cc);
			const Vec3b& color = mColor.at<Vec3b>(rr, cc);
			if(!_finite(point[0])) continue;
			oss << "v " << point[0] << ", " << point[1] << ", " << point[2] << ", ";
			oss << static_cast<float>(color[2]) / 255.0f << ", ";
			oss << static_cast<float>(color[1]) / 255.0f << ", ";
			oss << static_cast<float>(color[0]) / 255.0f << endl;
		}
	}
  ofstream ofs(filename.c_str(), ios_base::out);
  ofs << oss.str();
}

void Save(const Mat& mPoint, const Mat& mColor, const Mat& mDepth, const Mat& mInfrared, string& dispString, const string& path)
{
	const string namePoint = "point";
	const string nameDepth = "depth";
	const string nameColor = "color";
  const string nameInfrared = "infrared";
	const string extPoint = ".obj";
	const string extImage = ".jpg";
	const string extPointData = ".yml";
	
	if(modeSaveAccumulated || modeSaveSelect)
	{
		// generate fmt
		char buff_fmt[1024];
		char buff_str[1024];
		const size_t size = paramAccColors.size();
		//const size_t digit = static_cast<size_t>(log10l(static_cast<double>(size))) + 1;
    //sprintf_s(buff_fmt, "%%s/%%s_%%0%dd%%s", digit);
    sprintf_s(buff_fmt, "%%s/%%s_%%0%dd%%s", 3);
		
		cout << size << " data will be saved." << endl;

		for(size_t ii = 0; ii < size; ii++)
		{
			sprintf_s(buff_str, buff_fmt, path.c_str(), namePoint.c_str(), ii, extPoint.c_str());
			Save_Point(buff_str, paramAccPoints[ii], paramAccColors[ii]);
			sprintf_s(buff_str, buff_fmt, path.c_str(), nameColor.c_str(), ii, extImage.c_str());
			Save_Image(buff_str, paramAccColors[ii]);
			sprintf_s(buff_str, buff_fmt, path.c_str(), nameDepth.c_str(), ii, extPointData.c_str());
			Save_Depth(buff_str, paramAccPoints[ii], nameDepth);
      cout << ii << " th frame was saved." << endl;
		}
		
		if(modeSaveAccumulated) Toggle_SaveAccumulated(dispString);
		else if(modeSaveSelect) Toggle_SaveSelect(dispString);
	}

	else
	{
		const string filenamePoint = string(path).append("/").append(namePoint).append(extPoint);
		Save_Point(filenamePoint, mPoint, mColor);

		const string filenameColor = string(path).append("/").append(nameColor).append(extImage);
    const string filenameDepth = string(path).append("/").append(nameDepth).append(extImage);
		const string filenamePointData = string(path).append("/").append(nameDepth).append(extPointData);
    const string filenameInfrared = string(path).append("/").append(nameInfrared).append(extImage);
		
		flip(mColor, mColor,1);
    Save_Image(filenameColor, mColor);
    Save_Image(filenameDepth, mDepth);
    Save_Image(filenameInfrared, mInfrared);

		Save_Depth(filenamePointData, mPoint, nameDepth);
	}
	cout << "all data were saved." << endl;
}