/********************************************************************************
** Form generated from reading UI file 'slicewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLICEWIDGET_H
#define UI_SLICEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SliceWidget
{
public:
    QGridLayout *gridLayout_4;
    QSplitter *sph_settingAndViewer;
    QScrollArea *sa_imageviewer;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QSpacerItem *vspr_setting;
    QGroupBox *gb_sliceSetting;
    QGridLayout *gridLayout_2;
    QLineEdit *le_nozzleSize;
    QLabel *lb_layerHeight;
    QLabel *lb_nozzleSize;
    QLineEdit *le_layerHeight;
    QGroupBox *gb_machineSetting;
    QGridLayout *gridLayout_3;
    QLabel *lb_temperature;
    QLineEdit *le_temperature;
    QLabel *lb_filamentDiameter;
    QLineEdit *le_filamentDiameter;
    QLabel *lb_feedRate;
    QLineEdit *le_feedRate;
    QScrollArea *sa_viewer;
    QWidget *scrollAreaWidgetContents_2;
    QSlider *vslid_image;
    QDialogButtonBox *btmbox_widget;

    void setupUi(QWidget *SliceWidget)
    {
        if (SliceWidget->objectName().isEmpty())
            SliceWidget->setObjectName(QStringLiteral("SliceWidget"));
        SliceWidget->resize(522, 369);
        gridLayout_4 = new QGridLayout(SliceWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        sph_settingAndViewer = new QSplitter(SliceWidget);
        sph_settingAndViewer->setObjectName(QStringLiteral("sph_settingAndViewer"));
        sph_settingAndViewer->setOrientation(Qt::Horizontal);
        sa_imageviewer = new QScrollArea(sph_settingAndViewer);
        sa_imageviewer->setObjectName(QStringLiteral("sa_imageviewer"));
        sa_imageviewer->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 169, 320));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        vspr_setting = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(vspr_setting, 2, 0, 1, 1);

        gb_sliceSetting = new QGroupBox(scrollAreaWidgetContents);
        gb_sliceSetting->setObjectName(QStringLiteral("gb_sliceSetting"));
        gridLayout_2 = new QGridLayout(gb_sliceSetting);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        le_nozzleSize = new QLineEdit(gb_sliceSetting);
        le_nozzleSize->setObjectName(QStringLiteral("le_nozzleSize"));

        gridLayout_2->addWidget(le_nozzleSize, 1, 0, 1, 1);

        lb_layerHeight = new QLabel(gb_sliceSetting);
        lb_layerHeight->setObjectName(QStringLiteral("lb_layerHeight"));

        gridLayout_2->addWidget(lb_layerHeight, 2, 0, 1, 1);

        lb_nozzleSize = new QLabel(gb_sliceSetting);
        lb_nozzleSize->setObjectName(QStringLiteral("lb_nozzleSize"));

        gridLayout_2->addWidget(lb_nozzleSize, 0, 0, 1, 1);

        le_layerHeight = new QLineEdit(gb_sliceSetting);
        le_layerHeight->setObjectName(QStringLiteral("le_layerHeight"));

        gridLayout_2->addWidget(le_layerHeight, 3, 0, 1, 1);


        gridLayout->addWidget(gb_sliceSetting, 0, 0, 1, 1);

        gb_machineSetting = new QGroupBox(scrollAreaWidgetContents);
        gb_machineSetting->setObjectName(QStringLiteral("gb_machineSetting"));
        gridLayout_3 = new QGridLayout(gb_machineSetting);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lb_temperature = new QLabel(gb_machineSetting);
        lb_temperature->setObjectName(QStringLiteral("lb_temperature"));

        gridLayout_3->addWidget(lb_temperature, 0, 0, 1, 1);

        le_temperature = new QLineEdit(gb_machineSetting);
        le_temperature->setObjectName(QStringLiteral("le_temperature"));

        gridLayout_3->addWidget(le_temperature, 1, 0, 1, 1);

        lb_filamentDiameter = new QLabel(gb_machineSetting);
        lb_filamentDiameter->setObjectName(QStringLiteral("lb_filamentDiameter"));

        gridLayout_3->addWidget(lb_filamentDiameter, 2, 0, 1, 1);

        le_filamentDiameter = new QLineEdit(gb_machineSetting);
        le_filamentDiameter->setObjectName(QStringLiteral("le_filamentDiameter"));

        gridLayout_3->addWidget(le_filamentDiameter, 3, 0, 1, 1);

        lb_feedRate = new QLabel(gb_machineSetting);
        lb_feedRate->setObjectName(QStringLiteral("lb_feedRate"));

        gridLayout_3->addWidget(lb_feedRate, 4, 0, 1, 1);

        le_feedRate = new QLineEdit(gb_machineSetting);
        le_feedRate->setObjectName(QStringLiteral("le_feedRate"));

        gridLayout_3->addWidget(le_feedRate, 5, 0, 1, 1);


        gridLayout->addWidget(gb_machineSetting, 1, 0, 1, 1);

        sa_imageviewer->setWidget(scrollAreaWidgetContents);
        sph_settingAndViewer->addWidget(sa_imageviewer);
        sa_viewer = new QScrollArea(sph_settingAndViewer);
        sa_viewer->setObjectName(QStringLiteral("sa_viewer"));
        sa_viewer->setMinimumSize(QSize(300, 0));
        sa_viewer->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 298, 320));
        sa_viewer->setWidget(scrollAreaWidgetContents_2);
        sph_settingAndViewer->addWidget(sa_viewer);

        gridLayout_4->addWidget(sph_settingAndViewer, 0, 0, 1, 1);

        vslid_image = new QSlider(SliceWidget);
        vslid_image->setObjectName(QStringLiteral("vslid_image"));
        vslid_image->setOrientation(Qt::Vertical);

        gridLayout_4->addWidget(vslid_image, 0, 1, 1, 1);

        btmbox_widget = new QDialogButtonBox(SliceWidget);
        btmbox_widget->setObjectName(QStringLiteral("btmbox_widget"));
        btmbox_widget->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_4->addWidget(btmbox_widget, 1, 0, 1, 2);


        retranslateUi(SliceWidget);

        QMetaObject::connectSlotsByName(SliceWidget);
    } // setupUi

    void retranslateUi(QWidget *SliceWidget)
    {
        SliceWidget->setWindowTitle(QApplication::translate("SliceWidget", "SliceWidget", nullptr));
        gb_sliceSetting->setTitle(QApplication::translate("SliceWidget", "Slice Setting", nullptr));
        lb_layerHeight->setText(QApplication::translate("SliceWidget", "Layer Height (mm)", nullptr));
        lb_nozzleSize->setText(QApplication::translate("SliceWidget", "Nozzel Size (mm)", nullptr));
        gb_machineSetting->setTitle(QApplication::translate("SliceWidget", "Machine Setting", nullptr));
        lb_temperature->setText(QApplication::translate("SliceWidget", "Temperature", nullptr));
        lb_filamentDiameter->setText(QApplication::translate("SliceWidget", "Filament Diameter", nullptr));
        lb_feedRate->setText(QApplication::translate("SliceWidget", "Feed Rate (mm/s)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SliceWidget: public Ui_SliceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLICEWIDGET_H
