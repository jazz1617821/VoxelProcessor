#include "slicewidget.hpp"

SliceWidget::SliceWidget(QWidget * parent) : QWidget(parent) 
{
	ui.setupUi(this);



	// default values
	ui.le_nozzleSize->setText("0.4");
	ui.le_layerHeight->setText("0.2");
	ui.le_temperature->setText("215");
	ui.le_feedRate->setText("30");
	ui.le_filamentDiameter->setText("1.75");

	// connect
	connect(ui.btmbox_widget, SIGNAL(accepted(void)), this, SLOT(confirm(void)));
	connect(ui.btmbox_widget, SIGNAL(rejected(void)), this, SLOT(cancel(void)));
}

SliceWidget::~SliceWidget()
{
	
}


//
// Slot
//

void SliceWidget::confirm(void)
{
	// close
	this->hide();

	emit sliceDone();
}

void SliceWidget::cancel(void)
{
	this->hide();
}
