#pragma once
#include "ui_slicewidget.h"
#include <QWidget>

class SliceWidget : public QWidget {
	Q_OBJECT

public:
	SliceWidget(QWidget * parent = Q_NULLPTR);
	~SliceWidget();

signals:
	void sliceDone(void);

public slots:
	void confirm(void);
	void cancel(void);


private:
	Ui::SliceWidget ui;
};
