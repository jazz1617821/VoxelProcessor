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

class VoxelDataListItem : public QTreeWidgetItem
{
public:
	VoxelDataListItem(QStringList text,VoxelItem *it);
	~VoxelDataListItem();

public:
	VoxelItem *it;
};

class VoxelDataListManagerWidget : public QWidget {
	Q_OBJECT

public:
	VoxelDataListManagerWidget(QWidget  *parent = Q_NULLPTR);
	~VoxelDataListManagerWidget();

	void paintEvent(QPaintEvent *e);

protected:
	void mousePressEvent(QMouseEvent *e);

private:
	void buildList(void);

signals:
	void refresh(QTreeWidgetItem*, int);

	public slots:
	// context menu
	void updateVoxelItem(VoxelItem *sc);

	private slots:
	
public:
	QTreeWidget *listWidget;

private:
	QLayout *vlayout_managerPanel;

private:
	VoxelItem *sc;
};


