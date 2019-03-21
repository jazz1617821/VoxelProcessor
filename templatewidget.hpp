#pragma once
extern "C" {
#include "voxelize.h"
}
#include <QWidget>
#include <QMessageBox>
#include "ui_templatewidget.h"

enum MakeThing { Nothing, Empty, Ball, Cube, Cylinder };

struct BallParameter {
	float radius;
	bool hollow;
	float thinkness;
};

struct CubeParameter {
	float l, h, w;
	bool hollow;
	float thinkness;
};

class TemplateWidget : public QWidget {
	Q_OBJECT

public:
	TemplateWidget(QWidget * parent = Q_NULLPTR);
	~TemplateWidget();

	VoxelData* makeVoxelObject(void);
	bool readyOrNot(void);
	void reset(void);

private:
	void makeBall(VoxelData*);
	void makeCube(VoxelData*);

signals:
	void loadParaFinished(void);

public slots:
	void confirm(void);

private slots:
    void on_rb_empty_clicked(bool checked);
    void on_rb_ball_clicked(bool checked);
    void on_rb_cube_clicked(bool checked);
    void on_rb_cylinder_clicked(bool checked);
    void on_cb_ballHollow_clicked(bool checked);
    void on_cb_cubeHollow_clicked(bool checked);

private:
	Ui::TemplateWidget ui;

private:
	int colorId;

	int makewhat;

	char name[512];
	float voxelSize[3];

	BallParameter ballPara;
	CubeParameter cubePara;

};
