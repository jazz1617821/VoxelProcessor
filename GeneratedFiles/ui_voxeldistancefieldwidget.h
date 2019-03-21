/********************************************************************************
** Form generated from reading UI file 'voxeldistancefieldwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELDISTANCEFIELDWIDGET_H
#define UI_VOXELDISTANCEFIELDWIDGET_H

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

class Ui_VoxelDistanceFieldWidget
{
public:
    QVBoxLayout *verticalLayout;
    TitleBar *titleBar;
    QGroupBox *gb_voxel;
    QGridLayout *gridLayout;
    QLabel *lb_voxelSizeX;
    QLabel *lb_voxelResolutionX;
    QLabel *lb_voxelSizeZ;
    QLabel *lb_voxelResolution;
    QLabel *lb_voxelResolutionZ;
    QComboBox *cbb_voxel;
    QLineEdit *le_voxelSizeZ;
    QLineEdit *le_voxelResolutionY;
    QLineEdit *le_voxelResolutionZ;
    QLabel *lb_voxel;
    QLineEdit *le_voxelSizeY;
    QLineEdit *le_voxelResolutionX;
    QLabel *lb_voxelSize;
    QLabel *lb_voxelSIzeY;
    QLineEdit *le_voxelSizeX;
    QLabel *lb_voxelResolutionY;
    QGroupBox *gb_df;
    QGridLayout *gridLayout_2;
    QLineEdit *le_dfBoundFront;
    QLabel *lb_dfBoundBottom;
    QLabel *lb_dfBoundFront;
    QLineEdit *le_dfResolutionX;
    QLineEdit *le_dfResolutionY;
    QLineEdit *le_dfBoundBottom;
    QLabel *lb_dfResolutionY;
    QLabel *lb_dfResolution;
    QLineEdit *le_dfBoundTop;
    QLabel *lb_dfBound;
    QLabel *lb_dfType;
    QComboBox *cbb_df;
    QLineEdit *le_dfResolutionZ;
    QLabel *lb_dfResolutionZ;
    QLabel *lb_dfResolutionX;
    QLabel *lb_dfBoundTop;
    QLabel *lb_dfBoundBack;
    QLineEdit *le_dfBoundBack;
    QLabel *lb_dfBoundLeft;
    QLineEdit *le_dfBoundLeft;
    QLabel *lb_dfBoundRight;
    QLineEdit *le_dfBoundRight;
    QSpacerItem *vspr_bottom;
    QDialogButtonBox *btmbox_widget;

    void setupUi(QWidget *VoxelDistanceFieldWidget)
    {
        if (VoxelDistanceFieldWidget->objectName().isEmpty())
            VoxelDistanceFieldWidget->setObjectName(QStringLiteral("VoxelDistanceFieldWidget"));
        VoxelDistanceFieldWidget->resize(356, 391);
        verticalLayout = new QVBoxLayout(VoxelDistanceFieldWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(10, 0, 10, 15);
        titleBar = new TitleBar(VoxelDistanceFieldWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));

        verticalLayout->addWidget(titleBar);

        gb_voxel = new QGroupBox(VoxelDistanceFieldWidget);
        gb_voxel->setObjectName(QStringLiteral("gb_voxel"));
        gridLayout = new QGridLayout(gb_voxel);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        lb_voxelSizeX = new QLabel(gb_voxel);
        lb_voxelSizeX->setObjectName(QStringLiteral("lb_voxelSizeX"));

        gridLayout->addWidget(lb_voxelSizeX, 5, 0, 1, 1);

        lb_voxelResolutionX = new QLabel(gb_voxel);
        lb_voxelResolutionX->setObjectName(QStringLiteral("lb_voxelResolutionX"));

        gridLayout->addWidget(lb_voxelResolutionX, 3, 0, 1, 1);

        lb_voxelSizeZ = new QLabel(gb_voxel);
        lb_voxelSizeZ->setObjectName(QStringLiteral("lb_voxelSizeZ"));

        gridLayout->addWidget(lb_voxelSizeZ, 5, 4, 1, 1);

        lb_voxelResolution = new QLabel(gb_voxel);
        lb_voxelResolution->setObjectName(QStringLiteral("lb_voxelResolution"));

        gridLayout->addWidget(lb_voxelResolution, 2, 0, 1, 6);

        lb_voxelResolutionZ = new QLabel(gb_voxel);
        lb_voxelResolutionZ->setObjectName(QStringLiteral("lb_voxelResolutionZ"));

        gridLayout->addWidget(lb_voxelResolutionZ, 3, 4, 1, 1);

        cbb_voxel = new QComboBox(gb_voxel);
        cbb_voxel->setObjectName(QStringLiteral("cbb_voxel"));

        gridLayout->addWidget(cbb_voxel, 1, 0, 1, 6);

        le_voxelSizeZ = new QLineEdit(gb_voxel);
        le_voxelSizeZ->setObjectName(QStringLiteral("le_voxelSizeZ"));

        gridLayout->addWidget(le_voxelSizeZ, 5, 5, 1, 1);

        le_voxelResolutionY = new QLineEdit(gb_voxel);
        le_voxelResolutionY->setObjectName(QStringLiteral("le_voxelResolutionY"));

        gridLayout->addWidget(le_voxelResolutionY, 3, 3, 1, 1);

        le_voxelResolutionZ = new QLineEdit(gb_voxel);
        le_voxelResolutionZ->setObjectName(QStringLiteral("le_voxelResolutionZ"));

        gridLayout->addWidget(le_voxelResolutionZ, 3, 5, 1, 1);

        lb_voxel = new QLabel(gb_voxel);
        lb_voxel->setObjectName(QStringLiteral("lb_voxel"));

        gridLayout->addWidget(lb_voxel, 0, 0, 1, 6);

        le_voxelSizeY = new QLineEdit(gb_voxel);
        le_voxelSizeY->setObjectName(QStringLiteral("le_voxelSizeY"));

        gridLayout->addWidget(le_voxelSizeY, 5, 3, 1, 1);

        le_voxelResolutionX = new QLineEdit(gb_voxel);
        le_voxelResolutionX->setObjectName(QStringLiteral("le_voxelResolutionX"));

        gridLayout->addWidget(le_voxelResolutionX, 3, 1, 1, 1);

        lb_voxelSize = new QLabel(gb_voxel);
        lb_voxelSize->setObjectName(QStringLiteral("lb_voxelSize"));

        gridLayout->addWidget(lb_voxelSize, 4, 0, 1, 6);

        lb_voxelSIzeY = new QLabel(gb_voxel);
        lb_voxelSIzeY->setObjectName(QStringLiteral("lb_voxelSIzeY"));

        gridLayout->addWidget(lb_voxelSIzeY, 5, 2, 1, 1);

        le_voxelSizeX = new QLineEdit(gb_voxel);
        le_voxelSizeX->setObjectName(QStringLiteral("le_voxelSizeX"));

        gridLayout->addWidget(le_voxelSizeX, 5, 1, 1, 1);

        lb_voxelResolutionY = new QLabel(gb_voxel);
        lb_voxelResolutionY->setObjectName(QStringLiteral("lb_voxelResolutionY"));

        gridLayout->addWidget(lb_voxelResolutionY, 3, 2, 1, 1);

        lb_voxel->raise();
        cbb_voxel->raise();
        lb_voxelResolution->raise();
        lb_voxelResolutionX->raise();
        le_voxelResolutionX->raise();
        lb_voxelResolutionY->raise();
        le_voxelResolutionY->raise();
        lb_voxelResolutionZ->raise();
        le_voxelResolutionZ->raise();
        lb_voxelSize->raise();
        lb_voxelSizeX->raise();
        le_voxelSizeX->raise();
        lb_voxelSIzeY->raise();
        le_voxelSizeY->raise();
        lb_voxelSizeZ->raise();
        le_voxelSizeZ->raise();

        verticalLayout->addWidget(gb_voxel);

        gb_df = new QGroupBox(VoxelDistanceFieldWidget);
        gb_df->setObjectName(QStringLiteral("gb_df"));
        gridLayout_2 = new QGridLayout(gb_df);
        gridLayout_2->setSpacing(5);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        le_dfBoundFront = new QLineEdit(gb_df);
        le_dfBoundFront->setObjectName(QStringLiteral("le_dfBoundFront"));

        gridLayout_2->addWidget(le_dfBoundFront, 5, 5, 1, 1);

        lb_dfBoundBottom = new QLabel(gb_df);
        lb_dfBoundBottom->setObjectName(QStringLiteral("lb_dfBoundBottom"));

        gridLayout_2->addWidget(lb_dfBoundBottom, 5, 2, 1, 1);

        lb_dfBoundFront = new QLabel(gb_df);
        lb_dfBoundFront->setObjectName(QStringLiteral("lb_dfBoundFront"));

        gridLayout_2->addWidget(lb_dfBoundFront, 5, 4, 1, 1);

        le_dfResolutionX = new QLineEdit(gb_df);
        le_dfResolutionX->setObjectName(QStringLiteral("le_dfResolutionX"));

        gridLayout_2->addWidget(le_dfResolutionX, 3, 1, 1, 1);

        le_dfResolutionY = new QLineEdit(gb_df);
        le_dfResolutionY->setObjectName(QStringLiteral("le_dfResolutionY"));

        gridLayout_2->addWidget(le_dfResolutionY, 3, 3, 1, 1);

        le_dfBoundBottom = new QLineEdit(gb_df);
        le_dfBoundBottom->setObjectName(QStringLiteral("le_dfBoundBottom"));

        gridLayout_2->addWidget(le_dfBoundBottom, 5, 3, 1, 1);

        lb_dfResolutionY = new QLabel(gb_df);
        lb_dfResolutionY->setObjectName(QStringLiteral("lb_dfResolutionY"));

        gridLayout_2->addWidget(lb_dfResolutionY, 3, 2, 1, 1);

        lb_dfResolution = new QLabel(gb_df);
        lb_dfResolution->setObjectName(QStringLiteral("lb_dfResolution"));

        gridLayout_2->addWidget(lb_dfResolution, 2, 0, 1, 6);

        le_dfBoundTop = new QLineEdit(gb_df);
        le_dfBoundTop->setObjectName(QStringLiteral("le_dfBoundTop"));

        gridLayout_2->addWidget(le_dfBoundTop, 5, 1, 1, 1);

        lb_dfBound = new QLabel(gb_df);
        lb_dfBound->setObjectName(QStringLiteral("lb_dfBound"));

        gridLayout_2->addWidget(lb_dfBound, 4, 0, 1, 1);

        lb_dfType = new QLabel(gb_df);
        lb_dfType->setObjectName(QStringLiteral("lb_dfType"));

        gridLayout_2->addWidget(lb_dfType, 0, 0, 1, 6);

        cbb_df = new QComboBox(gb_df);
        cbb_df->setObjectName(QStringLiteral("cbb_df"));

        gridLayout_2->addWidget(cbb_df, 1, 0, 1, 6);

        le_dfResolutionZ = new QLineEdit(gb_df);
        le_dfResolutionZ->setObjectName(QStringLiteral("le_dfResolutionZ"));

        gridLayout_2->addWidget(le_dfResolutionZ, 3, 5, 1, 1);

        lb_dfResolutionZ = new QLabel(gb_df);
        lb_dfResolutionZ->setObjectName(QStringLiteral("lb_dfResolutionZ"));

        gridLayout_2->addWidget(lb_dfResolutionZ, 3, 4, 1, 1);

        lb_dfResolutionX = new QLabel(gb_df);
        lb_dfResolutionX->setObjectName(QStringLiteral("lb_dfResolutionX"));

        gridLayout_2->addWidget(lb_dfResolutionX, 3, 0, 1, 1);

        lb_dfBoundTop = new QLabel(gb_df);
        lb_dfBoundTop->setObjectName(QStringLiteral("lb_dfBoundTop"));

        gridLayout_2->addWidget(lb_dfBoundTop, 5, 0, 1, 1);

        lb_dfBoundBack = new QLabel(gb_df);
        lb_dfBoundBack->setObjectName(QStringLiteral("lb_dfBoundBack"));

        gridLayout_2->addWidget(lb_dfBoundBack, 6, 0, 1, 1);

        le_dfBoundBack = new QLineEdit(gb_df);
        le_dfBoundBack->setObjectName(QStringLiteral("le_dfBoundBack"));

        gridLayout_2->addWidget(le_dfBoundBack, 6, 1, 1, 1);

        lb_dfBoundLeft = new QLabel(gb_df);
        lb_dfBoundLeft->setObjectName(QStringLiteral("lb_dfBoundLeft"));

        gridLayout_2->addWidget(lb_dfBoundLeft, 6, 2, 1, 1);

        le_dfBoundLeft = new QLineEdit(gb_df);
        le_dfBoundLeft->setObjectName(QStringLiteral("le_dfBoundLeft"));

        gridLayout_2->addWidget(le_dfBoundLeft, 6, 3, 1, 1);

        lb_dfBoundRight = new QLabel(gb_df);
        lb_dfBoundRight->setObjectName(QStringLiteral("lb_dfBoundRight"));

        gridLayout_2->addWidget(lb_dfBoundRight, 6, 4, 1, 1);

        le_dfBoundRight = new QLineEdit(gb_df);
        le_dfBoundRight->setObjectName(QStringLiteral("le_dfBoundRight"));

        gridLayout_2->addWidget(le_dfBoundRight, 6, 5, 1, 1);


        verticalLayout->addWidget(gb_df);

        vspr_bottom = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(vspr_bottom);

        btmbox_widget = new QDialogButtonBox(VoxelDistanceFieldWidget);
        btmbox_widget->setObjectName(QStringLiteral("btmbox_widget"));
        btmbox_widget->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(btmbox_widget);


        retranslateUi(VoxelDistanceFieldWidget);

        QMetaObject::connectSlotsByName(VoxelDistanceFieldWidget);
    } // setupUi

    void retranslateUi(QWidget *VoxelDistanceFieldWidget)
    {
        VoxelDistanceFieldWidget->setWindowTitle(QApplication::translate("VoxelDistanceFieldWidget", "VoxelDistanceFieldWidget", nullptr));
        gb_voxel->setTitle(QApplication::translate("VoxelDistanceFieldWidget", "Voxel", nullptr));
        lb_voxelSizeX->setText(QApplication::translate("VoxelDistanceFieldWidget", "X:", nullptr));
        lb_voxelResolutionX->setText(QApplication::translate("VoxelDistanceFieldWidget", "X:", nullptr));
        lb_voxelSizeZ->setText(QApplication::translate("VoxelDistanceFieldWidget", "Z:", nullptr));
        lb_voxelResolution->setText(QApplication::translate("VoxelDistanceFieldWidget", "Voxel Resolution:", nullptr));
        lb_voxelResolutionZ->setText(QApplication::translate("VoxelDistanceFieldWidget", "Z:", nullptr));
        lb_voxel->setText(QApplication::translate("VoxelDistanceFieldWidget", "Voxel:", nullptr));
        lb_voxelSize->setText(QApplication::translate("VoxelDistanceFieldWidget", "Voxel Size:", nullptr));
        lb_voxelSIzeY->setText(QApplication::translate("VoxelDistanceFieldWidget", "Y:", nullptr));
        lb_voxelResolutionY->setText(QApplication::translate("VoxelDistanceFieldWidget", "Y:", nullptr));
        gb_df->setTitle(QApplication::translate("VoxelDistanceFieldWidget", "Distance Field", nullptr));
        lb_dfBoundBottom->setText(QApplication::translate("VoxelDistanceFieldWidget", "Bottom:", nullptr));
        lb_dfBoundFront->setText(QApplication::translate("VoxelDistanceFieldWidget", "Front:", nullptr));
        lb_dfResolutionY->setText(QApplication::translate("VoxelDistanceFieldWidget", "Y:", nullptr));
        lb_dfResolution->setText(QApplication::translate("VoxelDistanceFieldWidget", "DF Resolution:", nullptr));
        lb_dfBound->setText(QApplication::translate("VoxelDistanceFieldWidget", "DF Bound:", nullptr));
        lb_dfType->setText(QApplication::translate("VoxelDistanceFieldWidget", "DF Type", nullptr));
        lb_dfResolutionZ->setText(QApplication::translate("VoxelDistanceFieldWidget", "Z:", nullptr));
        lb_dfResolutionX->setText(QApplication::translate("VoxelDistanceFieldWidget", "X:", nullptr));
        lb_dfBoundTop->setText(QApplication::translate("VoxelDistanceFieldWidget", "Top:", nullptr));
        lb_dfBoundBack->setText(QApplication::translate("VoxelDistanceFieldWidget", "Back:", nullptr));
        lb_dfBoundLeft->setText(QApplication::translate("VoxelDistanceFieldWidget", "Left:", nullptr));
        lb_dfBoundRight->setText(QApplication::translate("VoxelDistanceFieldWidget", "Right:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VoxelDistanceFieldWidget: public Ui_VoxelDistanceFieldWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELDISTANCEFIELDWIDGET_H
