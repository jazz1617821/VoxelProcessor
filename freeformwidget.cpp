#include "freeformwidget.hpp"

FreeFormWidget::FreeFormWidget(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	ui.pb_circleImage->setIcon(QIcon("ApplicationImages/circle_selected.jpg"));
	ui.pb_circleImage->setIconSize(QSize(40, 40));
	ui.pb_circleImage->setCheckable(true);
	ui.pb_circleImage->setChecked(true);

	ui.pb_squareImage->setIcon(QIcon("ApplicationImages/square.jpg"));
	ui.pb_squareImage->setIconSize(QSize(40, 40));
	ui.pb_squareImage->setCheckable(true);
	ui.pb_squareImage->setChecked(false);

	ui.pb_boxSelectionImage->setIcon(QIcon("ApplicationImages/box.jpg"));
	ui.pb_boxSelectionImage->setIconSize(QSize(40, 40));
	ui.pb_boxSelectionImage->setCheckable(true);
	ui.pb_boxSelectionImage->setChecked(false);

	// connect 
	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));

	connect(ui.btmbox_widget, SIGNAL(accepted(void)), this, SLOT(confirm(void)));
	connect(ui.btmbox_widget, SIGNAL(rejected(void)), this, SLOT(closeWidget(void)));

	// push button
	connect(ui.pb_circleImage, SIGNAL(clicked(bool)), this, SLOT(clickCircle(bool)));
	connect(ui.pb_squareImage, SIGNAL(clicked(bool)), this, SLOT(clickSquare(bool)));
	connect(ui.pb_boxSelectionImage, SIGNAL(clicked(bool)), this, SLOT(clickBox(bool)));

}

FreeFormWidget::~FreeFormWidget() {

}

void FreeFormWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(ui.titleBar->rect().adjusted(1, 1, 15, 0), ui.titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

//
//		Slots
//
void FreeFormWidget::closeWidget(void)
{
	this->close();
}

void FreeFormWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void FreeFormWidget::dragDone(int x, int y)
{
	//moveWidget(x, y);
}

void FreeFormWidget::moveWidget(int x, int y)
{
	int posX, posY;

	posX = this->x() + x;
	posY = this->y() + y;

	QRect boundary = this->parentWidget()->rect();

	if (posX < boundary.topLeft().x()) {
		posX = boundary.topLeft().x();
	}
	if (posY < boundary.topLeft().y()) {
		posY = boundary.topLeft().y();
	}
	if (posX > boundary.bottomRight().x() - ui.titleBar->width()) {
		posX = boundary.bottomRight().x() - ui.titleBar->width();
	}
	if (posY > boundary.bottomRight().y() - ui.titleBar->height()) {
		posY = boundary.bottomRight().y() - ui.titleBar->height();
	}

	move(posX, posY);
}

void FreeFormWidget::confirm(void)
{
	if (ui.pb_circleImage->isChecked()) {
		this->close();
		emit(freeFormParameterChecked(FF_CIRCLE, ui.sb_circleRadius->value()));
	}
	else if (ui.pb_squareImage->isChecked()) {
		this->close();
		emit(freeFormParameterChecked(FF_SQUARE, ui.sb_squareRadius->value()));
	}
	else if (ui.pb_boxSelectionImage->isChecked()) {
		this->close();
		emit(freeFormParameterChecked(FF_BOXSELECTION, 0));
	}
}

void FreeFormWidget::clickCircle(bool i)
{
	ui.pb_circleImage->setChecked(true);
	ui.pb_squareImage->setChecked(false);
	ui.pb_boxSelectionImage->setChecked(false);
	ui.pb_circleImage->setIcon(QIcon("ApplicationImages/circle_selected.jpg"));
	ui.pb_squareImage->setIcon(QIcon("ApplicationImages/square.jpg"));
	ui.pb_boxSelectionImage->setIcon(QIcon("ApplicationImages/box.jpg"));
}

void FreeFormWidget::clickSquare(bool i)
{
	ui.pb_squareImage->setChecked(true);
	ui.pb_circleImage->setChecked(false);
	ui.pb_boxSelectionImage->setChecked(false);
	ui.pb_circleImage->setIcon(QIcon("ApplicationImages/circle.jpg"));
	ui.pb_squareImage->setIcon(QIcon("ApplicationImages/square_selected.jpg"));
	ui.pb_boxSelectionImage->setIcon(QIcon("ApplicationImages/box.jpg"));
}

void FreeFormWidget::clickBox(bool i)
{
	ui.pb_boxSelectionImage->setChecked(true);
	ui.pb_circleImage->setChecked(false);
	ui.pb_squareImage->setChecked(false);
	ui.pb_circleImage->setIcon(QIcon("ApplicationImages/circle.jpg"));
	ui.pb_squareImage->setIcon(QIcon("ApplicationImages/square.jpg"));
	ui.pb_boxSelectionImage->setIcon(QIcon("ApplicationImages/box_selected.jpg"));
}

