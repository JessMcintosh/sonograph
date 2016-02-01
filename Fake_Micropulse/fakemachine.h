#ifndef FAKEMACHINE_H
#define	FAKEMACHINE_H 

#include "networkcontroller.h"

class NetworkController;

class FakeMachine : public QObject
{
    Q_OBJECT

public:
    FakeMachine(QWidget *parent = 0);
	NetworkController *controller;
};

#endif
