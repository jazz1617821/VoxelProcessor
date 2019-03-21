#pragma once
extern "C" {
#include "voxelize.h"
#include "distancefield.h"
}
#include "ui_voxeldistancefieldwidget.h"
#include <QWidget>

class VoxelDistanceFieldWidget : public QWidget {
	Q_OBJECT

public:
	VoxelDistanceFieldWidget(QWidget *parent = Q_NULLPTR);
	~VoxelDistanceFieldWidget();
	void paintEvent(QPaintEvent *e);
	void setItems(VoxelItem *items);
	void showVoxelInfo(VoxelData *vdata);
	
signals:
	void dfDone(VoxelItem *item);

private slots:
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);
	void confirm(void);
	void cancel(void);

	void setSelectedItem(int index);

private:
	void moveWidget(int x, int y);
	int checkValues(void);

private:
	Ui::VoxelDistanceFieldWidget ui;

	VoxelItem *items;		// items pointer
	VoxelItem *selectedItem;
	bool dfChnaged;			// to set the defualt values
};
