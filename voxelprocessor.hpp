#ifndef VOXELPROCESSOR_H
#define VOXELPROCESSOR_H

#include <QtWidgets/QMainWindow>
#include "ui_voxelprocessor.h"

class VoxelProcessor : public QMainWindow
{
	Q_OBJECT

public:
	VoxelProcessor(QWidget *parent = 0);
	~VoxelProcessor();

signals:
	void openVoxelModel(const char *filepath);
	void saveVoxelModel();
	void createVoxelModel(void);

public slots:
	void on_actionOpen_triggered(void);
	void on_actionQuit_triggered(void);
	void on_actionNew_triggered(void);
	void on_actionSave_triggered(void);

private:
	Ui::VoxelProcessorClass ui;
};

#endif // VOXELPROCESSOR_H
