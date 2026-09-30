#pragma once
#include <string>
#include <opencv2/core/core.hpp>
//#include <src/Geometry3D/Geometry3D.h>
struct CORRESPONDENCE
{
	int p, q;
	float p_2dpoint[2];
	float q_2dpoint[2];
};
struct IMG_POINTLIST
{
	cv::Mat *img;
	std::vector<std::pair<cv::Point2i,cv::Point2i>> pair_list;
};
typedef std::vector<CORRESPONDENCE> VECCORR;
class CorrespondenceOpenCV
{
public:
	
	cv::Mat*vmap1, *vmap2;
	CorrespondenceOpenCV(cv::Mat*vmap1, cv::Mat*vmap2);
	VECCORR ExtractPairs(std::string srcfile, std::string dstfile);

	cv::Mat dst;
	cv::Mat img1, img2;
~CorrespondenceOpenCV();
};

