#pragma once

extern "C"{
#include "voxelize.h"
};

#include "ui_voxelclipplanewidget.h"
#include <QWidget>

class VoxelClipPlaneWidget : public QWidget {
	Q_OBJECT

public:
	VoxelClipPlaneWidget(QWidget * parent = Q_NULLPTR);
	~VoxelClipPlaneWidget();

	void paintEvent(QPaintEvent *e);

	void showClipInfo(VoxelData *vdata);
	void setItems(VoxelItem *items);
	void setCurrentItem(VoxelItem *selectedItem);

signals:
	void itemSelected(VoxelItem *item);
	void updateVoxelClip(int i, VoxelItem *item);

private slots:
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);

	// cbb select
	void setSelectedItem(int index);

	// check the values
	void setXClipSlider(int value);
	void setInvXClipSlider(int value);
	void setYClipSlider(int value);
	void setInvYClipSlider(int value);
	void setZClipSlider(int value);
	void setInvZClipSlider(int value);
	void setXClipSpinBox(int value);
	void setInvXClipSpinBox(int value);
	void setYClipSpinBox(int value);
	void setInvYClipSpinBox(int value);
	void setZClipSpinBox(int value);
	void setInvZClipSpinBox(int value);
	void updateXClip(int state);
	void updateInvXClip(int state);
	void updateYClip(int state);
	void updateInvYClip(int state);
	void updateZClip(int state);
	void updateInvZClip(int state);

private:
	void moveWidget(int x, int y);

private:
	Ui::VoxelClipPlaneWidget ui;

	VoxelItem *items;
	VoxelItem *selectedItem;
};
