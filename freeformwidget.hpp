#pragma once
#include <QWidget>
#include "ui_freeformwidget.h"


enum FREEFORMMETHOD { FF_CIRCLE, FF_SQUARE, FF_BOXSELECTION };

class FreeFormWidget : public QWidget {
	Q_OBJECT

public:
	FreeFormWidget(QWidget * parent = Q_NULLPTR);
	~FreeFormWidget();

	void paintEvent(QPaintEvent *e);

private:
	Ui::FreeFormWidget ui;

signals:
	void freeFormParameterChecked(int Method, int Radius);

	private slots:
	// tittle slot
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);
	void moveWidget(int x, int y);

	void confirm(void);

	void clickCircle(bool);
	void clickSquare(bool);
	void clickBox(bool);
};
