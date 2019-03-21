#include "voxelthicknesswidget.hpp"
#include <QMessageBox>

VoxelThicknessWidget::VoxelThicknessWidget(QWidget * parent) : QWidget(parent)
{
	ui.setupUi(this);

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	selectedItem = NULL;
	items = NULL;

	// connect
	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));
	connect(ui.btmbox_widget, SIGNAL(accepted(void)), this, SLOT(confirm(void)));
	connect(ui.btmbox_widget, SIGNAL(rejected(void)), this, SLOT(cancel(void)));
	connect(ui.cbb_voxel, SIGNAL(activated(int)), this, SLOT(setSelectedItem(int)));
}

VoxelThicknessWidget::~VoxelThicknessWidget()
{
	
}

void VoxelThicknessWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(ui.titleBar->rect().adjusted(1, 1, 15, 0), ui.titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void VoxelThicknessWidget::setItems(VoxelItem *items)
{
	QStringList names;
	VoxelItem *item;

	this->items = items;

	item = firstVoxelItem(items);
	while (!isAnchorVoxelItem(item))
	{
		if (item->vdata != NULL && item->vdata->df != NULL) {
			if (selectedItem == NULL)
				selectedItem = item;

			names.append(item->vdata->name);
		}
		item = nextVoxelItem(item);
	}

	ui.cbb_voxel->clear();
	ui.cbb_voxel->addItems(names);

	if (selectedItem != NULL) {
		showVoxelInfo(selectedItem->vdata);
	}
		
}

void VoxelThicknessWidget::moveWidget(int x, int y)
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

//
//		Slots
//

void VoxelThicknessWidget::closeWidget(void)
{
	this->close();
}

void VoxelThicknessWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void VoxelThicknessWidget::dragDone(int x, int y)
{
	//moveWidget(x, y);
}

void VoxelThicknessWidget::confirm(void)
{
	float minValue, maxValue;


	minValue = -ui.le_innerThickness->text().toFloat();
	maxValue = ui.le_outerThickness->text().toFloat();

	// close
	this->hide();

	emit thickDone(selectedItem, minValue, maxValue);
}

void VoxelThicknessWidget::cancel(void)
{
	this->hide();
}

void VoxelThicknessWidget::setSelectedItem(int index)
{
	int i;
	VoxelItem *item;

	item = firstVoxelItem(items);
	for (i = 0; i < index; ++i)
	{
		item = nextVoxelItem(item);
	}
	selectedItem = item;

	showVoxelInfo(selectedItem->vdata);
}

void VoxelThicknessWidget::showVoxelInfo(VoxelData *vdata)
{
	DistanceField *df;
	df = vdata->df;

	if (df->type == DT_CHESSBOARD) {
		ui.le_dfType->setText(tr("Chessbord 3X3X3"));
	}
	else if (df->type == DT_EUCLIDEAN) {
		ui.le_dfType->setText(tr("Euclidean 3X3X3"));
	}
	ui.le_dfMinValue->setText(QString::number(df->minValue));
	ui.le_dfMaxValue->setText(QString::number(df->maxValue));
}
