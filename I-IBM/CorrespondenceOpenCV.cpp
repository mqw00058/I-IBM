
#include "../../globals.h"
#include "CorrespondenceOpenCV.h"
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>
#include <QtGui>
#include <QFileDialog>
#include <QString>

int pcnt = 1;
cv::Point2i p, q;
int width = 0;
bool pcheck = false;
bool qcheck = false;
cv::Mat temp;
cv::Mat img;
void CallBackFunc1(int event, int x, int y, int flags, void* ptr)
{
	IMG_POINTLIST* plist = (IMG_POINTLIST*)ptr;


	if (event == cv::EVENT_LBUTTONUP && flags == (cv::EVENT_FLAG_CTRLKEY))
	{	std::ostringstream text;
			int fontFace = cv::FONT_ITALIC;
			double fontScale = 1;

		if (x < width)
		{
			p.x = x;
			p.y = y;

			pcheck = true;

		
			cv::circle(img, p, 6, CV_RGB(255, 0, 0), 1, CV_FILLED);


		}
		else
		{
			q.x = x - (width - 1);
			q.y = y;

			qcheck = true;

			cv::circle(img, cv::Point2i(q.x+width-1,q.y), 6, CV_RGB(255, 0, 0), 1, CV_FILLED);
		}

	

		if (pcheck && qcheck)		// list에 없으면
		{
		//	cv::Point2i p, q;

		//	cv::Scalar color = cv::Scalar(pcnt % 256, (pcnt >> 8) % 256, (pcnt >> 16) % 256);
		//	cv::line(img, p, cv::Point2i(q.x + width - 1, q.y), color, 1, CV_AA);


			plist->pair_list.push_back(std::make_pair(p, q));
			logviewdockwidget->addText(QString("Left mouse button is clicked while pressing CTRL key - %1 , %2 -- ").arg(p.x).arg(p.y));
			logviewdockwidget->addText(QString("Left mouse button is clicked while pressing CTRL key - %1 , %2 \n").arg(q.x).arg(q.y));
			pcheck = false;
			qcheck = false;
		}



	}


	if (event == cv::EVENT_LBUTTONUP && flags == (cv::EVENT_FLAG_SHIFTKEY))
	{
		img = plist->img->clone();
		for (int i = 0; i < plist->pair_list.size(); i++)
		{
			if (plist->pair_list[i].first.x - 10 < x && x < plist->pair_list[i].first.x + 10 && plist->pair_list[i].first.y - 10 < y && y < plist->pair_list[i].first.y + 10)
				plist->pair_list.erase(plist->pair_list.begin() + i);
		}

	}


	if (plist->pair_list.size() >0)
	{

		for (int i = 0; i < plist->pair_list.size(); i++)
		{
			cv::Point2i p, q;
			p = plist->pair_list[i].first;
			q = plist->pair_list[i].second;

			std::ostringstream text;
			int fontFace = cv::FONT_ITALIC;
			double fontScale = 1;
			cv::circle(img, p, 6, CV_RGB(255, 0, 0), 1, CV_FILLED);			
			text << i << ":(" << p.x << "," << p.y << ")";
			cv::putText(img, text.str(), p, fontFace, fontScale, CV_RGB(0, 0, 255));

			cv::circle(img, cv::Point2i(q.x + width - 1, q.y), 6, CV_RGB(255, 0, 0), 1, CV_FILLED);
			text << i << ":(" << q.x << "," << q.y << ")";

			
			cv::putText(img, text.str(), cv::Point2i(q.x + width - 1, q.y), fontFace, fontScale, CV_RGB(0, 0, 255));
			cv::Scalar color = cv::Scalar(i % 256, (i >> 8) % 256, (i >> 16) % 256);
			cv::line(img, p, cv::Point2i(q.x + width - 1, q.y), color, 1, CV_AA);

		}
	
	}

	cv::imshow("Source Window", img);
}

/*

void CallBackFunc2(int event, int x, int y, int flags, void* ptr)
{

if (event == cv::EVENT_LBUTTONUP && flags == (cv::EVENT_FLAG_CTRLKEY ))
{
IMG_POINTLIST* plist = (IMG_POINTLIST*)ptr;
cv::Point2i p;
p.x = x;
p.y = y;
cv::Mat *img = plist->img;


cv::circle(*img, cv::Point(x, y), 6, CV_RGB(255, 0, 0), 1, CV_FILLED);
std::ostringstream text;
int fontFace = cv::FONT_ITALIC;
double fontScale = 1;
text << qcnt << ":(" << x << "," << y << ")";
cv::putText(*img, text.str(), p, fontFace, fontScale, CV_RGB(0, 0, 255));


std::vector<cv::Point2i>::iterator it = find(plist->_list->begin(), plist->_list->end(), p);
if (it == plist->_list->end())		// list에 없으면
{
plist->_list->push_back(p);

logviewdockwidget->addText(QString("Left mouse button is clicked while pressing CTRL key - %1 , %2 \n").arg(p.x).arg(p.y));
cv::imshow("Source Window", *img);
}
qcnt++;
}
/ *
else if (flags == (cv::EVENT_FLAG_RBUTTON + cv::EVENT_FLAG_SHIFTKEY))
{
cout << "Right mouse button is clicked while pressing SHIFT key - position (" << x << ", " << y << ")" << endl;
}
else if (event == cv::EVENT_MOUSEMOVE && flags == cv::EVENT_FLAG_ALTKEY)
{
cout << "Mouse is moved over the window while pressing ALT key - position (" << x << ", " << y << ")" << endl;
}
* /

//	cv::waitKey(0);
}*/
CorrespondenceOpenCV::CorrespondenceOpenCV(cv::Mat*vmap1 = NULL, cv::Mat*vmap2 = NULL)
{
	this->vmap1 = vmap1;
	this->vmap2 = vmap2;
	pcnt = 1;

}

VECCORR CorrespondenceOpenCV::ExtractPairs(std::string srcfile, std::string dstfile)
{
	cv::Point2i p;
	// Read image from file 

	std::vector<cv::Point2i> src_list, dst_list;
	img1 = cv::imread(srcfile);

	width = img1.cols;
	img2 = cv::imread(dstfile);

	std::vector <CORRESPONDENCE> corr;

	//if fail to read the image
	if (img1.empty() || img2.empty())
	{
		QMessageBox::information(0, "Load Image", "Error loading the image", QMessageBox::Close);
		
		return corr;
	}


	//Create a window
	cv::namedWindow("Source Window", 1);

	cv::hconcat(img1, img2, img1); //두 이미지를 img1으로 합침
	IMG_POINTLIST ip1, ip2;
	ip1.img = &img1;
	img = img1.clone();
	src_list.clear();
	ip1.pair_list.clear();


	//show the image
	cv::imshow("Source Window", img1);

	//set the callback function for any mouse event
	cv::setMouseCallback("Source Window", CallBackFunc1, &ip1);



	// Wait until user press some key
	cv::waitKey(0);


	CORRESPONDENCE c;

	if (vmap1->cols > 0 && vmap2->cols > 0)
	{
		for (int i = 0; i < ip1.pair_list.size(); i++)
		{

			int vertxindex = vmap1->at< int >(ip1.pair_list[i].first.y, ip1.pair_list[i].first.x);
			int vertxindex1 = vmap2->at< int >(ip1.pair_list[i].second.y, ip1.pair_list[i].second.x);

			c.p = vertxindex;
			c.q = vertxindex1;
			c.p_2dpoint[0] = ip1.pair_list[i].first.x; c.p_2dpoint[1] = ip1.pair_list[i].first.y;
			c.q_2dpoint[0] = ip1.pair_list[i].second.x; c.q_2dpoint[1] = ip1.pair_list[i].second.y;
			corr.push_back(c);
		}
	}

	return corr;
}

CorrespondenceOpenCV::~CorrespondenceOpenCV()
{
}

