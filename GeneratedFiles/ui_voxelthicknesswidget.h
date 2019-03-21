/********************************************************************************
** Form generated from reading UI file 'voxelthicknesswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELTHICKNESSWIDGET_H
#define UI_VOXELTHICKNESSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "titlebar.hpp"

QT_BEGIN_NAMESPACE

class Ui_VoxelThicknessWidget
{
public:
    QVBoxLayout *verticalLayout;
    TitleBar *titleBar;
    QGroupBox *gb_df;
    QGridLayout *gridLayout;
    QLabel *lb_dfValues;
    QComboBox *cbb_voxel;
    QLineEdit *le_dfMaxValue;
    QLabel *lb_dfMaxValue;
    QLabel *lb_voxel;
    QLineEdit *le_dfMinValue;
    QLineEdit *le_dfType;
    QLabel *lb_dfMinValue;
    QLabel *lb_dfType;
    QGroupBox *gb_thickness;
    QGridLayout *gridLayout_2;
    QLineEdit *le_innerThickness;
    QLabel *lb_innerThickness;
    QLabel *lb_outerThickness;
    QLineEdit *le_outerThickness;
    QSpacerItem *vspr_bottom;
    QDialogButtonBox *btmbox_widget;

    void setupUi(QWidget *VoxelThicknessWidget)
    {
        if (VoxelThicknessWidget->objectName().isEmpty())
            VoxelThicknessWidget->setObjectName(QStringLiteral("VoxelThicknessWidget"));
        VoxelThicknessWidget->resize(292, 324);
        verticalLayout = new QVBoxLayout(VoxelThicknessWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(10, 0, 10, 15);
        titleBar = new TitleBar(VoxelThicknessWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));

        verticalLayout->addWidget(titleBar);

        gb_df = new QGroupBox(VoxelThicknessWidget);
        gb_df->setObjectName(QStringLiteral("gb_df"));
        gridLayout = new QGridLayout(gb_df);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        lb_dfValues = new QLabel(gb_df);
        lb_dfValues->setObjectName(QStringLiteral("lb_dfValues"));

        gridLayout->addWidget(lb_dfValues, 4, 0, 1, 5);

        cbb_voxel = new QComboBox(gb_df);
        cbb_voxel->setObjectName(QStringLiteral("cbb_voxel"));

        gridLayout->addWidget(cbb_voxel, 1, 0, 1, 5);

        le_dfMaxValue = new QLineEdit(gb_df);
        le_dfMaxValue->setObjectName(QStringLiteral("le_dfMaxValue"));

        gridLayout->addWidget(le_dfMaxValue, 5, 1, 1, 1);

        lb_dfMaxValue = new QLabel(gb_df);
        lb_dfMaxValue->setObjectName(QStringLiteral("lb_dfMaxValue"));

        gridLayout->addWidget(lb_dfMaxValue, 5, 0, 1, 1);

        lb_voxel = new QLabel(gb_df);
        lb_voxel->setObjectName(QStringLiteral("lb_voxel"));

        gridLayout->addWidget(lb_voxel, 0, 0, 1, 5);

        le_dfMinValue = new QLineEdit(gb_df);
        le_dfMinValue->setObjectName(QStringLiteral("le_dfMinValue"));

        gridLayout->addWidget(le_dfMinValue, 5, 3, 1, 1);

        le_dfType = new QLineEdit(gb_df);
        le_dfType->setObjectName(QStringLiteral("le_dfType"));

        gridLayout->addWidget(le_dfType, 3, 0, 1, 5);

        lb_dfMinValue = new QLabel(gb_df);
        lb_dfMinValue->setObjectName(QStringLiteral("lb_dfMinValue"));

        gridLayout->addWidget(lb_dfMinValue, 5, 2, 1, 1);

        lb_dfType = new QLabel(gb_df);
        lb_dfType->setObjectName(QStringLiteral("lb_dfType"));

        gridLayout->addWidget(lb_dfType, 2, 0, 1, 5);


        verticalLayout->addWidget(gb_df);

        gb_thickness = new QGroupBox(VoxelThicknessWidget);
        gb_thickness->setObjectName(QStringLiteral("gb_thickness"));
        gridLayout_2 = new QGridLayout(gb_thickness);
        gridLayout_2->setSpacing(5);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        le_innerThickness = new QLineEdit(gb_thickness);
        le_innerThickness->setObjectName(QStringLiteral("le_innerThickness"));

        gridLayout_2->addWidget(le_innerThickness, 1, 0, 1, 1);

        lb_innerThickness = new QLabel(gb_thickness);
        lb_innerThickness->setObjectName(QStringLiteral("lb_innerThickness"));

        gridLayout_2->addWidget(lb_innerThickness, 0, 0, 1, 1);

        lb_outerThickness = new QLabel(gb_thickness);
        lb_outerThickness->setObjectName(QStringLiteral("lb_outerThickness"));

        gridLayout_2->addWidget(lb_outerThickness, 2, 0, 1, 1);

        le_outerThickness = new QLineEdit(gb_thickness);
        le_outerThickness->setObjectName(QStringLiteral("le_outerThickness"));

        gridLayout_2->addWidget(le_outerThickness, 3, 0, 1, 1);


        verticalLayout->addWidget(gb_thickness);

        vspr_bottom = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(vspr_bottom);

        btmbox_widget = new QDialogButtonBox(VoxelThicknessWidget);
        btmbox_widget->setObjectName(QStringLiteral("btmbox_widget"));
        btmbox_widget->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(btmbox_widget);


        retranslateUi(VoxelThicknessWidget);

        QMetaObject::connectSlotsByName(VoxelThicknessWidget);
    } // setupUi

    void retranslateUi(QWidget *VoxelThicknessWidget)
    {
        VoxelThicknessWidget->setWindowTitle(QApplication::translate("VoxelThicknessWidget", "VoxelThicknessWidget", nullptr));
        gb_df->setTitle(QApplication::translate("VoxelThicknessWidget", "Distance Field", nullptr));
        lb_dfValues->setText(QApplication::translate("VoxelThicknessWidget", "DF Values:", nullptr));
        lb_dfMaxValue->setText(QApplication::translate("VoxelThicknessWidget", "Max:", nullptr));
        lb_voxel->setText(QApplication::translate("VoxelThicknessWidget", "Voxel:", nullptr));
        lb_dfMinValue->setText(QApplication::translate("VoxelThicknessWidget", "Min:", nullptr));
        lb_dfType->setText(QApplication::translate("VoxelThicknessWidget", "DF Type:", nullptr));
        gb_thickness->setTitle(QApplication::translate("VoxelThicknessWidget", "Thickness", nullptr));
        lb_innerThickness->setText(QApplication::translate("VoxelThicknessWidget", "Inner Thickness:", nullptr));
        lb_outerThickness->setText(QApplication::translate("VoxelThicknessWidget", "Outer Thickness:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VoxelThicknessWidget: public Ui_VoxelThicknessWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELTHICKNESSWIDGET_H
