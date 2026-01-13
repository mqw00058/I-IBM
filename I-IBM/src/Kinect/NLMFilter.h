#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void nonlocalMeansFilter(Mat& src, Mat& dest, int templeteWindowSize, int searchWindowSize, double h, double sigma=0.0);
// 16u version of non local means filter
void nonlocalMeansFilter16U(Mat& src, Mat& dest, int templeteWindowSize, int searchWindowSize, double h, double sigma=0.0);
