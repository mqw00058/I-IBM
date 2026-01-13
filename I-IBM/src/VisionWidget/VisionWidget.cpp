#include "VisionWidget.h"


VisionWidget::VisionWidget(QWidget *parent) : GLWidget(parent)
{
	/*if (argc < 4)
	{
		std::cout << "Usage: " << argv[0]<< " <inputPicturePath> <list.txt> <list_focal.txt>"<<std::endl;
		return -1;
	}

	std::string inputPath           = argv[1];
	std::string outputListPath      = argv[2];
	std::string outputFocalListPath = argv[3];
	*/
/*
	std::string inputPath = "F:/I-IBM2/I-IBM/src/Bundler/examples/test/";
	std::string outputListPath  = "list.txt";
	std::string outputFocalListPath = "list_focal.txt";
	BundlerFocalExtractor extractor(inputPath, outputListPath, outputFocalListPath);
	*/
}


VisionWidget::~VisionWidget(void)
{
}

void VisionWidget::paintGL()
{
	drawBundleImages();
}

void VisionWidget::drawBundleImages()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawWorkspace();
}