#pragma once
extern "C" {
#include "voxelize.h"
}
#include "titlebar.hpp"
#include "voxeldatalistmanagerwidget.hpp"
#include <QWidget>
#include <QLayout>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QStringList>
#include <QCheckBox>

class VoxelAttributeWidget : public QWidget {
	Q_OBJECT
	//Method
public:
	VoxelAttributeWidget(QWidget *parent = Q_NULLPTR);
	~VoxelAttributeWidget();

public slots:
	void updateAttribute(VoxelObject * vo);
	void updateAttribute(QTreeWidgetItem* it,int i);
	void clearVoxelObjectAttributePanel(void);
	void changeDFEnable(int);

signals:
	void stateChanged(int);
	//Member
public:
	QCheckBox *distfCheckbox;
private:

	QTreeWidget *treeWidget;
	QLayout *vlayout_managerPanel;
};
