#include "voxelattributewidget.hpp"

VoxelAttributeWidget::VoxelAttributeWidget(QWidget *parent)
{
	QStringList strs;

	distfCheckbox = new QCheckBox();

	//set vLayout
	vlayout_managerPanel = new QVBoxLayout(this);
	vlayout_managerPanel->setContentsMargins(0, 0, 0, 0);

	//init treewidget
	treeWidget = new QTreeWidget(this);
	treeWidget->clear();
	QStringList strList;
	strList.append("Attribute");
	strList.append("Value");
	treeWidget->setHeaderLabels(strList);

	vlayout_managerPanel->addWidget(treeWidget);

	// set context menu
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

}

VoxelAttributeWidget::~VoxelAttributeWidget()
{

}

void VoxelAttributeWidget::updateAttribute(VoxelObject * obj)
{
	QTreeWidget *w = this->treeWidget;
	QTreeWidgetItem *witem, *witem2;
	QString str;
	QStringList strs;
	VoxelData *vdata;
	DistanceField *df;

	w->clear();
	// add attributes
	// Voxel Object Name
	strs.append(tr("Name"));
	strs.append(obj->name);
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	// add to widget
	w->addTopLevelItem(witem);

	// Voxel BBox
	strs.append(tr("Bounding Box"));
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	strs.append(tr("Max"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->max[0]).arg(obj->bbox->max[1]).arg(obj->bbox->max[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	strs.append(tr("Min"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->min[0]).arg(obj->bbox->min[1]).arg(obj->bbox->min[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	// add to widget
	w->addTopLevelItem(witem);

	// Number Of children
	strs.append(tr("Children"));
	strs.append(QString::number(obj->numberOfChild));
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	// add to widget
	w->addTopLevelItem(witem);

	// Voxel Items
	if (obj->dataItem != NULL) {
		vdata = obj->dataItem->vdata;

		// Voxel Resolution
		strs.append(tr("Voxel Resolution"));
		str = QString("%1 x %2 x %3").arg(vdata->resolution[0]).arg(vdata->resolution[1]).arg(vdata->resolution[2]);
		strs.append(str);
		witem = new QTreeWidgetItem(strs);
		strs.clear();
		// add to widget
		w->addTopLevelItem(witem);

		// Voxel Size
		strs.append(tr("Voxel Size"));
		str = QString("%1 x %2 x %3").arg(vdata->voxelSize[0]).arg(vdata->voxelSize[1]).arg(vdata->voxelSize[2]);
		strs.append(str);
		witem = new QTreeWidgetItem(strs);
		strs.clear();
		// add to widget
		w->addTopLevelItem(witem);

		// Original Data
		if (vdata->originalDataType == OD_TYPE_MESH_MODEL) {
			strs.append(tr("Original Data"));
			strs.append(tr("Mesh"));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);
		}

		// Distance Field
		if (vdata->df != NULL) {
			df = vdata->df;

			// DF Enable
			strs.append(tr("DF Enable"));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);
			// add widget to tree
			distfCheckbox = new QCheckBox();
			distfCheckbox->setChecked(vdata->isEnableDF);
			connect(this->distfCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeDFEnable(int)));
			w->setItemWidget(witem, 1, distfCheckbox);

			// DF Type
			strs.append(tr("DF Type"));
			if (df->type == DT_CHESSBOARD)
				strs.append(tr("Chessboard 3X3X3"));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);

			// DF Resolution
			strs.append(tr("DF Resolution"));
			str = QString("%1 x %2 x %3").arg(df->resolution[0]).arg(df->resolution[1]).arg(df->resolution[2]);
			strs.append(str);
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);

			// DF VoxelSize
			strs.append(tr("DF Size"));
			str = QString("%1 x %2 x %3").arg(df->voxelSize[0]).arg(df->voxelSize[1]).arg(df->voxelSize[2]);
			strs.append(str);
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);

			// DF Bound
			strs.append(tr("DF Bound"));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			strs.append(tr("Left"));
			strs.append(QString::number(df->bound[DF_LEFT]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Right"));
			strs.append(QString::number(df->bound[DF_RIGHT]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Bottom"));
			strs.append(QString::number(df->bound[DF_BOTTOM]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Top"));
			strs.append(QString::number(df->bound[DF_TOP]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Back"));
			strs.append(QString::number(df->bound[DF_BACK]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Front"));
			strs.append(QString::number(df->bound[DF_FRONT]));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			// add to widget
			w->addTopLevelItem(witem);

			// DF Mininum Value
			strs.append(tr("DF Min Value"));
			strs.append(QString::number(df->minValue));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);

			// DF Maxinum Value
			strs.append(tr("DF Max Value"));
			strs.append(QString::number(df->maxValue));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			// add to widget
			w->addTopLevelItem(witem);

			// Thickness
			strs.append(tr("Thickness"));
			witem = new QTreeWidgetItem(strs);
			strs.clear();
			strs.append(tr("Inner"));
			strs.append(QString::number(df->innerThick));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			strs.append(tr("Outer"));
			strs.append(QString::number(df->outerThick));
			witem2 = new QTreeWidgetItem(strs);
			strs.clear();
			witem->addChild(witem2);
			// add to widget
			w->addTopLevelItem(witem);
		}
	}
	w->update();
}

void VoxelAttributeWidget::updateAttribute(QTreeWidgetItem * it,int i)
{
	QTreeWidget *w = this->treeWidget;
	QTreeWidgetItem *witem, *witem2;
	QString str;
	QStringList strs;
	VoxelData *obj = ((VoxelDataListItem*)it)->it->vdata;

	w->clear();

	// Voxel Name
	strs.append(tr("Name"));
	strs.append(obj->name);
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	w->addTopLevelItem(witem);
	
	// Voxel BBox
	strs.append(tr("Bounding Box"));
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	strs.append(tr("Max"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->max[0]).arg(obj->bbox->max[1]).arg(obj->bbox->max[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	strs.append(tr("Min"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->min[0]).arg(obj->bbox->min[1]).arg(obj->bbox->min[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	w->addTopLevelItem(witem);

	// Voxel Resolution
	strs.append(tr("Voxel Resolution"));
	str = QString("%1 x %2 x %3").arg(obj->resolution[0]).arg(obj->resolution[1]).arg(obj->resolution[2]);
	strs.append(str);
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	// add to widget
	w->addTopLevelItem(witem);

	// Voxel Size
	strs.append(tr("Voxel Size"));
	str = QString("%1 x %2 x %3").arg(obj->voxelSize[0]).arg(obj->voxelSize[1]).arg(obj->voxelSize[2]);
	strs.append(str);
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	// add to widget
	w->addTopLevelItem(witem);


	w->update();
}

void VoxelAttributeWidget::clearVoxelObjectAttributePanel(void)
{
	treeWidget->clear();
}

void VoxelAttributeWidget::changeDFEnable(int state)
{
	emit(stateChanged(state));
}
