#include "voxeldistancefieldwidget.hpp"
#include <QMessageBox>

VoxelDistanceFieldWidget::VoxelDistanceFieldWidget(QWidget *parent) : QWidget(parent) 
{
	QStringList dfTypes;

	ui.setupUi(this);

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	// set DF type
	dfTypes.append(tr("Chessboard 3X3X3"));
	dfTypes.append(tr("Euclidean 3X3X3"));
	ui.cbb_df->addItems(dfTypes);


	selectedItem = NULL;
	items = NULL;

	// flags
	dfChnaged = false;

	// connect
	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));
	connect(ui.btmbox_widget, SIGNAL(accepted(void)), this, SLOT(confirm(void)));
	connect(ui.btmbox_widget, SIGNAL(rejected(void)), this, SLOT(cancel(void)));
	connect(ui.cbb_voxel, SIGNAL(activated(int)), this, SLOT(setSelectedItem(int)));
	
}

VoxelDistanceFieldWidget::~VoxelDistanceFieldWidget() 
{
	
}

void VoxelDistanceFieldWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(ui.titleBar->rect().adjusted(1, 1, 15, 0), ui.titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void VoxelDistanceFieldWidget::setItems(VoxelItem *items)
{
	QStringList names;
	VoxelItem *item;

	this->items = items;

	item = firstVoxelItem(items);
	selectedItem = !isAnchorVoxelItem(item) ? item : NULL;
	while (!isAnchorVoxelItem(item))
	{
		names.append(item->vdata->name);
		item = nextVoxelItem(item);
	}

	ui.cbb_voxel->clear();
	ui.cbb_voxel->addItems(names);

	if (selectedItem != NULL) {
		showVoxelInfo(selectedItem->vdata);
	}
}

void VoxelDistanceFieldWidget::showVoxelInfo(VoxelData *vdata)
{
	ui.le_voxelResolutionX->setText(QString::number(vdata->resolution[0]));
	ui.le_voxelResolutionY->setText(QString::number(vdata->resolution[1]));
	ui.le_voxelResolutionZ->setText(QString::number(vdata->resolution[2]));
	ui.le_voxelSizeX->setText(QString::number(vdata->voxelSize[0]));
	ui.le_voxelSizeY->setText(QString::number(vdata->voxelSize[1]));
	ui.le_voxelSizeZ->setText(QString::number(vdata->voxelSize[2]));

	if (vdata->df == NULL) {
		ui.le_dfResolutionX->setText(QString::number(vdata->resolution[0] + 10));
		ui.le_dfResolutionY->setText(QString::number(vdata->resolution[1] + 10));
		ui.le_dfResolutionZ->setText(QString::number(vdata->resolution[2] + 10));
		ui.le_dfBoundLeft->setText("5");
		ui.le_dfBoundRight->setText("5");
		ui.le_dfBoundBottom->setText("5");
		ui.le_dfBoundTop->setText("5");
		ui.le_dfBoundBack->setText("5");
		ui.le_dfBoundFront->setText("5");
	}
	else {
		ui.le_dfResolutionX->setText(QString::number(vdata->resolution[0]));
		ui.le_dfResolutionY->setText(QString::number(vdata->resolution[1]));
		ui.le_dfResolutionZ->setText(QString::number(vdata->resolution[2]));
		ui.le_dfBoundLeft->setText(QString::number(vdata->df->bound[DF_LEFT]));
		ui.le_dfBoundRight->setText(QString::number(vdata->df->bound[DF_RIGHT]));
		ui.le_dfBoundBottom->setText(QString::number(vdata->df->bound[DF_BOTTOM]));
		ui.le_dfBoundTop->setText(QString::number(vdata->df->bound[DF_TOP]));
		ui.le_dfBoundBack->setText(QString::number(vdata->df->bound[DF_BACK]));
		ui.le_dfBoundFront->setText(QString::number(vdata->df->bound[DF_FRONT]));
	}
}

void VoxelDistanceFieldWidget::moveWidget(int x, int y)
{
	int posX, posY;

	posX = this->x() + x;
	posY = this->y() + y;

	QRect boundary = this->parentWidget()->rect();

	if (posX < boundary.topLeft().x()) {
		posX = boundary.topLeft().x();
	}
	if (posY < boundary.topLeft().y()) {
		posY = boundary.topLeft().y();
	}
	if (posX > boundary.bottomRight().x() - ui.titleBar->width()) {
		posX = boundary.bottomRight().x() - ui.titleBar->width();
	}
	if (posY > boundary.bottomRight().y() - ui.titleBar->height()) {
		posY = boundary.bottomRight().y() - ui.titleBar->height();
	}

	move(posX, posY);
}

int VoxelDistanceFieldWidget::checkValues(void)
{
	if (ui.cbb_voxel->currentText().isEmpty()) {
		QMessageBox::warning(this, tr("Warnning"), tr("Please select a voxel data..."), QMessageBox::Ok);
		return -1;
	}
}

//
//		Slots
//

void VoxelDistanceFieldWidget::closeWidget(void)
{
	this->close();
}

void VoxelDistanceFieldWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void VoxelDistanceFieldWidget::dragDone(int x, int y)
{
	//moveWidget(x, y);
}

void VoxelDistanceFieldWidget::confirm(void)
{
	QString dfType;
	int type, res[3], bound[6];

	if (checkValues() < 0)
		return;

	dfType = ui.cbb_df->currentText();

	// set variables
	if (dfType.compare("Chessboard 3X3X3") == 0) {
		type = DT_CHESSBOARD;
	}
	else if (dfType.compare("Euclidean 3X3X3") == 0) {
		type = DT_EUCLIDEAN;
	}
	res[0] = ui.le_dfResolutionX->text().toInt();
	res[1] = ui.le_dfResolutionY->text().toInt();
	res[2] = ui.le_dfResolutionZ->text().toInt();
	bound[0] = ui.le_dfBoundLeft->text().toInt();
	bound[1] = ui.le_dfBoundRight->text().toInt();
	bound[2] = ui.le_dfBoundBottom->text().toInt();
	bound[3] = ui.le_dfBoundTop->text().toInt();
	bound[4] = ui.le_dfBoundBack->text().toInt();
	bound[5] = ui.le_dfBoundFront->text().toInt();

	if (selectedItem->vdata->df == NULL) {
		selectedItem->vdata->df = newDistanceField(res,
			selectedItem->vdata->voxelSize, bound, NULL);
	}
	else {
		selectedItem->vdata->df = newDistanceField(res,
			selectedItem->vdata->voxelSize, bound, 
			selectedItem->vdata->df);
	}
	CDT(selectedItem->vdata->df, selectedItem->parent, type);
	// close
	this->hide();

	emit dfDone(selectedItem);
}

void VoxelDistanceFieldWidget::cancel(void)
{
	this->hide();
}

void VoxelDistanceFieldWidget::setSelectedItem(int index)
{
	int i;
	VoxelItem *item;

	item = firstVoxelItem(items);
	for (i = 0; i < index; ++i)
	{
		item = nextVoxelItem(item);
	}
	selectedItem = item;

	showVoxelInfo(item->vdata);
}
