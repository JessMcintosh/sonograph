#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>

#include "networkcontroller.h"

NetworkController::NetworkController(QWidget *parent)
    : QDialog(parent)
{
    groupAddress = QHostAddress("localhost");

    statusLabel = new QLabel(tr("Listening for multicasted messages"));
    quitButton = new QPushButton(tr("&Quit"));
    sendButton = new QPushButton(tr("&Send"));

    tcpSocket = new QTcpSocket(this);
	tcpSocket->connectToHost("127.0.0.1", 45454);

    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(sendData()));

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

void NetworkController::processPendingDatagrams()
{
	QByteArray data = tcpSocket->readAll();
	qDebug() << data;
    //while (tcpSocket->hasPendingDatagrams()) {
    //    QByteArray datagram;
    //    datagram.resize(tcpSocket->pendingDatagramSize());
    //    tcpSocket->readDatagram(datagram.data(), datagram.size());
    //    statusLabel->setText(tr("Received datagram: \"%1\"")
    //                         .arg(datagram.data()));

	//	//char value = (datagram.data())[0];
    //}
}

void NetworkController::sendData()
{
	QByteArray data("foobar\r\n\0");
	tcpSocket->write( data );
	//if( tcpSocket->waitForConnected() ) {
		//tcpSocket->write( data );
	//}
}
