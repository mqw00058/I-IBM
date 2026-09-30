#ifndef GLOPTIONWIDGET_H
#define GLOPTIONWIDGET_H

#include "../../mainwindow.h"
#include <QPushButton>
#include <QProgressBar>
#include <QGridLayout>
#include <QtGui>
#include <QString>

class GLOptionWidget : public QWidget
{
	Q_OBJECT
public:

	GLOptionWidget(MainWindow* parent =0 );
	~GLOptionWidget();

public slots:

	void ONOFF_DrawBoundingBox(int modelnumber);
	void ONOFF_DrawVertex(int modelnumber);
	void ONOFF_DrawEdge(int modelnumber);
	void ONOFF_DrawFace(int modelnumber);
	void ONOFF_DrawNormal(int modelnumber);

	void ONOFF_SmoothFlatMode(bool onoff);
	void OnOFF_TextureMode(bool onoff);
	void ONOFF_Centering(bool onff);
	void ONOFF_SelectPointsMode(bool onff);
	void ONOFF_SelectFacesMode(bool onff);
	void RemoveSelectedElements();

	void ONOFF_DrawWorkspace(bool onff);
	void ONOFF_DrawAxis(bool onff);
	void select_currentModel();

public:
	void CreateModelListView(QWidget *);
	QListWidget *listWidget;
	void CreateModelListItem(int modelnumber, QString modelname);

	void CreateModelViewOptions(QWidget *);
	void CreateModelSelectionOptions(QWidget*);
	void CreateRenderSceneOptions(QLayout* RenderSceneOptionLayout);
	MainWindow* mainwindow;
	GLWidget *glView;

protected:
	void keyPressEvent(QKeyEvent * event);

private:
	

};
#endif // GLOPTIONWIDGET_H
