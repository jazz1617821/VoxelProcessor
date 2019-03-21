#include "voxelprocessor.hpp"
#include "voxelize.h"
#include <QFileDialog>
#include <QTreeWidget>

VoxelProcessor::VoxelProcessor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowIcon(QIcon(("ApplicationImages/voxel.ico")));

	connect(this, SIGNAL(createVoxelModel(void)), ui.ogl_voxelViewer, SLOT(createVoxelModel()));
	connect(this, SIGNAL(openVoxelModel(const char*)),ui.ogl_voxelViewer, SLOT(openVoxelModel(const char*)));
	connect(this, SIGNAL(saveVoxelModel()), ui.ogl_voxelViewer, SLOT(saveVoxelModel()));
	connect(ui.ogl_voxelViewer, SIGNAL(sendVoxelModel(VoxelModel*)), ui.wid_voxelScene, SLOT(updateVoxelModel(VoxelModel*)));
	connect(ui.ogl_voxelViewer, SIGNAL(sendVoxelItem(VoxelItem*)), ui.wid_dataList, SLOT(updateVoxelItem(VoxelItem*)));

	connect(ui.ogl_voxelViewer, SIGNAL(selectVoxel(VoxelItem*)), ui.wid_voxelScene, SLOT(voxelSelected(VoxelItem*)));
	connect(ui.wid_voxelScene, SIGNAL(actionOnVoxelObject(VoxelObject*, int)), ui.ogl_voxelViewer, SLOT(actionOnVoxelObject(VoxelObject*, int)));

	// view widget pb clicked action
	connect(ui.pb_view, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(changeToViewMode(void)));
	connect(ui.pb_translate, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(changeToTranslateMode(void)));
	connect(ui.pb_rotate, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(changeToRotateMode(void)));
	connect(ui.pb_scale, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(changeToScaleMode(void)));
	connect(ui.pb_df, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(openVDFWidget(void)));
	connect(ui.pb_thickness, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(openVTHNWidget(void)));
	connect(ui.pb_slice, SIGNAL(released(void)), ui.ogl_voxelViewer, SLOT(openSliceWidget(void)));

	// edit widget pb clicked action
	connect(ui.pb_control, SIGNAL(clicked(void)), ui.ogl_editViewer, SLOT(openControlWidget(void)));
	connect(ui.pb_selection, SIGNAL(clicked(void)), ui.ogl_editViewer, SLOT(openSelectionWidget(void)));
	connect(ui.pb_freeform, SIGNAL(clicked(void)), ui.ogl_editViewer, SLOT(openFreeFormWidget(void)));
	connect(ui.pb_smoothing, SIGNAL(clicked(void)), ui.ogl_editViewer, SLOT(openSmoothingWidget(void)));
	connect(ui.pb_connectingcomponent, SIGNAL(clicked(void)), ui.ogl_editViewer, SLOT(recalculateConnectingComponent(void)));

	//Attribute
	connect(ui.ogl_voxelViewer, SIGNAL(sendVoxelObject(VoxelObject*)), ui.wid_objectAttribute, SLOT(updateAttribute(VoxelObject*)));
	connect(ui.wid_dataList->listWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), ui.wid_dataAttribute, SLOT(updateAttribute(QTreeWidgetItem*,int)));
	connect(ui.wid_voxelScene, SIGNAL(deleteObject(void)), ui.wid_objectAttribute, SLOT(clearVoxelObjectAttributePanel(void)));

	//datalist sent data to edit view
	connect(ui.wid_dataList->listWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), ui.ogl_editViewer, SLOT(acceptVoxelItem(QTreeWidgetItem*, int)));
	connect(ui.wid_dataList, SIGNAL(refresh(QTreeWidgetItem* ,int)), ui.ogl_editViewer, SLOT(acceptVoxelItem(QTreeWidgetItem*, int)));

	// change tab
	connect(ui.tab_mainWidget, SIGNAL(tabBarClicked(int)), ui.ogl_voxelViewer, SLOT(changeToEditWidget(int)));			// sent voxel item to edit widget when click edit widget 
	connect(ui.tab_mainWidget, SIGNAL(tabBarClicked(int)), ui.ogl_editViewer, SLOT(changeToViewWidget(int)));


	connect(ui.wid_objectAttribute, SIGNAL(stateChanged(int)), ui.ogl_voxelViewer, SLOT(changeDFEnable(int)));
}

VoxelProcessor::~VoxelProcessor()
{

}

void VoxelProcessor::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All Files (*.vm *.vdat);;VM Files (*.vm);;VD Files (*.vdat);;"));
	QByteArray ba = filename.toLatin1();
	const char *str = ba.data();
	const char *fe;	// file extension

	fe = strrchr(str, '.');
	if (!filename.isEmpty()) {
		if (!strcmp(fe, ".vm")) {
			emit(openVoxelModel(str));
		}
	}
}

void VoxelProcessor::on_actionQuit_triggered()
{
	exit(1);
}

void VoxelProcessor::on_actionNew_triggered()
{
	emit(createVoxelModel());
}

void VoxelProcessor::on_actionSave_triggered()
{
	emit(saveVoxelModel());
}
