#include <QApplication>

#include "networkgui.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	NetworkGUI nc;
	nc.show();
	return app.exec();
}
