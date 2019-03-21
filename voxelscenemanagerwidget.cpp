#include "voxelscenemanagerwidget.hpp"
#include <QString>
#include <QMenu>
#include <QAction>
#include <assert.h>


VoxelSceneManagerItem::VoxelSceneManagerItem(QStringList text, VoxelObject *obj)
	:QTreeWidgetItem(text)
{
	this->object = obj;
}

VoxelSceneManagerItem::~VoxelSceneManagerItem()
{

}


VoxelSceneManagerWidget::VoxelSceneManagerWidget(QWidget *parent) : QWidget(parent)
{
	// data
	sc = NULL;

	//setStyleSheet("background-color: rgba(0,0,0,0)");

	// vlayout
	vlayout_managerPanel = new QVBoxLayout(this);
	//titleBar = new TitleBar(this);
	//titleBar->setBackgroundColor(QColor(153, 204, 255));
	//vlayout_managerPanel->addWidget(titleBar);
	vlayout_managerPanel->setContentsMargins(0, 0, 0, 0);

	// tree widget
	treeWidget = new QTreeWidget(this);
	QStringList strList;
	strList.append("Name");
	treeWidget->setHeaderLabels(strList);
	//treeWidget->setStyleSheet("QHeaderView::section { background-color: rgba(0,0,0,0); }");

	vlayout_managerPanel->addWidget(treeWidget);

	// set context menu
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	// connect
	connect(treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(toggleContextMenu(const QPoint&)));
	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemSelected(QTreeWidgetItem*, int)));
	//connect(titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	//connect(titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragged(int, int)));
}

VoxelSceneManagerWidget::~VoxelSceneManagerWidget()
{

}

void VoxelSceneManagerWidget::paintEvent(QPaintEvent * e)
{
	//QPainter painter(this);

	//painter.drawRect(0, 0, width() - 5, height() - 7);
	//painter.fillRect(titleBar->rect().adjusted(1, 1, 15, 0), titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void VoxelSceneManagerWidget::mousePressEvent(QMouseEvent * e)
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

void VoxelSceneManagerWidget::buildTree(void)
{
	if (sc == NULL) return;

	VoxelSceneManagerItem *parentItem, *childItem;
	QStringList name;
	VoxelObject *parentObject, *childObject;
	Dequeue *itemDequeue;

	treeWidget->clear();
	name = QStringList(sc->root->name);
	parentItem = new VoxelSceneManagerItem(name, sc->root);
	treeWidget->addTopLevelItem(parentItem);
	itemDequeue = newDequeue();
	pushBackDequeue(itemDequeue, parentItem);
	while (!isEmptyDequeue(itemDequeue))
	{
		parentItem = (VoxelSceneManagerItem*)popFrontDequeue(itemDequeue);
		parentObject = parentItem->object;
		childObject = childOfVoxelObject(parentObject);
		while (childObject != NULL)
		{
			name = QStringList(childObject->name);
			childItem = new VoxelSceneManagerItem(name, childObject);
			parentItem->addChild(childItem);
			childObject = nextSiblingOfVoxelObject(childObject);
			pushBackDequeue(itemDequeue, childItem);
		}
	}
	deleteDequeue(&itemDequeue);
}

//
//		SLOTS
//

void VoxelSceneManagerWidget::updateVoxelModel(VoxelModel *sc)
{
	this->sc = sc;
	buildTree();
	update();
}

void VoxelSceneManagerWidget::toggleContextMenu(const QPoint& pos)
{
	//leaf voxel object menu
	QMenu leafObjMenu(tr("Actions"));

	//  Panels
	//QAction leafObjAction1(tr("Projection mapping"), this);
	QAction leafObjAction1(tr("Export DF data"), this);
	QAction leafObjAction2(tr("Delete"), this);
	QAction leafObjAction3(tr("Clip"), this);
	//action1.setCheckable(true);
	//action1.setChecked(this->lightWidget->isVisible());
	//connect(&leafObjAction1, SIGNAL(triggered()), this, SLOT(openImageProjectionWidget()));
	connect(&leafObjAction2, SIGNAL(triggered()), this, SLOT(deleteVoxelObject()));
	connect(&leafObjAction3, SIGNAL(triggered()), this, SLOT(openVoxelClipWidget()));
	connect(&leafObjAction1, SIGNAL(triggered()), this, SLOT(exportDistanceData()));
	leafObjMenu.addAction(&leafObjAction3);
	leafObjMenu.addAction(&leafObjAction2);

	//normal voxel object menu
	QMenu normalObjMenu(tr("Actions"));

	//  Panels
	QAction normalObjAction1(tr("Add"), this);
	QAction normalObjAction2(tr("Delete"), this);
	
	connect(&normalObjAction1, SIGNAL(triggered()), this, SLOT(addVoxelObject()));
	connect(&normalObjAction2, SIGNAL(triggered()), this, SLOT(deleteVoxelObject()));
	
	normalObjMenu.addAction(&normalObjAction1);
	normalObjMenu.addAction(&normalObjAction2);

	//root voxel object menu
	QMenu rootObjMenu(tr("Actions"));

	//  Panels
	QAction rootObjAction1(tr("Add"), this);

	connect(&rootObjAction1, SIGNAL(triggered()), this, SLOT(addVoxelObject()));

	rootObjMenu.addAction(&rootObjAction1);


	selectedItem = treeWidget->itemAt(pos);
	
	if (selectedItem == NULL) return;

	if (!selectedItem->parent()) {	// root obj only can add obj
		rootObjMenu.exec(treeWidget->mapToGlobal(pos));
	}
	else if (selectedItem != NULL) {		// leaf voxel object 
		if (((VoxelSceneManagerItem*)treeWidget->itemAt(pos))->object->dataItem->vdata->df) {
			leafObjMenu.addAction(&leafObjAction1);
		}
		leafObjMenu.exec(treeWidget->mapToGlobal(pos));
	}
	else {							// normal voxel object
		normalObjMenu.exec(treeWidget->mapToGlobal(pos));
	}
}

void VoxelSceneManagerWidget::voxelSelected(VoxelItem *item)
{
	VoxelObject *obj = item->parent;

	QList<QTreeWidgetItem*> list = this->treeWidget->findItems(obj->name, Qt::MatchContains | Qt::MatchRecursive, 0);
	foreach(QTreeWidgetItem* item, list)
	{
		VoxelSceneManagerItem* sceneItem = (VoxelSceneManagerItem*)item;
		if (sceneItem->object == obj) {
			this->treeWidget->setCurrentItem(item);
		}
	}
}

void VoxelSceneManagerWidget::itemSelected(QTreeWidgetItem* witem, int index)
{
	VoxelSceneManagerItem *item = (VoxelSceneManagerItem*)witem;
	VoxelObject *obj = item->object;
	
	emit actionOnVoxelObject(obj, VOBJ_SELECT);
}

void VoxelSceneManagerWidget::openImageProjectionWidget(void)
{
	if (selectedItem == NULL) return;

	QModelIndex index = treeWidget->currentIndex();

	printf("row: %d\ncol: %d\n", index.row(), index.column());
}

void VoxelSceneManagerWidget::deleteVoxelObject(void)
{
	VoxelSceneManagerItem *item;
	VoxelObject *obj;
	QList<QTreeWidgetItem *>  items = this->treeWidget->selectedItems();
	QTreeWidgetItem          *pp = nullptr;

	if (!items.isEmpty())
	{
		item = (VoxelSceneManagerItem*)selectedItem;
		obj = item->object;

		foreach(QTreeWidgetItem *item, items)
		{
			pp = item->parent();
			pp->removeChild(item);
			delete item;
		}
	}

	emit actionOnVoxelObject(obj, VOBJ_DELETE);
	update();
}

void VoxelSceneManagerWidget::openVoxelClipWidget(void)
{
	VoxelSceneManagerItem *item;
	VoxelObject *obj;

	if (selectedItem == NULL) return;

	item = (VoxelSceneManagerItem*)selectedItem;
	obj = item->object;

	emit actionOnVoxelObject(obj, VOBJ_CLIP);
}

void VoxelSceneManagerWidget::addVoxelObject(void)
{
	VoxelSceneManagerItem *item;
	VoxelObject *obj;
	
	item = (VoxelSceneManagerItem*)selectedItem;
	obj = item->object;

	emit actionOnVoxelObject(obj, VOBJ_ADD);
}

void VoxelSceneManagerWidget::exportDistanceData()
{
	VoxelSceneManagerItem *item;
	VoxelObject *obj;

	item = (VoxelSceneManagerItem*)selectedItem;
	obj = item->object;

	emit actionOnVoxelObject(obj, VOBJ_EXPORT_DF);

}
