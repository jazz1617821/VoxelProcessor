/********************************************************************************
** Form generated from reading UI file 'templatewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPLATEWIDGET_H
#define UI_TEMPLATEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TemplateWidget
{
public:
    QGridLayout *gridLayout;
    QScrollArea *sa_viewer;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QGroupBox *emptySettingBox;
    QGridLayout *gridLayout_4;
    QRadioButton *rb_empty;
    QGroupBox *cylinderSettingBox;
    QGridLayout *gridLayout_7;
    QRadioButton *rb_cylinder;
    QGroupBox *basicSettingBox;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *lb_voxelDataName;
    QLineEdit *le_voxelDataName;
    QVBoxLayout *verticalLayout_2;
    QLabel *lb_voxelSize;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lb_voxelSize_x;
    QLineEdit *le_voxelSize_x;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lb_voxelSize_y;
    QLineEdit *le_voxelSize_y;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lb_voxelSize_z;
    QLineEdit *le_voxelSize_z;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QGroupBox *ballSettingBox;
    QGridLayout *gridLayout_5;
    QVBoxLayout *verticalLayout;
    QRadioButton *rb_ball;
    QLabel *lb_radius;
    QLineEdit *le_radius;
    QCheckBox *cb_ballHollow;
    QLabel *lb_ballThinkness;
    QLineEdit *le_ballThinkness;
    QGroupBox *cubeSettingBox;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *rb_cube;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lb_length;
    QLineEdit *le_length;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lb_width;
    QLineEdit *le_width;
    QHBoxLayout *horizontalLayout_7;
    QLabel *lb_height;
    QLineEdit *le_height;
    QCheckBox *cb_cubeHollow;
    QLabel *lb_cubeThinkness;
    QLineEdit *le_cubeThinkness;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_ok;

    void setupUi(QWidget *TemplateWidget)
    {
        if (TemplateWidget->objectName().isEmpty())
            TemplateWidget->setObjectName(QStringLiteral("TemplateWidget"));
        TemplateWidget->resize(424, 517);
        gridLayout = new QGridLayout(TemplateWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        sa_viewer = new QScrollArea(TemplateWidget);
        sa_viewer->setObjectName(QStringLiteral("sa_viewer"));
        sa_viewer->setMinimumSize(QSize(300, 0));
        sa_viewer->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 387, 595));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        emptySettingBox = new QGroupBox(scrollAreaWidgetContents);
        emptySettingBox->setObjectName(QStringLiteral("emptySettingBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(emptySettingBox->sizePolicy().hasHeightForWidth());
        emptySettingBox->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(emptySettingBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        rb_empty = new QRadioButton(emptySettingBox);
        rb_empty->setObjectName(QStringLiteral("rb_empty"));
        rb_empty->setChecked(true);

        gridLayout_4->addWidget(rb_empty, 0, 0, 1, 1);


        gridLayout_2->addWidget(emptySettingBox, 1, 0, 1, 1);

        cylinderSettingBox = new QGroupBox(scrollAreaWidgetContents);
        cylinderSettingBox->setObjectName(QStringLiteral("cylinderSettingBox"));
        sizePolicy.setHeightForWidth(cylinderSettingBox->sizePolicy().hasHeightForWidth());
        cylinderSettingBox->setSizePolicy(sizePolicy);
        gridLayout_7 = new QGridLayout(cylinderSettingBox);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        rb_cylinder = new QRadioButton(cylinderSettingBox);
        rb_cylinder->setObjectName(QStringLiteral("rb_cylinder"));

        gridLayout_7->addWidget(rb_cylinder, 0, 0, 1, 1);


        gridLayout_2->addWidget(cylinderSettingBox, 4, 0, 1, 1);

        basicSettingBox = new QGroupBox(scrollAreaWidgetContents);
        basicSettingBox->setObjectName(QStringLiteral("basicSettingBox"));
        sizePolicy.setHeightForWidth(basicSettingBox->sizePolicy().hasHeightForWidth());
        basicSettingBox->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(basicSettingBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lb_voxelDataName = new QLabel(basicSettingBox);
        lb_voxelDataName->setObjectName(QStringLiteral("lb_voxelDataName"));

        verticalLayout_3->addWidget(lb_voxelDataName);

        le_voxelDataName = new QLineEdit(basicSettingBox);
        le_voxelDataName->setObjectName(QStringLiteral("le_voxelDataName"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(le_voxelDataName->sizePolicy().hasHeightForWidth());
        le_voxelDataName->setSizePolicy(sizePolicy1);
        le_voxelDataName->setMaxLength(10);

        verticalLayout_3->addWidget(le_voxelDataName);


        gridLayout_3->addLayout(verticalLayout_3, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lb_voxelSize = new QLabel(basicSettingBox);
        lb_voxelSize->setObjectName(QStringLiteral("lb_voxelSize"));

        verticalLayout_2->addWidget(lb_voxelSize);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lb_voxelSize_x = new QLabel(basicSettingBox);
        lb_voxelSize_x->setObjectName(QStringLiteral("lb_voxelSize_x"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lb_voxelSize_x->sizePolicy().hasHeightForWidth());
        lb_voxelSize_x->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(lb_voxelSize_x);

        le_voxelSize_x = new QLineEdit(basicSettingBox);
        le_voxelSize_x->setObjectName(QStringLiteral("le_voxelSize_x"));
        sizePolicy1.setHeightForWidth(le_voxelSize_x->sizePolicy().hasHeightForWidth());
        le_voxelSize_x->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(le_voxelSize_x);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lb_voxelSize_y = new QLabel(basicSettingBox);
        lb_voxelSize_y->setObjectName(QStringLiteral("lb_voxelSize_y"));
        sizePolicy2.setHeightForWidth(lb_voxelSize_y->sizePolicy().hasHeightForWidth());
        lb_voxelSize_y->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(lb_voxelSize_y);

        le_voxelSize_y = new QLineEdit(basicSettingBox);
        le_voxelSize_y->setObjectName(QStringLiteral("le_voxelSize_y"));
        sizePolicy1.setHeightForWidth(le_voxelSize_y->sizePolicy().hasHeightForWidth());
        le_voxelSize_y->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(le_voxelSize_y);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        lb_voxelSize_z = new QLabel(basicSettingBox);
        lb_voxelSize_z->setObjectName(QStringLiteral("lb_voxelSize_z"));
        sizePolicy2.setHeightForWidth(lb_voxelSize_z->sizePolicy().hasHeightForWidth());
        lb_voxelSize_z->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(lb_voxelSize_z);

        le_voxelSize_z = new QLineEdit(basicSettingBox);
        le_voxelSize_z->setObjectName(QStringLiteral("le_voxelSize_z"));
        sizePolicy1.setHeightForWidth(le_voxelSize_z->sizePolicy().hasHeightForWidth());
        le_voxelSize_z->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(le_voxelSize_z);


        verticalLayout_2->addLayout(horizontalLayout_4);


        gridLayout_3->addLayout(verticalLayout_2, 0, 1, 2, 1);

        verticalSpacer_2 = new QSpacerItem(128, 27, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 1, 0, 1, 1);


        gridLayout_2->addWidget(basicSettingBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 5, 0, 1, 1);

        ballSettingBox = new QGroupBox(scrollAreaWidgetContents);
        ballSettingBox->setObjectName(QStringLiteral("ballSettingBox"));
        sizePolicy.setHeightForWidth(ballSettingBox->sizePolicy().hasHeightForWidth());
        ballSettingBox->setSizePolicy(sizePolicy);
        gridLayout_5 = new QGridLayout(ballSettingBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        rb_ball = new QRadioButton(ballSettingBox);
        rb_ball->setObjectName(QStringLiteral("rb_ball"));

        verticalLayout->addWidget(rb_ball);

        lb_radius = new QLabel(ballSettingBox);
        lb_radius->setObjectName(QStringLiteral("lb_radius"));

        verticalLayout->addWidget(lb_radius);

        le_radius = new QLineEdit(ballSettingBox);
        le_radius->setObjectName(QStringLiteral("le_radius"));
        le_radius->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_radius->sizePolicy().hasHeightForWidth());
        le_radius->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(le_radius);

        cb_ballHollow = new QCheckBox(ballSettingBox);
        cb_ballHollow->setObjectName(QStringLiteral("cb_ballHollow"));
        cb_ballHollow->setEnabled(false);

        verticalLayout->addWidget(cb_ballHollow);

        lb_ballThinkness = new QLabel(ballSettingBox);
        lb_ballThinkness->setObjectName(QStringLiteral("lb_ballThinkness"));

        verticalLayout->addWidget(lb_ballThinkness);

        le_ballThinkness = new QLineEdit(ballSettingBox);
        le_ballThinkness->setObjectName(QStringLiteral("le_ballThinkness"));
        le_ballThinkness->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_ballThinkness->sizePolicy().hasHeightForWidth());
        le_ballThinkness->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(le_ballThinkness);


        gridLayout_5->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(ballSettingBox, 2, 0, 1, 1);

        cubeSettingBox = new QGroupBox(scrollAreaWidgetContents);
        cubeSettingBox->setObjectName(QStringLiteral("cubeSettingBox"));
        sizePolicy.setHeightForWidth(cubeSettingBox->sizePolicy().hasHeightForWidth());
        cubeSettingBox->setSizePolicy(sizePolicy);
        gridLayout_6 = new QGridLayout(cubeSettingBox);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        rb_cube = new QRadioButton(cubeSettingBox);
        rb_cube->setObjectName(QStringLiteral("rb_cube"));

        verticalLayout_4->addWidget(rb_cube);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        lb_length = new QLabel(cubeSettingBox);
        lb_length->setObjectName(QStringLiteral("lb_length"));
        sizePolicy2.setHeightForWidth(lb_length->sizePolicy().hasHeightForWidth());
        lb_length->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(lb_length);

        le_length = new QLineEdit(cubeSettingBox);
        le_length->setObjectName(QStringLiteral("le_length"));
        le_length->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_length->sizePolicy().hasHeightForWidth());
        le_length->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(le_length);


        horizontalLayout_8->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        lb_width = new QLabel(cubeSettingBox);
        lb_width->setObjectName(QStringLiteral("lb_width"));
        sizePolicy2.setHeightForWidth(lb_width->sizePolicy().hasHeightForWidth());
        lb_width->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(lb_width);

        le_width = new QLineEdit(cubeSettingBox);
        le_width->setObjectName(QStringLiteral("le_width"));
        le_width->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_width->sizePolicy().hasHeightForWidth());
        le_width->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(le_width);


        horizontalLayout_8->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        lb_height = new QLabel(cubeSettingBox);
        lb_height->setObjectName(QStringLiteral("lb_height"));
        sizePolicy2.setHeightForWidth(lb_height->sizePolicy().hasHeightForWidth());
        lb_height->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(lb_height);

        le_height = new QLineEdit(cubeSettingBox);
        le_height->setObjectName(QStringLiteral("le_height"));
        le_height->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_height->sizePolicy().hasHeightForWidth());
        le_height->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(le_height);


        horizontalLayout_8->addLayout(horizontalLayout_7);


        verticalLayout_4->addLayout(horizontalLayout_8);

        cb_cubeHollow = new QCheckBox(cubeSettingBox);
        cb_cubeHollow->setObjectName(QStringLiteral("cb_cubeHollow"));
        cb_cubeHollow->setEnabled(false);

        verticalLayout_4->addWidget(cb_cubeHollow);

        lb_cubeThinkness = new QLabel(cubeSettingBox);
        lb_cubeThinkness->setObjectName(QStringLiteral("lb_cubeThinkness"));

        verticalLayout_4->addWidget(lb_cubeThinkness);

        le_cubeThinkness = new QLineEdit(cubeSettingBox);
        le_cubeThinkness->setObjectName(QStringLiteral("le_cubeThinkness"));
        le_cubeThinkness->setEnabled(false);
        sizePolicy1.setHeightForWidth(le_cubeThinkness->sizePolicy().hasHeightForWidth());
        le_cubeThinkness->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(le_cubeThinkness);


        gridLayout_6->addLayout(verticalLayout_4, 0, 0, 1, 1);


        gridLayout_2->addWidget(cubeSettingBox, 3, 0, 1, 1);

        sa_viewer->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(sa_viewer, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pb_ok = new QPushButton(TemplateWidget);
        pb_ok->setObjectName(QStringLiteral("pb_ok"));

        horizontalLayout->addWidget(pb_ok);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(TemplateWidget);

        QMetaObject::connectSlotsByName(TemplateWidget);
    } // setupUi

    void retranslateUi(QWidget *TemplateWidget)
    {
        TemplateWidget->setWindowTitle(QApplication::translate("TemplateWidget", "TemplateWidget", nullptr));
        emptySettingBox->setTitle(QApplication::translate("TemplateWidget", "Empty", nullptr));
        rb_empty->setText(QApplication::translate("TemplateWidget", "Create Empty", nullptr));
        cylinderSettingBox->setTitle(QApplication::translate("TemplateWidget", "Cylinder", nullptr));
        rb_cylinder->setText(QApplication::translate("TemplateWidget", "Create Cylinder", nullptr));
        basicSettingBox->setTitle(QApplication::translate("TemplateWidget", "Basic Setting", nullptr));
        lb_voxelDataName->setText(QApplication::translate("TemplateWidget", "Voxel Data Name", nullptr));
        le_voxelDataName->setText(QString());
        lb_voxelSize->setText(QApplication::translate("TemplateWidget", "Voxel Size", nullptr));
        lb_voxelSize_x->setText(QApplication::translate("TemplateWidget", "X:", nullptr));
        le_voxelSize_x->setText(QApplication::translate("TemplateWidget", "1.0", nullptr));
        lb_voxelSize_y->setText(QApplication::translate("TemplateWidget", "Y:", nullptr));
        le_voxelSize_y->setText(QApplication::translate("TemplateWidget", "1.0", nullptr));
        lb_voxelSize_z->setText(QApplication::translate("TemplateWidget", "Z:", nullptr));
        le_voxelSize_z->setText(QApplication::translate("TemplateWidget", "1.0", nullptr));
        ballSettingBox->setTitle(QApplication::translate("TemplateWidget", "Ball", nullptr));
        rb_ball->setText(QApplication::translate("TemplateWidget", "Create Sphere", nullptr));
        lb_radius->setText(QApplication::translate("TemplateWidget", "Radius", nullptr));
        le_radius->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        cb_ballHollow->setText(QApplication::translate("TemplateWidget", "Hollow", nullptr));
        lb_ballThinkness->setText(QApplication::translate("TemplateWidget", "Thickness", nullptr));
        le_ballThinkness->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        cubeSettingBox->setTitle(QApplication::translate("TemplateWidget", "Cube", nullptr));
        rb_cube->setText(QApplication::translate("TemplateWidget", "Create Cube", nullptr));
        lb_length->setText(QApplication::translate("TemplateWidget", "Length:", nullptr));
        le_length->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        lb_width->setText(QApplication::translate("TemplateWidget", "Width:", nullptr));
        le_width->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        lb_height->setText(QApplication::translate("TemplateWidget", "Height:", nullptr));
        le_height->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        cb_cubeHollow->setText(QApplication::translate("TemplateWidget", "Hollow", nullptr));
        lb_cubeThinkness->setText(QApplication::translate("TemplateWidget", "Thickness", nullptr));
        le_cubeThinkness->setText(QApplication::translate("TemplateWidget", "1", nullptr));
        pb_ok->setText(QApplication::translate("TemplateWidget", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TemplateWidget: public Ui_TemplateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEMPLATEWIDGET_H
