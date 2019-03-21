#include "voxeldatalistmanagerwidget.hpp"
#include <QString>
#include <QMenu>
#include <QAction>
#include <assert.h>


VoxelDataListItem::VoxelDataListItem(QStringList text,VoxelItem *it)
	: QTreeWidgetItem(text)
{
	this->it = it;
}

VoxelDataListItem::~VoxelDataListItem()
{

}


VoxelDataListManagerWidget::VoxelDataListManagerWidget(QWidget *parent) : QWidget(parent)
{
	// data
	sc = NULL;

	// vlayout
	vlayout_managerPanel = new QVBoxLayout(this);
	vlayout_managerPanel->setContentsMargins(0, 0, 0, 0);

	// tree widget
	listWidget = new QTreeWidget(this);
	QStringList strList;
	strList.append("Name");
	listWidget->setHeaderLabels(strList);

	vlayout_managerPanel->addWidget(listWidget);

	// set context menu
	listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

VoxelDataListManagerWidget::~VoxelDataListManagerWidget()
{

}

void VoxelDataListManagerWidget::paintEvent(QPaintEvent * e)
{
	QWidget::paintEvent(e);
}

void VoxelDataListManagerWidget::mousePressEvent(QMouseEvent * e)
{
	switch (e->buttons())
	{
	case Qt::LeftButton:
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:

		break;
	default:
		break;
	}
}

void VoxelDataListManagerWidget::buildList(void)
{
	if (sc == NULL)return;

	QStringList name;

	listWidget->clear();

	VoxelItem *itemPtr = sc;
	itemPtr = (VoxelItem*)itemPtr->link.flink;

	while(itemPtr->vdata != NULL) {
		VoxelDataListItem *nowItem;

		name = QStringList(itemPtr->vdata->name);
		nowItem = new VoxelDataListItem(name, itemPtr);
		listWidget->addTopLevelItem(nowItem);

		itemPtr = (VoxelItem*)itemPtr->link.flink;
	}

	VoxelDataListItem *firstItem = new VoxelDataListItem(name, (VoxelItem*)sc->link.flink);
	emit(refresh(((QTreeWidgetItem*)firstItem), 1));
	
	
}

//
//		SLOTS
//

void VoxelDataListManagerWidget::updateVoxelItem(VoxelItem *sc)
{
	this->sc = sc;
	buildList();
	update();
}
