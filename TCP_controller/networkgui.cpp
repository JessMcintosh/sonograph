#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>

#include "networkgui.h"

NetworkGUI::NetworkGUI(QWidget *parent)
    : QDialog(parent)
{
	controller = new NetworkController();

    statusLabel = new QLabel(tr("Listening for multicasted messages"));
    quitButton = new QPushButton(tr("&Quit"));
    sendButton = new QPushButton(tr("&Send"));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendSomething()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(sendButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("TCP client"));
}

void NetworkGUI::sendSomething()
{
	QByteArray data("foobar\r\n");
	//QString data("foobar\r\n");
	controller->sendData(data);
	//if( tcpSocket->waitForConnected() ) {
		//tcpSocket->write( data );
	//}
}
