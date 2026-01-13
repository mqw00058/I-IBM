#ifndef LOGDOCKWIDGET_H
#define LOGDOCKWIDGET_H

#include <QDockwidget>
#include <QtGui>


class LogDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit LogDockWidget(QDockWidget *dock = 0);

private:
	QDockWidget *logdock;





signals:

	public slots :

};

#endif // LOGDOCKWIDGET_H
