#pragma once
extern "C" {
#include "voxelize.h"
};
#include "ui_voxelthicknesswidget.h"
#include <QWidget>


class VoxelThicknessWidget : public QWidget {
	Q_OBJECT

public:
	VoxelThicknessWidget(QWidget * parent = Q_NULLPTR);
	~VoxelThicknessWidget();
	void paintEvent(QPaintEvent *e);
	void setItems(VoxelItem *items);
	void showVoxelInfo(VoxelData *vdata);

signals:
	void thickDone(VoxelItem *item, float minValue, float maxValue);

private slots:
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);
	void confirm(void);
	void cancel(void);

	void setSelectedItem(int index);

private:
	void moveWidget(int x, int y);

private:
	Ui::VoxelThicknessWidget ui;

	VoxelItem *items;
	VoxelItem *selectedItem;
};
