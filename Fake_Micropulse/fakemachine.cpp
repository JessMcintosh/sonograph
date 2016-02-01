#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>

#include "fakemachine.h"

FakeMachine::FakeMachine(QWidget *parent)
    : QObject(parent)
{
	controller = new NetworkController();
}
