#include "templatewidget.hpp"

#define X 0
#define Y 1
#define Z 2

#define BORDER 2

TemplateWidget::TemplateWidget(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);

	colorId = 0;

	makewhat = Nothing;

	connect(ui.pb_ok, SIGNAL(clicked(void)), this, SLOT(confirm(void)));
}

TemplateWidget::~TemplateWidget() {
	
}

VoxelData * TemplateWidget::makeVoxelObject()
{
	VoxelData* vdata = NULL;

	switch (makewhat) {
	case Empty:
		break;
	case Ball:
		vdata = newVoxelData();
		makeBall(vdata);
		break;
	case Cube:
		vdata = newVoxelData();
		makeCube(vdata);
		break;
	case Cylinder:

		break;
	default:
		break;
	}

	if (vdata != NULL) {
		//vdata->id = colorId++;
	}

	return vdata;
}

bool TemplateWidget::readyOrNot(void)
{
	if (makewhat != Nothing) {
		return true;
	}
	else {
		return false;
	}
}

void TemplateWidget::reset(void)
{
	ui.le_voxelDataName->setText("");
	ui.le_voxelSize_x->setText("1.0");
	ui.le_voxelSize_y->setText("1.0");
	ui.le_voxelSize_z->setText("1.0");

	makewhat = Nothing;
	return;
}

void TemplateWidget::makeBall(VoxelData * vdata)
{
	memcpy(vdata->name, this->name, 10);
	vdata->resolution[X] = vdata->resolution[Y] = vdata->resolution[Z] = (int)(this->ballPara.radius + BORDER) * 2;

	vdata->voxelSize[X] = this->voxelSize[X];
	vdata->voxelSize[Y] = this->voxelSize[Y];
	vdata->voxelSize[Z] = this->voxelSize[Z];

	vdata->rawData = (Voxel*)calloc(vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z], sizeof(Voxel));

	if (this->ballPara.hollow)
	{								//Hollow
		for (int z = BORDER; z < vdata->resolution[Z] - BORDER; z++) {
			for (int y = BORDER; y < vdata->resolution[Y] - BORDER; y++) {
				for (int x = BORDER; x < vdata->resolution[X] - BORDER; x++) {
					int index = x + y*vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					int position = (x - vdata->resolution[X] / 2)*(x - vdata->resolution[X] / 2) + (y - vdata->resolution[Y] / 2)*(y - vdata->resolution[Y] / 2) + (z - vdata->resolution[Z] / 2)*(z - vdata->resolution[Z] / 2);
					int R = (ballPara.radius*ballPara.radius);
					int r = (ballPara.radius - ballPara.thinkness)*(ballPara.radius - ballPara.thinkness);
					if (position <= R && position >= r) {
						vdata->rawData[index].data = VOX_SURFACE;
					}
				}
			}
		}
	}
	else
	{								//Solid
		for (int z = BORDER; z < vdata->resolution[Z] - BORDER; z++) {
			for (int y = BORDER; y < vdata->resolution[Y] - BORDER; y++) {
				for (int x = BORDER; x < vdata->resolution[X] - BORDER; x++) {
					int index = x + y*vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					int position = (x - vdata->resolution[X] / 2)*(x - vdata->resolution[X] / 2) + (y - vdata->resolution[Y] / 2)*(y - vdata->resolution[Y] / 2) + (z - vdata->resolution[Z] / 2)*(z - vdata->resolution[Z] / 2);
					int R = (ballPara.radius*ballPara.radius);
					if (position <= R) {
						vdata->rawData[index].data = VOX_SOLID;
					}
				}
			}
		}
	}

	vdata->bbox->min[X] = -vdata->resolution[X] * vdata->voxelSize[X] / 2;
	vdata->bbox->min[Y] = -vdata->resolution[Y] * vdata->voxelSize[Y] / 2;
	vdata->bbox->min[Z] = -vdata->resolution[Z] * vdata->voxelSize[Z] / 2;

	vdata->bbox->max[X] = vdata->resolution[X] * vdata->voxelSize[X] / 2;
	vdata->bbox->max[Y] = vdata->resolution[Y] * vdata->voxelSize[Y] / 2;
	vdata->bbox->max[Z] = vdata->resolution[Z] * vdata->voxelSize[Z] / 2;

}

void TemplateWidget::makeCube(VoxelData * vdata)
{
	memcpy(vdata->name, this->name, 10);
	vdata->resolution[X] = (int)(this->cubePara.l + BORDER * 2);
	vdata->resolution[Y] = (int)(this->cubePara.h + BORDER * 2);
	vdata->resolution[Z] = (int)(this->cubePara.w + BORDER * 2);

	vdata->voxelSize[X] = this->voxelSize[X];
	vdata->voxelSize[Y] = this->voxelSize[Y];
	vdata->voxelSize[Z] = this->voxelSize[Z];

	vdata->rawData = (Voxel*)calloc(vdata->resolution[X] * vdata->resolution[Y] * vdata->resolution[Z], sizeof(Voxel));

	if (!this->cubePara.hollow)
	{
		for (int z = BORDER; z < vdata->resolution[Z] - BORDER; z++)
		{
			for (int y = BORDER; y < vdata->resolution[Y] - BORDER; y++)
			{
				for (int x = BORDER; x < vdata->resolution[X] - BORDER; x++)
				{
					int nowIndex = x + y * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					if (x == BORDER || x == vdata->resolution[X] - BORDER)
					{
						vdata->rawData[nowIndex].data = VOX_SURFACE;
					}
					else if (y == BORDER || y == vdata->resolution[Y] - BORDER)
					{
						vdata->rawData[nowIndex].data = VOX_SURFACE;
					}
					else if (z == BORDER || z == vdata->resolution[Z] - BORDER)
					{
						vdata->rawData[nowIndex].data = VOX_SURFACE;
					}
					else {
						vdata->rawData[nowIndex].data = VOX_SOLID;
					}
				}
			}
		}
	}
	else
	{
		for (int z = BORDER; z < vdata->resolution[Z] - BORDER; z++)
		{
			for (int y = BORDER; y < vdata->resolution[Y] - BORDER; y++)
			{
				for (int i = 0; i < this->cubePara.thinkness; i++)
				{
					int frontIndex = (BORDER + i) + y * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					int backIndex = (vdata->resolution[X] - BORDER - 1 - i) + y * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					vdata->rawData[frontIndex].data = vdata->rawData[backIndex].data = VOX_SURFACE;
				}
			}
		}
		for (int y = BORDER; y < vdata->resolution[Y] - BORDER; y++)
		{
			for (int x = BORDER; x < vdata->resolution[X] - BORDER; x++)
			{
				for (int i = 0; i < this->cubePara.thinkness; i++)
				{
					int frontIndex = x + y * vdata->resolution[X] + (BORDER + i) * vdata->resolution[X] * vdata->resolution[Y];
					int backIndex = x + y * vdata->resolution[X] + (vdata->resolution[Z] - BORDER - 1 - i) * vdata->resolution[X] * vdata->resolution[Y];
					vdata->rawData[frontIndex].data = vdata->rawData[backIndex].data = VOX_SURFACE;
				}
			}
		}
		for (int z = BORDER; z < vdata->resolution[Z] - BORDER; z++)
		{
			for (int x = BORDER; x < vdata->resolution[X] - BORDER; x++)
			{
				for (int i = 0; i < this->cubePara.thinkness; i++)
				{
					int frontIndex = x + (BORDER + i) * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					int backIndex = x + (vdata->resolution[Y] - BORDER - 1 - i) * vdata->resolution[X] + z * vdata->resolution[X] * vdata->resolution[Y];
					vdata->rawData[frontIndex].data = vdata->rawData[backIndex].data = VOX_SURFACE;
				}
			}
		}
	}

	vdata->bbox->min[X] = -vdata->resolution[X] * vdata->voxelSize[X] / 2;
	vdata->bbox->min[Y] = -vdata->resolution[Y] * vdata->voxelSize[Y] / 2;
	vdata->bbox->min[Z] = -vdata->resolution[Z] * vdata->voxelSize[Z] / 2;

	vdata->bbox->max[X] = vdata->resolution[X] * vdata->voxelSize[X] / 2;
	vdata->bbox->max[Y] = vdata->resolution[Y] * vdata->voxelSize[Y] / 2;
	vdata->bbox->max[Z] = vdata->resolution[Z] * vdata->voxelSize[Z] / 2;


}

void TemplateWidget::confirm(void)
{
	float voxelSize[3];

	// close
	this->hide();

	if (ui.le_voxelDataName) {
		QString temp = ui.le_voxelDataName->text();
		char *str = (char *)malloc(10);
		QByteArray ba = temp.toLatin1();
		strcpy(str, ba.data());
		memcpy(this->name, str, 10);
	}
	else {
		QMessageBox::information(this, tr("Warning"), "Voxel data Name is empty.");
	}

	voxelSize[X] = ui.le_voxelSize_x->text().toFloat();
	voxelSize[Y] = ui.le_voxelSize_y->text().toFloat();
	voxelSize[Z] = ui.le_voxelSize_z->text().toFloat();

	if (voxelSize[X] && voxelSize[Y] && voxelSize[Z]) {
		this->voxelSize[X] = voxelSize[X];
		this->voxelSize[Y] = voxelSize[Y];
		this->voxelSize[Z] = voxelSize[Z];
	}
	else {
		QMessageBox::information(this, tr("Warning"), "Voxel size is empty.");
	}

	if (ui.rb_empty->isChecked()) {
		
		makewhat = Empty;
	}
	else if (ui.rb_ball->isChecked()) {
		if (!ui.le_radius->text().isEmpty()) {
			ballPara.radius = ui.le_radius->text().toFloat();
			if (ui.cb_ballHollow->isChecked()) {
				ballPara.hollow = ui.cb_ballHollow->isChecked();
				ballPara.thinkness = ui.le_ballThinkness->text().toFloat();
			}
			else {
				ballPara.hollow = false;
			}
			makewhat = Ball;
		}
		else {
			QMessageBox::information(this, tr("Warning"), "Radius is empty.");
		}
	}
	else if (ui.rb_cube->isChecked()) {
		int l, w, h;
		if (!ui.le_length->text().isEmpty() && !ui.le_width->text().isEmpty() && !ui.le_height->text().isEmpty()) {
			cubePara.l = ui.le_length->text().toFloat();
			cubePara.w = ui.le_width->text().toFloat();
			cubePara.h = ui.le_height->text().toFloat();
			if (ui.cb_cubeHollow->isChecked()) {
				cubePara.hollow = ui.cb_cubeHollow->isChecked();
				cubePara.thinkness = ui.le_cubeThinkness->text().toFloat();
			}
			else {
				cubePara.hollow = false;
			}
			makewhat = Cube;
		}
		else {
			QMessageBox::information(this, tr("Warning"), "Side length is empty.");
		}
	}
	else if (ui.rb_cylinder->isChecked()) {

		makewhat = Cylinder;
	}
	else {

		makewhat = Nothing;
	}

	emit(loadParaFinished());
}

void TemplateWidget::on_rb_empty_clicked(bool checked)
{
	if (checked) {
		// set member enable
		// null

		// set the other disable
		ui.rb_ball->setChecked(false);
		ui.le_radius->setEnabled(false);
		ui.cb_ballHollow->setEnabled(false);
		ui.le_ballThinkness->setEnabled(false);

		ui.rb_cube->setChecked(false);
		ui.le_length->setEnabled(false);
		ui.le_width->setEnabled(false);
		ui.le_height->setEnabled(false);
		ui.cb_cubeHollow->setEnabled(false);
		ui.le_cubeThinkness->setEnabled(false);

		ui.rb_cylinder->setChecked(false);

	}else{

	}
}

void TemplateWidget::on_rb_ball_clicked(bool checked)
{
	if (checked) {
		// set member enable
		ui.le_radius->setEnabled(true);
		ui.cb_ballHollow->setEnabled(true);

		// set the other disable
		ui.rb_empty->setChecked(false);

		ui.rb_cube->setChecked(false);
		ui.le_length->setEnabled(false);
		ui.le_width->setEnabled(false);
		ui.le_height->setEnabled(false);
		ui.cb_cubeHollow->setEnabled(false);
		ui.le_cubeThinkness->setEnabled(false);

		ui.rb_cylinder->setChecked(false);
	}
	else {
		ui.le_radius->setEnabled(false);
		ui.cb_ballHollow->setEnabled(false);
	}

}

void TemplateWidget::on_rb_cube_clicked(bool checked)
{
	if (checked) {
		// set member enable
		ui.le_length->setEnabled(true);
		ui.le_width->setEnabled(true);
		ui.le_height->setEnabled(true);
		ui.cb_cubeHollow->setEnabled(true);

		// set the other disable
		ui.rb_empty->setChecked(false);

		ui.rb_ball->setChecked(false);
		ui.le_radius->setEnabled(false);
		ui.cb_ballHollow->setEnabled(false);
		ui.le_ballThinkness->setEnabled(false);

		ui.rb_cylinder->setChecked(false);

	}
	else {
		ui.le_length->setEnabled(false);
		ui.le_width->setEnabled(false);
		ui.le_height->setEnabled(false);
		ui.cb_cubeHollow->setEnabled(false);
	}
}

void TemplateWidget::on_rb_cylinder_clicked(bool checked)
{
	if (checked) {
		// set member enable
		// null

		// set the other disable
		ui.rb_empty->setChecked(false);

		ui.rb_ball->setChecked(false);
		ui.le_radius->setEnabled(false);
		ui.cb_ballHollow->setEnabled(false);
		ui.le_ballThinkness->setEnabled(false);

		ui.rb_cube->setChecked(false);
		ui.le_length->setEnabled(false);
		ui.le_width->setEnabled(false);
		ui.le_height->setEnabled(false);
		ui.cb_cubeHollow->setEnabled(false);
		ui.le_cubeThinkness->setEnabled(false);

	}
	else {

	}
}

void TemplateWidget::on_cb_ballHollow_clicked(bool checked)
{
	if (checked) {
		ui.le_ballThinkness->setEnabled(true);
	}
	else {
		ui.le_ballThinkness->setEnabled(false);
	}
}

void TemplateWidget::on_cb_cubeHollow_clicked(bool checked)
{
	if (checked) {
		ui.le_cubeThinkness->setEnabled(true);
	}
	else {
		ui.le_cubeThinkness->setEnabled(false);
	}
}
