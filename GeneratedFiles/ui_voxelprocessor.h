/********************************************************************************
** Form generated from reading UI file 'voxelprocessor.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELPROCESSOR_H
#define UI_VOXELPROCESSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "voxelattributewidget.hpp"
#include "voxeldatalistmanagerwidget.hpp"
#include "voxeleditoroglwidget.hpp"
#include "voxelscenemanagerwidget.hpp"
#include "voxelvieweroglwidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_VoxelProcessorClass
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionNew;
    QAction *actionSave;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tab_mainWidget;
    QWidget *tab_voxelScene;
    QVBoxLayout *verticalLayout;
    QSplitter *spl_viewerAll;
    QSplitter *spl_managerandViewer;
    QSplitter *spl_managerandAttribute;
    VoxelSceneManagerWidget *wid_voxelScene;
    VoxelAttributeWidget *wid_objectAttribute;
    VoxelViewerOGLWidget *ogl_voxelViewer;
    QScrollArea *sa_voxelTools;
    QWidget *sa_voxelToolsContents;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pb_view;
    QPushButton *pb_translate;
    QPushButton *pb_rotate;
    QPushButton *pb_scale;
    QPushButton *pb_df;
    QPushButton *pb_thickness;
    QPushButton *pb_boolean;
    QPushButton *pb_slice;
    QSpacerItem *verticalSpacer;
    QWidget *tab_voxelEditor;
    QVBoxLayout *verticalLayout_4;
    QSplitter *spl_editWidgetAll;
    QSplitter *spl_listandViewer;
    QSplitter *spl_listandAttribute;
    VoxelDataListManagerWidget *wid_dataList;
    VoxelAttributeWidget *wid_dataAttribute;
    VoxelEditorOGLWidget *ogl_editViewer;
    QScrollArea *sa_editTools;
    QWidget *sa_editToolsContents;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pb_selection;
    QPushButton *pb_control;
    QPushButton *pb_freeform;
    QPushButton *pb_smoothing;
    QPushButton *pb_connectingcomponent;
    QPushButton *pb_7;
    QPushButton *pb_8;
    QSpacerItem *verticalSpacer_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VoxelProcessorClass)
    {
        if (VoxelProcessorClass->objectName().isEmpty())
            VoxelProcessorClass->setObjectName(QStringLiteral("VoxelProcessorClass"));
        VoxelProcessorClass->resize(795, 583);
        actionOpen = new QAction(VoxelProcessorClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(VoxelProcessorClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionNew = new QAction(VoxelProcessorClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionSave = new QAction(VoxelProcessorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new QWidget(VoxelProcessorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tab_mainWidget = new QTabWidget(centralWidget);
        tab_mainWidget->setObjectName(QStringLiteral("tab_mainWidget"));
        tab_voxelScene = new QWidget();
        tab_voxelScene->setObjectName(QStringLiteral("tab_voxelScene"));
        verticalLayout = new QVBoxLayout(tab_voxelScene);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        spl_viewerAll = new QSplitter(tab_voxelScene);
        spl_viewerAll->setObjectName(QStringLiteral("spl_viewerAll"));
        spl_viewerAll->setOrientation(Qt::Horizontal);
        spl_managerandViewer = new QSplitter(spl_viewerAll);
        spl_managerandViewer->setObjectName(QStringLiteral("spl_managerandViewer"));
        spl_managerandViewer->setOrientation(Qt::Horizontal);
        spl_managerandAttribute = new QSplitter(spl_managerandViewer);
        spl_managerandAttribute->setObjectName(QStringLiteral("spl_managerandAttribute"));
        spl_managerandAttribute->setMinimumSize(QSize(200, 0));
        spl_managerandAttribute->setOrientation(Qt::Vertical);
        wid_voxelScene = new VoxelSceneManagerWidget(spl_managerandAttribute);
        wid_voxelScene->setObjectName(QStringLiteral("wid_voxelScene"));
        wid_voxelScene->setEnabled(true);
        wid_voxelScene->setMinimumSize(QSize(0, 200));
        spl_managerandAttribute->addWidget(wid_voxelScene);
        wid_objectAttribute = new VoxelAttributeWidget(spl_managerandAttribute);
        wid_objectAttribute->setObjectName(QStringLiteral("wid_objectAttribute"));
        wid_objectAttribute->setMinimumSize(QSize(0, 200));
        spl_managerandAttribute->addWidget(wid_objectAttribute);
        spl_managerandViewer->addWidget(spl_managerandAttribute);
        ogl_voxelViewer = new VoxelViewerOGLWidget(spl_managerandViewer);
        ogl_voxelViewer->setObjectName(QStringLiteral("ogl_voxelViewer"));
        ogl_voxelViewer->setMinimumSize(QSize(400, 0));
        spl_managerandViewer->addWidget(ogl_voxelViewer);
        spl_viewerAll->addWidget(spl_managerandViewer);
        sa_voxelTools = new QScrollArea(spl_viewerAll);
        sa_voxelTools->setObjectName(QStringLiteral("sa_voxelTools"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sa_voxelTools->sizePolicy().hasHeightForWidth());
        sa_voxelTools->setSizePolicy(sizePolicy);
        sa_voxelTools->setMinimumSize(QSize(0, 0));
        sa_voxelTools->setMaximumSize(QSize(83, 16777215));
        sa_voxelTools->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        sa_voxelTools->setWidgetResizable(true);
        sa_voxelToolsContents = new QWidget();
        sa_voxelToolsContents->setObjectName(QStringLiteral("sa_voxelToolsContents"));
        sa_voxelToolsContents->setGeometry(QRect(0, 0, 64, 512));
        verticalLayout_2 = new QVBoxLayout(sa_voxelToolsContents);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        pb_view = new QPushButton(sa_voxelToolsContents);
        pb_view->setObjectName(QStringLiteral("pb_view"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pb_view->sizePolicy().hasHeightForWidth());
        pb_view->setSizePolicy(sizePolicy1);
        pb_view->setMinimumSize(QSize(0, 64));
        pb_view->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_view);

        pb_translate = new QPushButton(sa_voxelToolsContents);
        pb_translate->setObjectName(QStringLiteral("pb_translate"));
        pb_translate->setMinimumSize(QSize(0, 64));
        pb_translate->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_translate);

        pb_rotate = new QPushButton(sa_voxelToolsContents);
        pb_rotate->setObjectName(QStringLiteral("pb_rotate"));
        pb_rotate->setMinimumSize(QSize(0, 64));
        pb_rotate->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_rotate);

        pb_scale = new QPushButton(sa_voxelToolsContents);
        pb_scale->setObjectName(QStringLiteral("pb_scale"));
        pb_scale->setMinimumSize(QSize(0, 64));
        pb_scale->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_scale);

        pb_df = new QPushButton(sa_voxelToolsContents);
        pb_df->setObjectName(QStringLiteral("pb_df"));
        pb_df->setMinimumSize(QSize(0, 64));
        pb_df->setMaximumSize(QSize(64, 16777215));
        pb_df->setAutoDefault(false);
        pb_df->setFlat(false);

        verticalLayout_2->addWidget(pb_df);

        pb_thickness = new QPushButton(sa_voxelToolsContents);
        pb_thickness->setObjectName(QStringLiteral("pb_thickness"));
        pb_thickness->setMinimumSize(QSize(0, 64));
        pb_thickness->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_thickness);

        pb_boolean = new QPushButton(sa_voxelToolsContents);
        pb_boolean->setObjectName(QStringLiteral("pb_boolean"));
        pb_boolean->setMinimumSize(QSize(0, 64));
        pb_boolean->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_boolean);

        pb_slice = new QPushButton(sa_voxelToolsContents);
        pb_slice->setObjectName(QStringLiteral("pb_slice"));
        pb_slice->setMinimumSize(QSize(0, 64));
        pb_slice->setMaximumSize(QSize(64, 16777215));

        verticalLayout_2->addWidget(pb_slice);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        sa_voxelTools->setWidget(sa_voxelToolsContents);
        spl_viewerAll->addWidget(sa_voxelTools);

        verticalLayout->addWidget(spl_viewerAll);

        tab_mainWidget->addTab(tab_voxelScene, QString());
        tab_voxelEditor = new QWidget();
        tab_voxelEditor->setObjectName(QStringLiteral("tab_voxelEditor"));
        verticalLayout_4 = new QVBoxLayout(tab_voxelEditor);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        spl_editWidgetAll = new QSplitter(tab_voxelEditor);
        spl_editWidgetAll->setObjectName(QStringLiteral("spl_editWidgetAll"));
        spl_editWidgetAll->setOrientation(Qt::Horizontal);
        spl_listandViewer = new QSplitter(spl_editWidgetAll);
        spl_listandViewer->setObjectName(QStringLiteral("spl_listandViewer"));
        spl_listandViewer->setOrientation(Qt::Horizontal);
        spl_listandAttribute = new QSplitter(spl_listandViewer);
        spl_listandAttribute->setObjectName(QStringLiteral("spl_listandAttribute"));
        spl_listandAttribute->setMinimumSize(QSize(200, 0));
        spl_listandAttribute->setOrientation(Qt::Vertical);
        wid_dataList = new VoxelDataListManagerWidget(spl_listandAttribute);
        wid_dataList->setObjectName(QStringLiteral("wid_dataList"));
        wid_dataList->setEnabled(true);
        wid_dataList->setMinimumSize(QSize(0, 200));
        spl_listandAttribute->addWidget(wid_dataList);
        wid_dataAttribute = new VoxelAttributeWidget(spl_listandAttribute);
        wid_dataAttribute->setObjectName(QStringLiteral("wid_dataAttribute"));
        wid_dataAttribute->setMinimumSize(QSize(0, 200));
        spl_listandAttribute->addWidget(wid_dataAttribute);
        spl_listandViewer->addWidget(spl_listandAttribute);
        ogl_editViewer = new VoxelEditorOGLWidget(spl_listandViewer);
        ogl_editViewer->setObjectName(QStringLiteral("ogl_editViewer"));
        ogl_editViewer->setMinimumSize(QSize(400, 0));
        ogl_editViewer->setMouseTracking(true);
        spl_listandViewer->addWidget(ogl_editViewer);
        spl_editWidgetAll->addWidget(spl_listandViewer);
        sa_editTools = new QScrollArea(spl_editWidgetAll);
        sa_editTools->setObjectName(QStringLiteral("sa_editTools"));
        sizePolicy.setHeightForWidth(sa_editTools->sizePolicy().hasHeightForWidth());
        sa_editTools->setSizePolicy(sizePolicy);
        sa_editTools->setMinimumSize(QSize(0, 0));
        sa_editTools->setMaximumSize(QSize(83, 16777215));
        sa_editTools->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        sa_editTools->setWidgetResizable(true);
        sa_editToolsContents = new QWidget();
        sa_editToolsContents->setObjectName(QStringLiteral("sa_editToolsContents"));
        sa_editToolsContents->setGeometry(QRect(0, 0, 64, 502));
        verticalLayout_3 = new QVBoxLayout(sa_editToolsContents);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        pb_selection = new QPushButton(sa_editToolsContents);
        pb_selection->setObjectName(QStringLiteral("pb_selection"));
        pb_selection->setMinimumSize(QSize(0, 64));
        pb_selection->setMaximumSize(QSize(64, 16777215));
        pb_selection->setAutoDefault(false);
        pb_selection->setFlat(false);

        verticalLayout_3->addWidget(pb_selection);

        pb_control = new QPushButton(sa_editToolsContents);
        pb_control->setObjectName(QStringLiteral("pb_control"));
        sizePolicy1.setHeightForWidth(pb_control->sizePolicy().hasHeightForWidth());
        pb_control->setSizePolicy(sizePolicy1);
        pb_control->setMinimumSize(QSize(0, 64));
        pb_control->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_control);

        pb_freeform = new QPushButton(sa_editToolsContents);
        pb_freeform->setObjectName(QStringLiteral("pb_freeform"));
        pb_freeform->setMinimumSize(QSize(0, 64));
        pb_freeform->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_freeform);

        pb_smoothing = new QPushButton(sa_editToolsContents);
        pb_smoothing->setObjectName(QStringLiteral("pb_smoothing"));
        pb_smoothing->setMinimumSize(QSize(0, 64));
        pb_smoothing->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_smoothing);

        pb_connectingcomponent = new QPushButton(sa_editToolsContents);
        pb_connectingcomponent->setObjectName(QStringLiteral("pb_connectingcomponent"));
        pb_connectingcomponent->setMinimumSize(QSize(0, 64));
        pb_connectingcomponent->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_connectingcomponent);

        pb_7 = new QPushButton(sa_editToolsContents);
        pb_7->setObjectName(QStringLiteral("pb_7"));
        pb_7->setMinimumSize(QSize(0, 64));
        pb_7->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_7);

        pb_8 = new QPushButton(sa_editToolsContents);
        pb_8->setObjectName(QStringLiteral("pb_8"));
        pb_8->setMinimumSize(QSize(0, 64));
        pb_8->setMaximumSize(QSize(64, 16777215));

        verticalLayout_3->addWidget(pb_8);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        sa_editTools->setWidget(sa_editToolsContents);
        spl_editWidgetAll->addWidget(sa_editTools);

        verticalLayout_4->addWidget(spl_editWidgetAll);

        tab_mainWidget->addTab(tab_voxelEditor, QString());

        horizontalLayout->addWidget(tab_mainWidget);

        VoxelProcessorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VoxelProcessorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 795, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        VoxelProcessorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VoxelProcessorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VoxelProcessorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VoxelProcessorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VoxelProcessorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(VoxelProcessorClass);

        tab_mainWidget->setCurrentIndex(0);
        pb_df->setDefault(false);
        pb_selection->setDefault(false);


        QMetaObject::connectSlotsByName(VoxelProcessorClass);
    } // setupUi

    void retranslateUi(QMainWindow *VoxelProcessorClass)
    {
        VoxelProcessorClass->setWindowTitle(QApplication::translate("VoxelProcessorClass", "VoxelProcessor", nullptr));
        actionOpen->setText(QApplication::translate("VoxelProcessorClass", "Open", nullptr));
        actionQuit->setText(QApplication::translate("VoxelProcessorClass", "Quit", nullptr));
        actionNew->setText(QApplication::translate("VoxelProcessorClass", "New", nullptr));
        actionSave->setText(QApplication::translate("VoxelProcessorClass", "Save", nullptr));
        pb_view->setText(QApplication::translate("VoxelProcessorClass", "View", nullptr));
        pb_translate->setText(QApplication::translate("VoxelProcessorClass", "Translate", nullptr));
        pb_rotate->setText(QApplication::translate("VoxelProcessorClass", "Rotate", nullptr));
        pb_scale->setText(QApplication::translate("VoxelProcessorClass", "Scale", nullptr));
        pb_df->setText(QApplication::translate("VoxelProcessorClass", "DF", nullptr));
        pb_thickness->setText(QApplication::translate("VoxelProcessorClass", "Thickness", nullptr));
        pb_boolean->setText(QApplication::translate("VoxelProcessorClass", "Boolean", nullptr));
        pb_slice->setText(QApplication::translate("VoxelProcessorClass", "Slice", nullptr));
        tab_mainWidget->setTabText(tab_mainWidget->indexOf(tab_voxelScene), QApplication::translate("VoxelProcessorClass", "Scene", nullptr));
        pb_selection->setText(QApplication::translate("VoxelProcessorClass", "Selection", nullptr));
        pb_control->setText(QApplication::translate("VoxelProcessorClass", "Control", nullptr));
        pb_freeform->setText(QApplication::translate("VoxelProcessorClass", "FreeForm", nullptr));
        pb_smoothing->setText(QApplication::translate("VoxelProcessorClass", "Smoothing", nullptr));
        pb_connectingcomponent->setText(QApplication::translate("VoxelProcessorClass", "Connecting\n"
"Component", nullptr));
        pb_7->setText(QApplication::translate("VoxelProcessorClass", "7", nullptr));
        pb_8->setText(QApplication::translate("VoxelProcessorClass", "8", nullptr));
        tab_mainWidget->setTabText(tab_mainWidget->indexOf(tab_voxelEditor), QApplication::translate("VoxelProcessorClass", "Editor", nullptr));
        menuFile->setTitle(QApplication::translate("VoxelProcessorClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VoxelProcessorClass: public Ui_VoxelProcessorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELPROCESSOR_H
