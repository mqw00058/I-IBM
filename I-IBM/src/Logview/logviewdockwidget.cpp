#include "logviewdockwidget.h"
 #include <QScrollBar>


LogviewDockWidget::LogviewDockWidget(QDockWidget *parent) : logdock(parent)
{
    textLog = new QTextEdit;
	sb =    textLog->verticalScrollBar();	//vertical scrollbar 생성
	sb->setValue(sb->maximum());			//vertical scrollbar textLog 커서의 maximum에 위치
    textLog->setReadOnly(true);
   // textLog->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
   textLog->setMinimumHeight(logdock->parentWidget()->height()/3);
}


void LogviewDockWidget::initialText()
{
    textLog->insertPlainText(tr("I-IBM by In Yeop Jang (2012.1)\n"));
    for(int i=0; i< textLog->size().width()/10; i++)
        textLog->insertPlainText("=");
    textLog->insertPlainText("\n\n");
}

void LogviewDockWidget::createLogWidget()
{
    logdock->setFeatures( QDockWidget::DockWidgetClosable |
                          QDockWidget::DockWidgetMovable |
                          QDockWidget::DockWidgetFloatable);


    //logdock->setStyle(QApplication::style());
    logdock->setWidget(textLog);
    logdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

//logdock->resize(100,10);
logdock->adjustSize();

   // connect(logdock,SIGNAL(dockLocationChanged(Qt::DockWidgetArea )),this,SLOT(moved()));

}


void LogviewDockWidget::addText(const QString str)
{
    textLog->insertPlainText(str);
    textLog->update();
	sb->setValue(sb->maximum());	//vertical scrollbar textLog 커서의 maximum에 위치
}

void LogviewDockWidget::moved()
{
    textLog = new QTextEdit;
    textLog->setReadOnly(true);
    //textLog->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //textLog->setMinimumSize(100,3);
    textLog->insertPlainText("Log view\n");
    for(int i=0; i<logdock->width()/6.5; i++)
        textLog->insertPlainText("=");
    textLog->insertPlainText("\n");

    textLog->update();

    textLog->updateGeometry();
}

