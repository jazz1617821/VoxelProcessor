#include "voxelclipplanewidget.hpp"

VoxelClipPlaneWidget::VoxelClipPlaneWidget(QWidget * parent) : QWidget(parent) 
{
	ui.setupUi(this);

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	selectedItem = NULL;
	items = NULL;

	// connect
	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));
	connect(ui.spb_xClip, SIGNAL(valueChanged(int)), this, SLOT(setXClipSlider(int)));
	connect(ui.spb_invXClip, SIGNAL(valueChanged(int)), this, SLOT(setInvXClipSlider(int)));
	connect(ui.spb_yClip, SIGNAL(valueChanged(int)), this, SLOT(setYClipSlider(int)));
	connect(ui.spb_invYClip, SIGNAL(valueChanged(int)), this, SLOT(setInvYClipSlider(int)));
	connect(ui.spb_zClip, SIGNAL(valueChanged(int)), this, SLOT(setZClipSlider(int)));
	connect(ui.spb_invZClip, SIGNAL(valueChanged(int)), this, SLOT(setInvZClipSlider(int)));
	connect(ui.hsld_xClip, SIGNAL(valueChanged(int)), this, SLOT(setXClipSpinBox(int)));
	connect(ui.hsld_invXClip, SIGNAL(valueChanged(int)), this, SLOT(setInvXClipSpinBox(int)));
	connect(ui.hsld_yClip, SIGNAL(valueChanged(int)), this, SLOT(setYClipSpinBox(int)));
	connect(ui.hsld_invYClip, SIGNAL(valueChanged(int)), this, SLOT(setInvYClipSpinBox(int)));
	connect(ui.hsld_zClip, SIGNAL(valueChanged(int)), this, SLOT(setZClipSpinBox(int)));
	connect(ui.hsld_invZClip, SIGNAL(valueChanged(int)), this, SLOT(setInvZClipSpinBox(int)));
	connect(ui.ckb_xClip, SIGNAL(stateChanged(int)), this, SLOT(updateXClip(int)));
	connect(ui.ckb_invXClip, SIGNAL(stateChanged(int)), this, SLOT(updateInvXClip(int)));
	connect(ui.ckb_yClip, SIGNAL(stateChanged(int)), this, SLOT(updateYClip(int)));
	connect(ui.ckb_invYClip, SIGNAL(stateChanged(int)), this, SLOT(updateInvYClip(int)));
	connect(ui.ckb_zClip, SIGNAL(stateChanged(int)), this, SLOT(updateZClip(int)));
	connect(ui.ckb_invZClip, SIGNAL(stateChanged(int)), this, SLOT(updateInvZClip(int)));
	connect(ui.cbb_voxel, SIGNAL(activated(int)), this, SLOT(setSelectedItem(int)));
}

VoxelClipPlaneWidget::~VoxelClipPlaneWidget() 
{
	
}


void VoxelClipPlaneWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(ui.titleBar->rect().adjusted(1, 1, 15, 0), ui.titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void VoxelClipPlaneWidget::showClipInfo(VoxelData *vdata)
{
	VBO *vbo;
	DistanceField *df;

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		vbo = df->vbo;
		// clip variables
		// x clip spin box
		ui.spb_xClip->setValue(df->clip[CPP_X]);
		ui.spb_invXClip->setValue(df->clip[CPP_INV_X]);
		// y clip spin box
		ui.spb_yClip->setValue(df->clip[CPP_Y]);
		ui.spb_invYClip->setValue(df->clip[CPP_INV_Y]);
		// z clip spin box
		ui.spb_zClip->setValue(df->clip[CPP_Z]);
		ui.spb_invZClip->setValue(df->clip[CPP_INV_Z]);
		// x clip slider
		ui.hsld_xClip->setMaximum(df->resolution[0]);
		ui.hsld_xClip->setValue((int)df->clip[CPP_X]);
		ui.hsld_invXClip->setMaximum(df->resolution[0]);
		ui.hsld_invXClip->setValue((int)df->clip[CPP_INV_X]);
		// y clip slider
		ui.hsld_yClip->setMaximum(df->resolution[1]);
		ui.hsld_yClip->setValue((int)df->clip[CPP_Y]);
		ui.hsld_invYClip->setMaximum(df->resolution[1]);
		ui.hsld_invYClip->setValue((int)df->clip[CPP_INV_Y]);
		// z clip slider
		ui.hsld_zClip->setMaximum(df->resolution[2]);
		ui.hsld_zClip->setValue((int)df->clip[CPP_Z]);
		ui.hsld_invZClip->setMaximum(df->resolution[2]);
		ui.hsld_invZClip->setValue((int)df->clip[CPP_INV_Z]);
		// x clip check box
		ui.ckb_xClip->setChecked(vbo->clip[CPP_X]->isEnable);
		ui.ckb_invXClip->setChecked(vbo->clip[CPP_INV_X]->isEnable);
		// y clip check box
		ui.ckb_yClip->setChecked(vbo->clip[CPP_Y]->isEnable);
		ui.ckb_invYClip->setChecked(vbo->clip[CPP_INV_Y]->isEnable);
		// z clip check box
		ui.ckb_zClip->setChecked(vbo->clip[CPP_Z]->isEnable);
		ui.ckb_invZClip->setChecked(vbo->clip[CPP_INV_Z]->isEnable);
	}
	else {
		vbo = vdata->vbo;
		// clip variables
		// x clip spin box
		ui.spb_xClip->setValue(vdata->clip[CPP_X]);
		ui.spb_invXClip->setValue(vdata->clip[CPP_INV_X]);
		// y clip spin box
		ui.spb_yClip->setValue(vdata->clip[CPP_Y]);
		ui.spb_invYClip->setValue(vdata->clip[CPP_INV_Y]);
		// z clip spin box
		ui.spb_zClip->setValue(vdata->clip[CPP_Z]);
		ui.spb_invZClip->setValue(vdata->clip[CPP_INV_Z]);
		// x clip slider
		ui.hsld_xClip->setMaximum(vdata->resolution[0]);
		ui.hsld_xClip->setValue((int)vdata->clip[CPP_X]);
		ui.hsld_invXClip->setMaximum(vdata->resolution[0]);
		ui.hsld_invXClip->setValue((int)vdata->clip[CPP_INV_X]);
		// y clip slider
		ui.hsld_yClip->setMaximum(vdata->resolution[1]);
		ui.hsld_yClip->setValue((int)vdata->clip[CPP_Y]);
		ui.hsld_invYClip->setMaximum(vdata->resolution[1]);
		ui.hsld_invYClip->setValue((int)vdata->clip[CPP_INV_Y]);
		// z clip slider
		ui.hsld_zClip->setMaximum(vdata->resolution[2]);
		ui.hsld_zClip->setValue((int)vdata->clip[CPP_Z]);
		ui.hsld_invZClip->setMaximum(vdata->resolution[2]);
		ui.hsld_invZClip->setValue((int)vdata->clip[CPP_INV_Z]);
		// x clip check box
		ui.ckb_xClip->setChecked(vbo->clip[CPP_X]->isEnable);
		ui.ckb_invXClip->setChecked(vbo->clip[CPP_INV_X]->isEnable);
		// y clip check box
		ui.ckb_yClip->setChecked(vbo->clip[CPP_Y]->isEnable);
		ui.ckb_invYClip->setChecked(vbo->clip[CPP_INV_Y]->isEnable);
		// z clip check box
		ui.ckb_zClip->setChecked(vbo->clip[CPP_Z]->isEnable);
		ui.ckb_invZClip->setChecked(vbo->clip[CPP_INV_Z]->isEnable);
	}
}

void VoxelClipPlaneWidget::setItems(VoxelItem *items)
{
	QStringList names;
	VoxelItem *item;

	this->items = items;

	item = firstVoxelItem(items);
	selectedItem = item;
	while (!isAnchorVoxelItem(item))
	{
		names.append(item->vdata->name);
		item = nextVoxelItem(item);
	}
	
	// voxel combo box
	ui.cbb_voxel->clear();
	ui.cbb_voxel->addItems(names);

	// show clip data
	showClipInfo(selectedItem->vdata);
}

void VoxelClipPlaneWidget::setCurrentItem(VoxelItem *selectedItem)
{
	this->selectedItem = selectedItem;
}

void VoxelClipPlaneWidget::moveWidget(int x, int y)
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

void VoxelClipPlaneWidget::closeWidget(void)
{
	this->close();
}

void VoxelClipPlaneWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void VoxelClipPlaneWidget::dragDone(int x, int y)
{
	//moveWidget(x, y);
}

void VoxelClipPlaneWidget::setSelectedItem(int index)
{
	int i;
	VoxelItem *item;

	item = firstVoxelItem(items);
	for (i = 0; i < index; ++i)
	{
		item = nextVoxelItem(item);
	}
	selectedItem = item;

	showClipInfo(item->vdata);

	emit itemSelected(item);
}


void VoxelClipPlaneWidget::setXClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.hsld_invXClip->value()) {
		ui.hsld_invXClip->setValue(value);
		ui.spb_invXClip->setValue(value);
	}
	ui.hsld_xClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_X] = value;
		df->vbo->clip[CPP_X]->plane[3] = -(df->bbox->min[0] + value * df->voxelSize[0]);
	}
	else {
		vdata->clip[CPP_X] = value;
		vdata->vbo->clip[CPP_X]->plane[3] = -(vdata->bbox->min[0] + value * vdata->voxelSize[0]);
	}

	emit updateVoxelClip(CPP_X, selectedItem);
}

void VoxelClipPlaneWidget::setInvXClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value < ui.hsld_xClip->value()) {
		ui.hsld_xClip->setValue(value);
		ui.spb_xClip->setValue(value);
	}
	ui.hsld_invXClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_X] = value;
		df->vbo->clip[CPP_INV_X]->plane[3] = df->bbox->min[0] + value * df->voxelSize[0];
	}
	else {
		vdata->clip[CPP_INV_X] = value;
		vdata->vbo->clip[CPP_INV_X]->plane[3] = vdata->bbox->min[0] + value * vdata->voxelSize[0];
	}

	emit updateVoxelClip(CPP_INV_X, selectedItem);
}

void VoxelClipPlaneWidget::setYClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.hsld_invYClip->value()) {
		ui.hsld_invYClip->setValue(value);
		ui.spb_invYClip->setValue(value);
	}
	ui.hsld_yClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_Y] = value;
		df->vbo->clip[CPP_Y]->plane[3] = -(df->bbox->min[1] + value * df->voxelSize[1]);
	}
	else {
		vdata->clip[CPP_Y] = value;
		vdata->vbo->clip[CPP_Y]->plane[3] = -(vdata->bbox->min[1] + value * vdata->voxelSize[1]);
	}

	emit updateVoxelClip(CPP_Y, selectedItem);
}

void VoxelClipPlaneWidget::setInvYClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value < ui.hsld_yClip->value()) {
		ui.hsld_yClip->setValue(value);
		ui.spb_yClip->setValue(value);
	}
	ui.hsld_invYClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_Y] = value;
		df->vbo->clip[CPP_INV_Y]->plane[3] = df->bbox->min[1] + value * df->voxelSize[1];
	}
	else {
		vdata->clip[CPP_INV_Y] = value;
		vdata->vbo->clip[CPP_INV_Y]->plane[3] = vdata->bbox->min[1] + value * vdata->voxelSize[1];
	}

	emit updateVoxelClip(CPP_INV_Y, selectedItem);
}

void VoxelClipPlaneWidget::setZClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.hsld_invZClip->value()) {
		ui.hsld_invZClip->setValue(value);
		ui.spb_invZClip->setValue(value);
	}
	ui.hsld_zClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_Z] = value;
		df->vbo->clip[CPP_Z]->plane[3] = -(df->bbox->min[2] + value * df->voxelSize[2]);
	}
	else {
		vdata->clip[CPP_Z] = value;
		vdata->vbo->clip[CPP_Z]->plane[3] = -(vdata->bbox->min[2] + value * vdata->voxelSize[2]);
	}

	emit updateVoxelClip(CPP_Z, selectedItem);
}

void VoxelClipPlaneWidget::setInvZClipSlider(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value < ui.hsld_zClip->value()) {
		ui.hsld_zClip->setValue(value);
		ui.spb_zClip->setValue(value);
	}
	ui.hsld_invZClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_Z] = value;
		df->vbo->clip[CPP_INV_Z]->plane[3] = df->bbox->min[2] + value * df->voxelSize[2];
	}
	else {
		vdata->clip[CPP_INV_Z] = value;
		vdata->vbo->clip[CPP_INV_Z]->plane[3] = vdata->bbox->min[2] + value * vdata->voxelSize[2];
	}

	emit updateVoxelClip(CPP_INV_Z, selectedItem);
}

void VoxelClipPlaneWidget::setXClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_invXClip->value()) {
		ui.hsld_invXClip->setValue(value);
		ui.spb_invXClip->setValue(value);
	}
	ui.spb_xClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_X] = value;
		df->vbo->clip[CPP_X]->plane[3] = -(df->bbox->min[0] + value * df->voxelSize[0]);
	}
	else {
		vdata->clip[CPP_X] = value;
		vdata->vbo->clip[CPP_X]->plane[3] = -(vdata->bbox->min[0] + value * vdata->voxelSize[0]);
	}

	emit updateVoxelClip(CPP_X, selectedItem);
}

void VoxelClipPlaneWidget::setInvXClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_xClip->value()) {
		ui.hsld_invXClip->setValue(value);
		ui.spb_invXClip->setValue(value);
	}
	ui.spb_invXClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_X] = value;
		df->vbo->clip[CPP_INV_X]->plane[3] = df->bbox->min[0] + value * df->voxelSize[0];
	}
	else {
		vdata->clip[CPP_INV_X] = value;
		vdata->vbo->clip[CPP_INV_X]->plane[3] = vdata->bbox->min[0] + value * vdata->voxelSize[0];
	}

	emit updateVoxelClip(CPP_INV_X, selectedItem);
}

void VoxelClipPlaneWidget::setYClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_invYClip->value()) {
		ui.hsld_invYClip->setValue(value);
		ui.spb_invYClip->setValue(value);
	}
	ui.spb_yClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_Y] = value;
		df->vbo->clip[CPP_Y]->plane[3] = -(df->bbox->min[1] + value * df->voxelSize[1]);
	}
	else {
		vdata->clip[CPP_Y] = value;
		vdata->vbo->clip[CPP_Y]->plane[3] = -(vdata->bbox->min[1] + value * vdata->voxelSize[1]);
	}

	emit updateVoxelClip(CPP_Y, selectedItem);
}

void VoxelClipPlaneWidget::setInvYClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_yClip->value()) {
		ui.hsld_invYClip->setValue(value);
		ui.spb_invYClip->setValue(value);
	}
	ui.spb_invYClip->setValue(value);
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_Y] = value;
		df->vbo->clip[CPP_INV_Y]->plane[3] = df->bbox->min[1] + value * df->voxelSize[1];
	}
	else {
		vdata->clip[CPP_INV_Y] = value;
		vdata->vbo->clip[CPP_INV_Y]->plane[3] = vdata->bbox->min[1] + value * vdata->voxelSize[1];
	}

	emit updateVoxelClip(CPP_INV_Y, selectedItem);
}

void VoxelClipPlaneWidget::setZClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_invZClip->value()) {
		ui.hsld_invZClip->setValue(value);
		ui.spb_invZClip->setValue(value);
	}
	ui.spb_zClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_Z] = value;
		df->vbo->clip[CPP_Z]->plane[3] = -(df->bbox->min[2] + value * df->voxelSize[2]);
	}
	else {
		vdata->clip[CPP_Z] = value;
		vdata->vbo->clip[CPP_Z]->plane[3] = -(vdata->bbox->min[2] + value * vdata->voxelSize[2]);
	}

	emit updateVoxelClip(CPP_Z, selectedItem);
}

void VoxelClipPlaneWidget::setInvZClipSpinBox(int value)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (value > ui.spb_zClip->value()) {
		ui.hsld_invZClip->setValue(value);
		ui.spb_invZClip->setValue(value);
	}
	ui.spb_invZClip->setValue(value);

	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->clip[CPP_INV_Z] = value;
		df->vbo->clip[CPP_INV_Z]->plane[3] = df->bbox->min[2] + value * df->voxelSize[2];
	}
	else {
		vdata->clip[CPP_INV_Z] = value;
		vdata->vbo->clip[CPP_INV_Z]->plane[3] = vdata->bbox->min[2] + value * vdata->voxelSize[2];
	}

	emit updateVoxelClip(CPP_INV_Z, selectedItem);
}

void VoxelClipPlaneWidget::updateXClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_X]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_X]->isEnable = state;
	}
	emit updateVoxelClip(CPP_X, selectedItem);
}

void VoxelClipPlaneWidget::updateInvXClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_INV_X]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_INV_X]->isEnable = state;
	}
	emit updateVoxelClip(CPP_INV_X, selectedItem);
}

void VoxelClipPlaneWidget::updateYClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_Y]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_Y]->isEnable = state;
	}
	emit updateVoxelClip(CPP_Y, selectedItem);
}

void VoxelClipPlaneWidget::updateInvYClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_INV_Y]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_INV_Y]->isEnable = state;
	}
	emit updateVoxelClip(CPP_INV_Y, selectedItem);
}

void VoxelClipPlaneWidget::updateZClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_Z]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_Z]->isEnable = state;
	}
	emit updateVoxelClip(CPP_Z, selectedItem);
}

void VoxelClipPlaneWidget::updateInvZClip(int state)
{
	VoxelData *vdata;
	DistanceField *df;

	vdata = selectedItem->vdata;
	if (vdata->df != NULL && vdata->isEnableDF) {
		df = vdata->df;
		df->vbo->clip[CPP_INV_Z]->isEnable = state;
	}
	else {
		vdata->vbo->clip[CPP_INV_Z]->isEnable = state;
	}
	emit updateVoxelClip(CPP_INV_Z, selectedItem);
}
