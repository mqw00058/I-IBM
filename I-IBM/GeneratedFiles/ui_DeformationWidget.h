/********************************************************************************
** Form generated from reading UI file 'DeformationWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFORMATIONWIDGET_H
#define UI_DEFORMATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeformationWidget
{
public:
    QFormLayout *formLayout;
    QHBoxLayout *horizontalLayout0;
    QPushButton *pushButton_OpenXMLs;
    QHBoxLayout *horizontalLayout;
    QPushButton *deformMove;
    QPushButton *deformSelButton;
    QPushButton *deformGraphButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *correspondenceButton;
    QPushButton *doDeformButton;
    QCheckBox *check_drawgraph;
    QPushButton *pushButton_SaveDeformedPointCloud;

    void setupUi(QWidget *DeformationWidget)
    {
        if (DeformationWidget->objectName().isEmpty())
            DeformationWidget->setObjectName(QStringLiteral("DeformationWidget"));
        DeformationWidget->resize(263, 300);
        formLayout = new QFormLayout(DeformationWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(0);
        formLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout0 = new QHBoxLayout();
        horizontalLayout0->setSpacing(6);
        horizontalLayout0->setObjectName(QStringLiteral("horizontalLayout0"));
        pushButton_OpenXMLs = new QPushButton(DeformationWidget);
        pushButton_OpenXMLs->setObjectName(QStringLiteral("pushButton_OpenXMLs"));

        horizontalLayout0->addWidget(pushButton_OpenXMLs);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout0);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        deformMove = new QPushButton(DeformationWidget);
        deformMove->setObjectName(QStringLiteral("deformMove"));
        deformMove->setCheckable(true);

        horizontalLayout->addWidget(deformMove);

        deformSelButton = new QPushButton(DeformationWidget);
        deformSelButton->setObjectName(QStringLiteral("deformSelButton"));
        deformSelButton->setCheckable(true);

        horizontalLayout->addWidget(deformSelButton);

        deformGraphButton = new QPushButton(DeformationWidget);
        deformGraphButton->setObjectName(QStringLiteral("deformGraphButton"));
        deformGraphButton->setCheckable(true);
        deformGraphButton->setChecked(false);
        deformGraphButton->setFlat(false);

        horizontalLayout->addWidget(deformGraphButton);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        correspondenceButton = new QPushButton(DeformationWidget);
        correspondenceButton->setObjectName(QStringLiteral("correspondenceButton"));
        correspondenceButton->setMaximumSize(QSize(130, 16777215));
        correspondenceButton->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(correspondenceButton);

        doDeformButton = new QPushButton(DeformationWidget);
        doDeformButton->setObjectName(QStringLiteral("doDeformButton"));
        doDeformButton->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_2->addWidget(doDeformButton);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_2);

        check_drawgraph = new QCheckBox(DeformationWidget);
        check_drawgraph->setObjectName(QStringLiteral("check_drawgraph"));

        formLayout->setWidget(5, QFormLayout::FieldRole, check_drawgraph);

        pushButton_SaveDeformedPointCloud = new QPushButton(DeformationWidget);
        pushButton_SaveDeformedPointCloud->setObjectName(QStringLiteral("pushButton_SaveDeformedPointCloud"));

        formLayout->setWidget(6, QFormLayout::FieldRole, pushButton_SaveDeformedPointCloud);


        retranslateUi(DeformationWidget);
        QObject::connect(deformGraphButton, SIGNAL(clicked()), DeformationWidget, SLOT(gen_graph()));
        QObject::connect(deformSelButton, SIGNAL(clicked()), DeformationWidget, SLOT(mouse_pick()));
        QObject::connect(deformMove, SIGNAL(clicked()), DeformationWidget, SLOT(mouse_deform()));
        QObject::connect(correspondenceButton, SIGNAL(clicked()), DeformationWidget, SLOT(slot_correspondence()));
        QObject::connect(doDeformButton, SIGNAL(clicked()), DeformationWidget, SLOT(slot_dodeform()));
        QObject::connect(check_drawgraph, SIGNAL(toggled(bool)), DeformationWidget, SLOT(ONOFF_DrawGraph()));
        QObject::connect(pushButton_SaveDeformedPointCloud, SIGNAL(clicked()), DeformationWidget, SLOT(slot_SaveDeformedPointCloud()));
        QObject::connect(pushButton_OpenXMLs, SIGNAL(clicked()), DeformationWidget, SLOT(slot_OpenXMLDir()));

        QMetaObject::connectSlotsByName(DeformationWidget);
    } // setupUi

    void retranslateUi(QWidget *DeformationWidget)
    {
        DeformationWidget->setWindowTitle(QApplication::translate("DeformationWidget", "DeformationWidget", 0));
        pushButton_OpenXMLs->setText(QApplication::translate("DeformationWidget", "Open XML Directory", 0));
        deformMove->setText(QApplication::translate("DeformationWidget", "Deform", 0));
        deformSelButton->setText(QApplication::translate("DeformationWidget", "Selection", 0));
        deformGraphButton->setText(QApplication::translate("DeformationWidget", "Graph", 0));
        correspondenceButton->setText(QApplication::translate("DeformationWidget", "Correspondence", 0));
        doDeformButton->setText(QApplication::translate("DeformationWidget", "Do Deformation", 0));
        check_drawgraph->setText(QApplication::translate("DeformationWidget", "Show Graph", 0));
        pushButton_SaveDeformedPointCloud->setText(QApplication::translate("DeformationWidget", "Save Deformed Point Cloud", 0));
    } // retranslateUi

};

namespace Ui {
    class DeformationWidget: public Ui_DeformationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFORMATIONWIDGET_H
