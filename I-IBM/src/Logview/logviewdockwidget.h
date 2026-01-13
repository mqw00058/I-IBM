#ifndef LOGVIEWDOCKWIDGET_H
#define LOGVIEWDOCKWIDGET_H


#include <QDockWidget>
#include <QTextEdit>


class LogviewDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    LogviewDockWidget(QDockWidget *parent = 0);
    
	void createLogWidget();
    void initialText();
    void addText(const QString str);

    void moved();

private:

    QTextEdit *textLog;		  // Log widget --> main variable
	QScrollBar *sb;			//scrollbar 
    QDockWidget *logdock;


signals:

public slots:
    
};

#endif // LOGVIEWDOCKWIDGET_H
