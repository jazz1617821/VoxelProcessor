#pragma once
extern "C" {
#include "voxelize.h"
}
#include "titlebar.hpp"
#include "voxelizewidget.hpp"
#include <QWidget>
#include <QLayout>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QStringList>

enum VOXEL_SCENE_OBJ_ACTIONS { VOBJ_SELECT, VOBJ_DELETE, VOBJ_CLIP, VOBJ_ADD, VOBJ_EXPORT_DF };

class VoxelSceneManagerItem : public QTreeWidgetItem
{
public:
	VoxelSceneManagerItem(QStringList text, VoxelObject *obj);
	~VoxelSceneManagerItem();

public:
	VoxelObject *object;
};

class VoxelSceneManagerWidget : public QWidget {
	Q_OBJECT

public:
	VoxelSceneManagerWidget(QWidget  *parent = Q_NULLPTR);
	~VoxelSceneManagerWidget();

	void paintEvent(QPaintEvent *e);

protected:
	void mousePressEvent(QMouseEvent *e);

private:
	void buildTree(void);

signals:
	void actionOnVoxelObject(VoxelObject *obj, int action);
	void deleteObject(void);
	void sendVoxelScene(VoxelModel *sc);

	public slots:
	// context menu
	void updateVoxelModel(VoxelModel *sc);
	void toggleContextMenu(const QPoint &pos);
	void voxelSelected(VoxelItem *item);
	void itemSelected(QTreeWidgetItem* witem, int index);

	private slots:
	//void closeWidget(void);
	//void dragged(int x, int y);
	void openImageProjectionWidget(void);
	void deleteVoxelObject(void);
	void openVoxelClipWidget(void);
	void addVoxelObject(void);
	void exportDistanceData();

private:
	//TitleBar *titleBar;
	QLayout *vlayout_managerPanel;
	QTreeWidget *treeWidget;

	QTreeWidgetItem *selectedItem;

private:
	VoxelModel *sc;
};


