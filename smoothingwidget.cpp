#include "smoothingwidget.hpp"

SmoothingWidget::SmoothingWidget(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);

	ui.titleBar->setBackgroundColor(QColor(153, 204, 255));

	// connect 
	connect(ui.titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(ui.titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragWidget(int, int)));
	connect(ui.titleBar, SIGNAL(released(int, int)), this, SLOT(dragDone(int, int)));

}

SmoothingWidget::~SmoothingWidget() {
	
}

void SmoothingWidget::paintEvent(QPaintEvent * e)
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
void SmoothingWidget::closeWidget(void)
{
	this->close();
}

void SmoothingWidget::dragWidget(int x, int y)
{
	moveWidget(x, y);
}

void SmoothingWidget::dragDone(int x, int y)
{

}

void SmoothingWidget::moveWidget(int x, int y)
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



void SmoothingWidget::on_pb_erosion_clicked()
{
	if (ui.rb_six_neighbor->isChecked()) {
		emit erosionClicked(SIX_NEIGHBOR);
	}
	else if(ui.rb_twenty_six_neighbor->isChecked()){
		emit erosionClicked(TWENTY_SIX_NEIGHBOR);
	}
}

void SmoothingWidget::on_pb_dilation_clicked()
{
	if (ui.rb_six_neighbor->isChecked()) {
		emit dilationClicked(SIX_NEIGHBOR);
	}
	else if (ui.rb_twenty_six_neighbor->isChecked()) {
		emit dilationClicked(TWENTY_SIX_NEIGHBOR);
	}
}

void SmoothingWidget::on_pb_opening_clicked()
{
	if (ui.rb_six_neighbor->isChecked()) {
		emit openingClicked(ui.sb_openingErosionTimes->value(), ui.sb_openingDilationTimes->value(), SIX_NEIGHBOR);
	}
	else if (ui.rb_twenty_six_neighbor->isChecked()) {
		emit openingClicked(ui.sb_openingErosionTimes->value(), ui.sb_openingDilationTimes->value(), TWENTY_SIX_NEIGHBOR);
	}
}

void SmoothingWidget::on_pb_closing_clicked()
{
	if (ui.rb_six_neighbor->isChecked()) {
		emit closingClicked(ui.sb_closingDilationTimes->value(), ui.sb_closingErosionTimes->value(), SIX_NEIGHBOR);
	}
	else if (ui.rb_twenty_six_neighbor->isChecked()) {
		emit closingClicked(ui.sb_closingDilationTimes->value(), ui.sb_closingErosionTimes->value(), TWENTY_SIX_NEIGHBOR);
	}
}
