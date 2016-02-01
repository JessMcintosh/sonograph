#include <QApplication>

#include "fakemachine.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	FakeMachine fm;
	return app.exec();
}
