#include "voxelprocessor.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VoxelProcessor w;
	w.show();
	return a.exec();
}
