/********************************************************************************
** Form generated from reading UI file 'freeformwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FREEFORMWIDGET_H
#define UI_FREEFORMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "titlebar.hpp"

QT_BEGIN_NAMESPACE

class Ui_FreeFormWidget
{
public:
    QGridLayout *gridLayout;
    TitleBar *titleBar;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *gb_circle;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pb_circleImage;
    QVBoxLayout *verticalLayout_2;
    QLabel *lb_circleRadius;
    QHBoxLayout *horizontalLayout;
    QSpinBox *sb_circleRadius;
    QSlider *sl_circleRadius;
    QGroupBox *gb_square;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pb_squareImage;
    QVBoxLayout *verticalLayout_3;
    QLabel *lb_squareSideLength;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *sb_squareRadius;
    QSlider *sl_squareRadius;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pb_boxSelectionImage;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *btmbox_widget;

    void setupUi(QWidget *FreeFormWidget)
    {
        if (FreeFormWidget->objectName().isEmpty())
            FreeFormWidget->setObjectName(QStringLiteral("FreeFormWidget"));
        FreeFormWidget->resize(262, 408);
        gridLayout = new QGridLayout(FreeFormWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(10, 0, 10, 15);
        titleBar = new TitleBar(FreeFormWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));

        gridLayout->addWidget(titleBar, 0, 0, 1, 1);

        scrollArea = new QScrollArea(FreeFormWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setStyleSheet(QStringLiteral("background-color: rgba(220, 220, 220, 0)"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 240, 346));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gb_circle = new QGroupBox(scrollAreaWidgetContents);
        gb_circle->setObjectName(QStringLiteral("gb_circle"));
        gridLayout_2 = new QGridLayout(gb_circle);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pb_circleImage = new QPushButton(gb_circle);
        pb_circleImage->setObjectName(QStringLiteral("pb_circleImage"));
        pb_circleImage->setStyleSheet(QStringLiteral(""));
        pb_circleImage->setCheckable(true);
        pb_circleImage->setAutoDefault(true);

        horizontalLayout_2->addWidget(pb_circleImage);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lb_circleRadius = new QLabel(gb_circle);
        lb_circleRadius->setObjectName(QStringLiteral("lb_circleRadius"));

        verticalLayout_2->addWidget(lb_circleRadius);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        sb_circleRadius = new QSpinBox(gb_circle);
        sb_circleRadius->setObjectName(QStringLiteral("sb_circleRadius"));
        sb_circleRadius->setMinimum(1);
        sb_circleRadius->setMaximum(10);

        horizontalLayout->addWidget(sb_circleRadius);

        sl_circleRadius = new QSlider(gb_circle);
        sl_circleRadius->setObjectName(QStringLiteral("sl_circleRadius"));
        sl_circleRadius->setMinimum(1);
        sl_circleRadius->setMaximum(10);
        sl_circleRadius->setValue(1);
        sl_circleRadius->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(sl_circleRadius);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(gb_circle);

        gb_square = new QGroupBox(scrollAreaWidgetContents);
        gb_square->setObjectName(QStringLiteral("gb_square"));
        gridLayout_3 = new QGridLayout(gb_square);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pb_squareImage = new QPushButton(gb_square);
        pb_squareImage->setObjectName(QStringLiteral("pb_squareImage"));
        pb_squareImage->setStyleSheet(QStringLiteral(""));
        pb_squareImage->setCheckable(true);

        horizontalLayout_4->addWidget(pb_squareImage);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lb_squareSideLength = new QLabel(gb_square);
        lb_squareSideLength->setObjectName(QStringLiteral("lb_squareSideLength"));

        verticalLayout_3->addWidget(lb_squareSideLength);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        sb_squareRadius = new QSpinBox(gb_square);
        sb_squareRadius->setObjectName(QStringLiteral("sb_squareRadius"));
        sb_squareRadius->setMinimum(1);
        sb_squareRadius->setMaximum(10);

        horizontalLayout_3->addWidget(sb_squareRadius);

        sl_squareRadius = new QSlider(gb_square);
        sl_squareRadius->setObjectName(QStringLiteral("sl_squareRadius"));
        sl_squareRadius->setMinimum(1);
        sl_squareRadius->setMaximum(10);
        sl_squareRadius->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(sl_squareRadius);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout_3);


        gridLayout_3->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout->addWidget(gb_square);

        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pb_boxSelectionImage = new QPushButton(groupBox);
        pb_boxSelectionImage->setObjectName(QStringLiteral("pb_boxSelectionImage"));
        pb_boxSelectionImage->setStyleSheet(QStringLiteral(""));
        pb_boxSelectionImage->setCheckable(true);

        horizontalLayout_5->addWidget(pb_boxSelectionImage);

        horizontalSpacer = new QSpacerItem(248, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        gridLayout_4->addLayout(horizontalLayout_5, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 1, 0, 1, 1);

        btmbox_widget = new QDialogButtonBox(FreeFormWidget);
        btmbox_widget->setObjectName(QStringLiteral("btmbox_widget"));
        btmbox_widget->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(btmbox_widget, 2, 0, 1, 1);


        retranslateUi(FreeFormWidget);
        QObject::connect(sl_circleRadius, SIGNAL(valueChanged(int)), sb_circleRadius, SLOT(setValue(int)));
        QObject::connect(sb_circleRadius, SIGNAL(valueChanged(int)), sl_circleRadius, SLOT(setValue(int)));
        QObject::connect(sb_squareRadius, SIGNAL(valueChanged(int)), sl_squareRadius, SLOT(setValue(int)));
        QObject::connect(sl_squareRadius, SIGNAL(valueChanged(int)), sb_squareRadius, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(FreeFormWidget);
    } // setupUi

    void retranslateUi(QWidget *FreeFormWidget)
    {
        FreeFormWidget->setWindowTitle(QApplication::translate("FreeFormWidget", "FreeFormWidget", nullptr));
        gb_circle->setTitle(QApplication::translate("FreeFormWidget", "Circle", nullptr));
        pb_circleImage->setText(QString());
        lb_circleRadius->setText(QApplication::translate("FreeFormWidget", "Radius", nullptr));
        gb_square->setTitle(QApplication::translate("FreeFormWidget", "Square", nullptr));
        pb_squareImage->setText(QString());
        lb_squareSideLength->setText(QApplication::translate("FreeFormWidget", "Side length", nullptr));
        groupBox->setTitle(QApplication::translate("FreeFormWidget", "Box Selection", nullptr));
        pb_boxSelectionImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FreeFormWidget: public Ui_FreeFormWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FREEFORMWIDGET_H
