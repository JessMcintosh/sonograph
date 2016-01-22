#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>

#include "networkcontroller.h"

NetworkController::NetworkController()
{
    groupAddress = QHostAddress("localhost");

    tcpSocket = new QTcpSocket(this);
	//tcpSocket->connectToHost("127.0.0.1", 45454);
	tcpSocket->connectToHost("10.1.1.2", 1067);

	QObject::connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
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

void NetworkController::sendData(QByteArray data)
{
	//QByteArray data("foobar\r\n\0");
	tcpSocket->write(data);
	//if( tcpSocket->waitForConnected() ) {
		//tcpSocket->write( data );
	//}
}
