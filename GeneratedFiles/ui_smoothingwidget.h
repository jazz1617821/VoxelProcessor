/********************************************************************************
** Form generated from reading UI file 'smoothingwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMOOTHINGWIDGET_H
#define UI_SMOOTHINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "titlebar.hpp"

QT_BEGIN_NAMESPACE

class Ui_SmoothingWidget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *basicGB;
    QGridLayout *gridLayout_2;
    QLabel *lb_erosion;
    QLabel *lb_dilation;
    QPushButton *pb_erosion;
    QPushButton *pb_dilation;
    TitleBar *titleBar;
    QGroupBox *advanceGB;
    QGridLayout *gridLayout_3;
    QLabel *lb_opening;
    QLabel *lb_erosion_2;
    QSpinBox *sb_openingErosionTimes;
    QLabel *lb_dilation_2;
    QSpinBox *sb_openingDilationTimes;
    QPushButton *pb_opening;
    QLabel *lb_closiong;
    QLabel *lb_dilation_3;
    QSpinBox *sb_closingDilationTimes;
    QLabel *lb_erosion_3;
    QSpinBox *sb_closingErosionTimes;
    QPushButton *pb_closing;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_close;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QRadioButton *rb_six_neighbor;
    QRadioButton *rb_twenty_six_neighbor;

    void setupUi(QWidget *SmoothingWidget)
    {
        if (SmoothingWidget->objectName().isEmpty())
            SmoothingWidget->setObjectName(QStringLiteral("SmoothingWidget"));
        SmoothingWidget->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SmoothingWidget->sizePolicy().hasHeightForWidth());
        SmoothingWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(SmoothingWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 0, 10, 15);
        basicGB = new QGroupBox(SmoothingWidget);
        basicGB->setObjectName(QStringLiteral("basicGB"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(basicGB->sizePolicy().hasHeightForWidth());
        basicGB->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(basicGB);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lb_erosion = new QLabel(basicGB);
        lb_erosion->setObjectName(QStringLiteral("lb_erosion"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lb_erosion->sizePolicy().hasHeightForWidth());
        lb_erosion->setSizePolicy(sizePolicy2);
        lb_erosion->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lb_erosion, 0, 0, 1, 1);

        lb_dilation = new QLabel(basicGB);
        lb_dilation->setObjectName(QStringLiteral("lb_dilation"));
        sizePolicy2.setHeightForWidth(lb_dilation->sizePolicy().hasHeightForWidth());
        lb_dilation->setSizePolicy(sizePolicy2);
        lb_dilation->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lb_dilation, 0, 1, 1, 1);

        pb_erosion = new QPushButton(basicGB);
        pb_erosion->setObjectName(QStringLiteral("pb_erosion"));

        gridLayout_2->addWidget(pb_erosion, 1, 0, 1, 1);

        pb_dilation = new QPushButton(basicGB);
        pb_dilation->setObjectName(QStringLiteral("pb_dilation"));

        gridLayout_2->addWidget(pb_dilation, 1, 1, 1, 1);


        gridLayout->addWidget(basicGB, 2, 0, 1, 1);

        titleBar = new TitleBar(SmoothingWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(titleBar->sizePolicy().hasHeightForWidth());
        titleBar->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(titleBar, 0, 0, 1, 1);

        advanceGB = new QGroupBox(SmoothingWidget);
        advanceGB->setObjectName(QStringLiteral("advanceGB"));
        sizePolicy1.setHeightForWidth(advanceGB->sizePolicy().hasHeightForWidth());
        advanceGB->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(advanceGB);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lb_opening = new QLabel(advanceGB);
        lb_opening->setObjectName(QStringLiteral("lb_opening"));
        lb_opening->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_opening, 0, 0, 1, 1);

        lb_erosion_2 = new QLabel(advanceGB);
        lb_erosion_2->setObjectName(QStringLiteral("lb_erosion_2"));
        sizePolicy.setHeightForWidth(lb_erosion_2->sizePolicy().hasHeightForWidth());
        lb_erosion_2->setSizePolicy(sizePolicy);
        lb_erosion_2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_erosion_2, 0, 1, 1, 1);

        sb_openingErosionTimes = new QSpinBox(advanceGB);
        sb_openingErosionTimes->setObjectName(QStringLiteral("sb_openingErosionTimes"));

        gridLayout_3->addWidget(sb_openingErosionTimes, 0, 2, 1, 1);

        lb_dilation_2 = new QLabel(advanceGB);
        lb_dilation_2->setObjectName(QStringLiteral("lb_dilation_2"));
        sizePolicy.setHeightForWidth(lb_dilation_2->sizePolicy().hasHeightForWidth());
        lb_dilation_2->setSizePolicy(sizePolicy);
        lb_dilation_2->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_dilation_2, 0, 3, 1, 1);

        sb_openingDilationTimes = new QSpinBox(advanceGB);
        sb_openingDilationTimes->setObjectName(QStringLiteral("sb_openingDilationTimes"));

        gridLayout_3->addWidget(sb_openingDilationTimes, 0, 4, 1, 1);

        pb_opening = new QPushButton(advanceGB);
        pb_opening->setObjectName(QStringLiteral("pb_opening"));

        gridLayout_3->addWidget(pb_opening, 0, 5, 1, 1);

        lb_closiong = new QLabel(advanceGB);
        lb_closiong->setObjectName(QStringLiteral("lb_closiong"));
        lb_closiong->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_closiong, 1, 0, 1, 1);

        lb_dilation_3 = new QLabel(advanceGB);
        lb_dilation_3->setObjectName(QStringLiteral("lb_dilation_3"));
        sizePolicy.setHeightForWidth(lb_dilation_3->sizePolicy().hasHeightForWidth());
        lb_dilation_3->setSizePolicy(sizePolicy);
        lb_dilation_3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_dilation_3, 1, 1, 1, 1);

        sb_closingDilationTimes = new QSpinBox(advanceGB);
        sb_closingDilationTimes->setObjectName(QStringLiteral("sb_closingDilationTimes"));

        gridLayout_3->addWidget(sb_closingDilationTimes, 1, 2, 1, 1);

        lb_erosion_3 = new QLabel(advanceGB);
        lb_erosion_3->setObjectName(QStringLiteral("lb_erosion_3"));
        sizePolicy.setHeightForWidth(lb_erosion_3->sizePolicy().hasHeightForWidth());
        lb_erosion_3->setSizePolicy(sizePolicy);
        lb_erosion_3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_erosion_3, 1, 3, 1, 1);

        sb_closingErosionTimes = new QSpinBox(advanceGB);
        sb_closingErosionTimes->setObjectName(QStringLiteral("sb_closingErosionTimes"));

        gridLayout_3->addWidget(sb_closingErosionTimes, 1, 4, 1, 1);

        pb_closing = new QPushButton(advanceGB);
        pb_closing->setObjectName(QStringLiteral("pb_closing"));

        gridLayout_3->addWidget(pb_closing, 1, 5, 1, 1);


        gridLayout->addWidget(advanceGB, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pb_close = new QPushButton(SmoothingWidget);
        pb_close->setObjectName(QStringLiteral("pb_close"));

        horizontalLayout->addWidget(pb_close);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 1);

        groupBox = new QGroupBox(SmoothingWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        rb_six_neighbor = new QRadioButton(groupBox);
        rb_six_neighbor->setObjectName(QStringLiteral("rb_six_neighbor"));
        rb_six_neighbor->setChecked(true);

        gridLayout_4->addWidget(rb_six_neighbor, 0, 0, 1, 1);

        rb_twenty_six_neighbor = new QRadioButton(groupBox);
        rb_twenty_six_neighbor->setObjectName(QStringLiteral("rb_twenty_six_neighbor"));

        gridLayout_4->addWidget(rb_twenty_six_neighbor, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);


        retranslateUi(SmoothingWidget);
        QObject::connect(pb_close, SIGNAL(clicked()), SmoothingWidget, SLOT(close()));

        QMetaObject::connectSlotsByName(SmoothingWidget);
    } // setupUi

    void retranslateUi(QWidget *SmoothingWidget)
    {
        SmoothingWidget->setWindowTitle(QApplication::translate("SmoothingWidget", "SmoothingWidget", nullptr));
        basicGB->setTitle(QApplication::translate("SmoothingWidget", "Basic", nullptr));
        lb_erosion->setText(QApplication::translate("SmoothingWidget", "Erosion", nullptr));
        lb_dilation->setText(QApplication::translate("SmoothingWidget", "Dilation", nullptr));
        pb_erosion->setText(QApplication::translate("SmoothingWidget", "Go", nullptr));
        pb_dilation->setText(QApplication::translate("SmoothingWidget", "Go", nullptr));
        advanceGB->setTitle(QApplication::translate("SmoothingWidget", "Advanced", nullptr));
        lb_opening->setText(QApplication::translate("SmoothingWidget", "Opening", nullptr));
        lb_erosion_2->setText(QApplication::translate("SmoothingWidget", "Erosion", nullptr));
        lb_dilation_2->setText(QApplication::translate("SmoothingWidget", "Dilation", nullptr));
        pb_opening->setText(QApplication::translate("SmoothingWidget", "Go", nullptr));
        lb_closiong->setText(QApplication::translate("SmoothingWidget", "Closing", nullptr));
        lb_dilation_3->setText(QApplication::translate("SmoothingWidget", "Dilation", nullptr));
        lb_erosion_3->setText(QApplication::translate("SmoothingWidget", "Erosion", nullptr));
        pb_closing->setText(QApplication::translate("SmoothingWidget", "Go", nullptr));
        pb_close->setText(QApplication::translate("SmoothingWidget", "Close", nullptr));
        groupBox->setTitle(QApplication::translate("SmoothingWidget", "Mode", nullptr));
        rb_six_neighbor->setText(QApplication::translate("SmoothingWidget", "6 Neighbor", nullptr));
        rb_twenty_six_neighbor->setText(QApplication::translate("SmoothingWidget", "26 Neighbor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SmoothingWidget: public Ui_SmoothingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMOOTHINGWIDGET_H
