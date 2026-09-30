#ifndef BUNDLEROPTIONWIDET_CPP
#define BUNDLEROPTIONWIDET_CPP

#include "BundlerOptionWidget.h"

template <class T>
BundlerOptionWidgetT<T>::BundlerOptionWidgetT(T* Ptr) : visionView(Ptr)
{
	QGroupBox *BundlerProcessGroup = new QGroupBox(tr("Model View Options"),visionView);
	QGridLayout *BundlerProcessLayout = new QGridLayout(BundlerProcessGroup);

	CreateBundlerButtons(BundlerProcessLayout);
			
	BundlerProcessGroup->setLayout(BundlerProcessLayout);
	

	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	QGroupBox *RenderSceneOptionGroup = new QGroupBox(tr("Render Scene Options"));
		QVBoxLayout *RenderSceneOptionLayout = new QVBoxLayout(RenderSceneOptionGroup);
			//	CreateRenderSceneOptions(RenderSceneOptionLayout);
		//RenderSceneOptionGroup->setLayout(RenderSceneOptionLayout);
	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á

QVBoxLayout *mainLayout = new QVBoxLayout;
mainLayout->addWidget(BundlerProcessGroup);
mainLayout->addWidget(RenderSceneOptionGroup);
mainLayout->addSpacing(12);
mainLayout->addStretch(1);

setLayout(mainLayout);
}

template <class T>
BundlerOptionWidgetT<T>::~BundlerOptionWidgetT() 
{
}

//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
template <class T>
inline void BundlerOptionWidgetT<T>::browse()
{
	QString directory = QFileDialog::getExistingDirectory(this,
			tr("Find Files"), QDir::currentPath());

		if (!directory.isEmpty()) {
			if (directoryComboBox->findText(directory) == -1)
				directoryComboBox->addItem(directory);
			directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
		}

}

template <class T>
inline void BundlerOptionWidgetT<T>::bundlerFocalExtraction()
{ 
	QString path = directoryComboBox->currentText();
	/*QMessageBox t;
	t.setText(path);
	t.exec();
	*/
	if(path.right(0).compare(QString('/'))!=0)
	{
			path.append(QString("/"));
			qDebug() << path.right(0);
	}
	
	std::string inputPath           = path.toUtf8().constData();
	//std::string inputPath = "F:/Users/jiy/Desktop/I-IBM2/I-IBM/src/Bundler/examples/test/";
	std::string outputListPath      = "list.txt";
	std::string outputFocalListPath = "list_focal.txt";

	BundlerFocalExtractor extractor(inputPath, outputListPath, outputFocalListPath);
}

//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á



template <class T>
void BundlerOptionWidgetT<T>::CreateBundlerButtons(QGridLayout  * BundlerProcessLayout)
{
	BundlerProcessLayout->setMargin(0);
	directoryComboBox = new QComboBox;
	directoryComboBox->setEditable(true);
	directoryComboBox->addItem(QDir::currentPath());
	directoryComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QPushButton *browseButton = new QPushButton("&Browse");

	BundlerProcessLayout->addWidget(directoryComboBox,0,0);
	BundlerProcessLayout->addWidget(browseButton,0,1);

	QPushButton *BundlerFocalButton = new QPushButton("Bundler &Focal Extractor");
	QProgressBar *progressBundlerFocal = new QProgressBar;
	progressBundlerFocal->setRange(0,100);
	//progressBundlerFocal->setMinimumWidth(300);
	BundlerProcessLayout->addWidget(BundlerFocalButton,1,0);
	BundlerProcessLayout->addWidget(progressBundlerFocal,1,1);
	//¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á¡á
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(BundlerFocalButton, SIGNAL(clicked()), this, SLOT(bundlerFocalExtraction()));

}

#endif 