/********************************************************************************
** Form generated from reading UI file 'KinectOptionWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECTOPTIONWIDGET_H
#define UI_KINECTOPTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectOptionWidget
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *KinectOptionWidget)
    {
        if (KinectOptionWidget->objectName().isEmpty())
            KinectOptionWidget->setObjectName(QStringLiteral("KinectOptionWidget"));
        KinectOptionWidget->resize(400, 300);
        pushButton = new QPushButton(KinectOptionWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(110, 60, 151, 34));
        pushButton_2 = new QPushButton(KinectOptionWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 120, 151, 34));

        retranslateUi(KinectOptionWidget);
        QObject::connect(pushButton, SIGNAL(clicked()), KinectOptionWidget, SLOT(ConnectKinect()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), KinectOptionWidget, SLOT(StopKinect()));

        QMetaObject::connectSlotsByName(KinectOptionWidget);
    } // setupUi

    void retranslateUi(QWidget *KinectOptionWidget)
    {
        KinectOptionWidget->setWindowTitle(QApplication::translate("KinectOptionWidget", "KinectOptionWidget", 0));
        pushButton->setText(QApplication::translate("KinectOptionWidget", "Connect Kinect", 0));
        pushButton_2->setText(QApplication::translate("KinectOptionWidget", "Stop Kinect", 0));
    } // retranslateUi

};

namespace Ui {
    class KinectOptionWidget: public Ui_KinectOptionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTOPTIONWIDGET_H
