﻿#include "voxelizewidget.hpp"
#include <QMessageBox>
#include <QFileDialog>

#define X 0
#define Y 1
#define Z 2

VoxelizeWidget::VoxelizeWidget(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);

	// initial variables
	voxSize[X] = 0.4;
	voxSize[Y] = 0.4;
	voxSize[Z] = 0.4;
	isSolid = false;

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));
	connect(ui.btmbox_widget, SIGNAL(accepted(void)), this, SLOT(confirm(void)));
	connect(ui.btmbox_widget, SIGNAL(rejected(void)), this, SLOT(cancel(void)));
	connect(ui.le_resolutionX, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelResolution(void)));
	connect(ui.le_resolutionY, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelResolution(void)));
	connect(ui.le_resolutionZ, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelResolution(void)));
	connect(ui.le_voxelSizeX, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelSize(void)));
	connect(ui.le_voxelSizeY, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelSize(void)));
	connect(ui.le_voxelSizeZ, SIGNAL(editingFinished(void)), this, SLOT(changeVoxelSize(void)));
	connect(ui.ckb_solidModel, SIGNAL(stateChanged(int)), this, SLOT(changeSolid(int)));
}

VoxelizeWidget::~VoxelizeWidget() 
{
	
}

void VoxelizeWidget::setModelScene(MeshModelScene *scene)
{
	if (isEmptyModelItem(scene->items))
		return;

	this->scene = scene;
	modelVoxelizeInfo(scene->root, voxSize, voxRes);
	ui.le_voxelSizeX->setText(QString::number(voxSize[X]));
	ui.le_voxelSizeY->setText(QString::number(voxSize[Y]));
	ui.le_voxelSizeZ->setText(QString::number(voxSize[Z]));
	ui.le_resolutionX->setText(QString::number(voxRes[X]));
	ui.le_resolutionY->setText(QString::number(voxRes[Y]));
	ui.le_resolutionZ->setText(QString::number(voxRes[Z]));
}

void VoxelizeWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(ui.titleBar->rect().adjusted(1, 1, 15, 0), ui.titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void VoxelizeWidget::moveWidget(int x, int y)
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

void VoxelizeWidget::closeWidget(void)
{
	this->close();
}

void VoxelizeWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void VoxelizeWidget::dragDone(int x, int y)
{
	//moveWidget(x, y);
}

void VoxelizeWidget::confirm(void)
{
	/*
	QString filename;
	const char *str;
	const char *fe;
	VoxelModel *vmodel;
	int intRes[3];

	intRes[X] = (int)voxRes[X];
	intRes[Y] = (int)voxRes[Y];
	intRes[Z] = (int)voxRes[Z];

	if (scene != NULL)
		vmodel = modelSceneVoxelize(scene, voxSize, intRes, isSolid);
	else
		vmodel = NULL;

	if (vmodel != NULL) {
		filename = QFileDialog::getSaveFileName(
			this,
			tr("Save File"),
			"untitled",
			tr("Model Files (*.vm);; Data Files (*.vdat)"));
		QByteArray ba = filename.toLatin1();
		str = ba.data();
		fe = strrchr(str, '.');

		if (!filename.isEmpty()) {
			if (!strcmp(fe, ".vm")) {
				//export vmodel

				FILE* vmodelfile = fopen(str, "w");

				//Write vmodel 
				fprintf(vmodelfile, "Voxel Model:\n");
				fprintf(vmodelfile, "Name:%s\n", vmodel->name);
				fprintf(vmodelfile, "Resolution:%dx%dx%d\n", vmodel->resolution[0], vmodel->resolution[1], vmodel->resolution[2]);
				fprintf(vmodelfile, "Voxelsize:%.2f:%.2f:%.2f\n", vmodel->voxelSize[0], vmodel->voxelSize[1], vmodel->voxelSize[2]);
				fprintf(vmodelfile, "Number of voxel data:%d\n", vmodel->numberOfVoxelData);

				//DFS Write Voxel Object
				writeVO(str, vmodelfile, vmodel->root);

				//
				fclose(vmodelfile);
				return;
			}
			else if (!strcmp(fe, ".vdat")) {
				//export vdata


			}
		}
		else {
			QMessageBox::information(this, tr("Warning"), "Failed to save the file.");
		}
	}
	else {
		QMessageBox::information(this, tr("Warning"), "There is no data to export.");
	}
	// close
	this->hide();
	*/
}

void VoxelizeWidget::cancel(void)
{
	this->hide();
}

void VoxelizeWidget::changeVoxelSize(void)
{
	voxSize[X] = ui.le_voxelSizeX->text().toFloat();
	voxSize[Y] = ui.le_voxelSizeY->text().toFloat();
	voxSize[Z] = ui.le_voxelSizeZ->text().toFloat();
	modelVoxelizeInfo(scene->root, voxSize, voxRes);
	ui.le_resolutionX->setText(QString::number(voxRes[X]));
	ui.le_resolutionY->setText(QString::number(voxRes[Y]));
	ui.le_resolutionZ->setText(QString::number(voxRes[Z]));
}

void VoxelizeWidget::changeVoxelResolution(void)
{
	voxRes[X] = ui.le_resolutionX->text().toFloat();
	voxRes[Y] = ui.le_resolutionY->text().toFloat();
	voxRes[Z] = ui.le_resolutionZ->text().toFloat();
	modelVoxelizeInfo(scene->root, voxRes, voxSize);
	ui.le_voxelSizeX->setText(QString::number(voxSize[X]));
	ui.le_voxelSizeY->setText(QString::number(voxSize[Y]));
	ui.le_voxelSizeZ->setText(QString::number(voxSize[Z]));
}

void VoxelizeWidget::changeSolid(int state)
{
	isSolid = state;
}