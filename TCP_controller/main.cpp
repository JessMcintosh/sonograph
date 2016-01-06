#include <QApplication>

#include "networkcontroller.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	NetworkController nc;
	nc.show();
	return app.exec();
}
