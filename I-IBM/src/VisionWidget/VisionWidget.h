#pragma once
#include "../GLWidget/GLWidget.h"
#include "../Bundler/BundlerFocalExtractor/BundlerFocalExtractor.h"

class VisionWidget :
	public GLWidget
{
public:
	VisionWidget(QWidget *parent=0);
	~VisionWidget(void);

	virtual void paintGL();
	void drawBundleImages();
};

