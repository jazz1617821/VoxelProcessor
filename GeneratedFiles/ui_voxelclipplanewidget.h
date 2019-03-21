/********************************************************************************
** Form generated from reading UI file 'voxelclipplanewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELCLIPPLANEWIDGET_H
#define UI_VOXELCLIPPLANEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "titlebar.hpp"

QT_BEGIN_NAMESPACE

class Ui_VoxelClipPlaneWidget
{
public:
    QGridLayout *gridLayout;
    QSpinBox *spb_invZClip;
    QSlider *hsld_yClip;
    QSlider *hsld_invYClip;
    QSpinBox *spb_zClip;
    QSpinBox *spb_invXClip;
    QSlider *hsld_xClip;
    QSlider *hsld_zClip;
    QSpinBox *spb_xClip;
    QSpinBox *spb_yClip;
    QSlider *hsld_invZClip;
    QSpinBox *spb_invYClip;
    QSlider *hsld_invXClip;
    QCheckBox *ckb_xClip;
    QLabel *lb_voxel;
    QComboBox *cbb_voxel;
    QLabel *lb_xClip;
    QLabel *lb_yClip;
    QLabel *lb_zClip;
    QCheckBox *ckb_invXClip;
    QCheckBox *ckb_yClip;
    QCheckBox *ckb_invYClip;
    QCheckBox *ckb_zClip;
    QCheckBox *ckb_invZClip;
    TitleBar *titleBar;
    QSpacerItem *vspr_bottom;

    void setupUi(QWidget *VoxelClipPlaneWidget)
    {
        if (VoxelClipPlaneWidget->objectName().isEmpty())
            VoxelClipPlaneWidget->setObjectName(QStringLiteral("VoxelClipPlaneWidget"));
        VoxelClipPlaneWidget->resize(400, 297);
        gridLayout = new QGridLayout(VoxelClipPlaneWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 0, 10, 15);
        spb_invZClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_invZClip->setObjectName(QStringLiteral("spb_invZClip"));
        spb_invZClip->setMaximum(30000);

        gridLayout->addWidget(spb_invZClip, 12, 2, 1, 1);

        hsld_yClip = new QSlider(VoxelClipPlaneWidget);
        hsld_yClip->setObjectName(QStringLiteral("hsld_yClip"));
        hsld_yClip->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(hsld_yClip, 8, 1, 1, 1);

        hsld_invYClip = new QSlider(VoxelClipPlaneWidget);
        hsld_invYClip->setObjectName(QStringLiteral("hsld_invYClip"));
        hsld_invYClip->setOrientation(Qt::Horizontal);
        hsld_invYClip->setInvertedAppearance(false);

        gridLayout->addWidget(hsld_invYClip, 9, 1, 1, 1);

        spb_zClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_zClip->setObjectName(QStringLiteral("spb_zClip"));
        spb_zClip->setMaximum(30000);

        gridLayout->addWidget(spb_zClip, 11, 2, 1, 1);

        spb_invXClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_invXClip->setObjectName(QStringLiteral("spb_invXClip"));
        spb_invXClip->setMaximum(30000);

        gridLayout->addWidget(spb_invXClip, 6, 2, 1, 1);

        hsld_xClip = new QSlider(VoxelClipPlaneWidget);
        hsld_xClip->setObjectName(QStringLiteral("hsld_xClip"));
        hsld_xClip->setOrientation(Qt::Horizontal);
        hsld_xClip->setInvertedAppearance(false);
        hsld_xClip->setInvertedControls(false);
        hsld_xClip->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(hsld_xClip, 5, 1, 1, 1);

        hsld_zClip = new QSlider(VoxelClipPlaneWidget);
        hsld_zClip->setObjectName(QStringLiteral("hsld_zClip"));
        hsld_zClip->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(hsld_zClip, 11, 1, 1, 1);

        spb_xClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_xClip->setObjectName(QStringLiteral("spb_xClip"));
        spb_xClip->setMaximum(30000);
        spb_xClip->setValue(0);

        gridLayout->addWidget(spb_xClip, 5, 2, 1, 1);

        spb_yClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_yClip->setObjectName(QStringLiteral("spb_yClip"));
        spb_yClip->setMaximum(30000);

        gridLayout->addWidget(spb_yClip, 8, 2, 1, 1);

        hsld_invZClip = new QSlider(VoxelClipPlaneWidget);
        hsld_invZClip->setObjectName(QStringLiteral("hsld_invZClip"));
        hsld_invZClip->setOrientation(Qt::Horizontal);
        hsld_invZClip->setInvertedAppearance(false);

        gridLayout->addWidget(hsld_invZClip, 12, 1, 1, 1);

        spb_invYClip = new QSpinBox(VoxelClipPlaneWidget);
        spb_invYClip->setObjectName(QStringLiteral("spb_invYClip"));
        spb_invYClip->setMaximum(30000);

        gridLayout->addWidget(spb_invYClip, 9, 2, 1, 1);

        hsld_invXClip = new QSlider(VoxelClipPlaneWidget);
        hsld_invXClip->setObjectName(QStringLiteral("hsld_invXClip"));
        hsld_invXClip->setOrientation(Qt::Horizontal);
        hsld_invXClip->setInvertedAppearance(false);
        hsld_invXClip->setInvertedControls(false);

        gridLayout->addWidget(hsld_invXClip, 6, 1, 1, 1);

        ckb_xClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_xClip->setObjectName(QStringLiteral("ckb_xClip"));
        ckb_xClip->setChecked(false);

        gridLayout->addWidget(ckb_xClip, 5, 0, 1, 1);

        lb_voxel = new QLabel(VoxelClipPlaneWidget);
        lb_voxel->setObjectName(QStringLiteral("lb_voxel"));

        gridLayout->addWidget(lb_voxel, 1, 0, 1, 2);

        cbb_voxel = new QComboBox(VoxelClipPlaneWidget);
        cbb_voxel->setObjectName(QStringLiteral("cbb_voxel"));

        gridLayout->addWidget(cbb_voxel, 2, 0, 1, 3);

        lb_xClip = new QLabel(VoxelClipPlaneWidget);
        lb_xClip->setObjectName(QStringLiteral("lb_xClip"));

        gridLayout->addWidget(lb_xClip, 3, 0, 1, 2);

        lb_yClip = new QLabel(VoxelClipPlaneWidget);
        lb_yClip->setObjectName(QStringLiteral("lb_yClip"));

        gridLayout->addWidget(lb_yClip, 7, 0, 1, 2);

        lb_zClip = new QLabel(VoxelClipPlaneWidget);
        lb_zClip->setObjectName(QStringLiteral("lb_zClip"));

        gridLayout->addWidget(lb_zClip, 10, 0, 1, 2);

        ckb_invXClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_invXClip->setObjectName(QStringLiteral("ckb_invXClip"));

        gridLayout->addWidget(ckb_invXClip, 6, 0, 1, 1);

        ckb_yClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_yClip->setObjectName(QStringLiteral("ckb_yClip"));

        gridLayout->addWidget(ckb_yClip, 8, 0, 1, 1);

        ckb_invYClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_invYClip->setObjectName(QStringLiteral("ckb_invYClip"));

        gridLayout->addWidget(ckb_invYClip, 9, 0, 1, 1);

        ckb_zClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_zClip->setObjectName(QStringLiteral("ckb_zClip"));

        gridLayout->addWidget(ckb_zClip, 11, 0, 1, 1);

        ckb_invZClip = new QCheckBox(VoxelClipPlaneWidget);
        ckb_invZClip->setObjectName(QStringLiteral("ckb_invZClip"));

        gridLayout->addWidget(ckb_invZClip, 12, 0, 1, 1);

        titleBar = new TitleBar(VoxelClipPlaneWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));

        gridLayout->addWidget(titleBar, 0, 0, 1, 3);

        vspr_bottom = new QSpacerItem(20, 219, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(vspr_bottom, 13, 0, 1, 3);


        retranslateUi(VoxelClipPlaneWidget);

        QMetaObject::connectSlotsByName(VoxelClipPlaneWidget);
    } // setupUi

    void retranslateUi(QWidget *VoxelClipPlaneWidget)
    {
        VoxelClipPlaneWidget->setWindowTitle(QApplication::translate("VoxelClipPlaneWidget", "VoxelClipPlaneWidget", nullptr));
        ckb_xClip->setText(QString());
        lb_voxel->setText(QApplication::translate("VoxelClipPlaneWidget", "Voxel:", nullptr));
        lb_xClip->setText(QApplication::translate("VoxelClipPlaneWidget", "X-Clip:", nullptr));
        lb_yClip->setText(QApplication::translate("VoxelClipPlaneWidget", "Y-Clip:", nullptr));
        lb_zClip->setText(QApplication::translate("VoxelClipPlaneWidget", "Z-Clip:", nullptr));
        ckb_invXClip->setText(QString());
        ckb_yClip->setText(QString());
        ckb_invYClip->setText(QString());
        ckb_zClip->setText(QString());
        ckb_invZClip->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VoxelClipPlaneWidget: public Ui_VoxelClipPlaneWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELCLIPPLANEWIDGET_H
