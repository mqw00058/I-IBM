#ifndef BUNDLEROPTIONWIDGET_H
#define BUNDLEROPTIONWIDGET_H

#include <QPushButton>
#include <QProgressBar>
#include <QGridLayout>
#include <QtGui>
#include <QString>

#include "src/Bundler/BundlerFocalExtractor/BundlerFocalExtractor.h"

class BundlerOptionWidget  : public QWidget
{
	Q_OBJECT
public :
	BundlerOptionWidget(){};	
	~BundlerOptionWidget(){};

public slots:
	virtual inline void browse(){};
	virtual inline void bundlerFocalExtraction(){};
};


template <class T>
class BundlerOptionWidgetT : public BundlerOptionWidget
{

public:
	T*			visionView;

	BundlerOptionWidgetT(T *Ptr);
	~BundlerOptionWidgetT();
		
	void CreateBundlerButtons(QGridLayout *);

public:	//slot functions
	void browse();
	void bundlerFocalExtraction();

private:
	QComboBox *directoryComboBox;
};

//=============================================================================
#if defined(OM_INCLUDE_TEMPLATES) && !defined(BUNDLEROPTIONWIDET_CPP)
#define BUNDLEROPTIONWIDET_TEMPLATES
#include "BundlerOptionWidget.cpp"
#endif
//============================================================================
#endif // GLOPTIONWIDGET_H
