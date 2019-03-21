#pragma once
#include <QWidget>
#include "ui_smoothingwidget.h"
#include "neighbor.h"

class SmoothingWidget : public QWidget {
	Q_OBJECT

public:
	SmoothingWidget(QWidget * parent = Q_NULLPTR);
	~SmoothingWidget();

	void paintEvent(QPaintEvent *e);

private:
	Ui::SmoothingWidget ui;

signals:
	void erosionClicked(int neighborMode);
	void dilationClicked(int neighborMode);
	void openingClicked(int erosionTimes, int dilationTimes, int neighborMode);
	void closingClicked(int dilationTimes, int erosionTimes, int neighborMode);

private slots:
	// tittle slot
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);
	void moveWidget(int x, int y);
    void on_pb_erosion_clicked();
    void on_pb_dilation_clicked();
    void on_pb_opening_clicked();
    void on_pb_closing_clicked();
};
