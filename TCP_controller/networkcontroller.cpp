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
	currentTime = 0;
	currentElement = 0;
	ignoreNext = false;
}

void NetworkController::processPendingDatagrams()
{
	QByteArray data = tcpSocket->readAll();

	if(ignoreNext) { ignoreNext = false; return; }

	//qDebug() << "--Start data read--";

	while(data.toHex().startsWith("1af00300ff")){
		//qDebug() << "starts with 1af";
		//qDebug() << data.toHex();
		// Remove first 8 byte
		currentElement = (int)data[7] - 1;
		data.remove(0, 8);
		//qDebug() << "Element: " << currentElement;
		for(int t = 0; t < sampleSize; t++){
			//dataBuffer[index(t,currentElement)] = (((double)data[t]))/128.0;
			//double val = (double)data[t];
			//if(val > 128.0) val = 256.0 - val;
			//	
			//dataBuffer[index(t,currentElement)] = val / 128.0;
			//dataBuffer[index(t,currentElement)] = (((double)data[t]))/128.0;
			//qDebug() << dataBuffer[index(t,currentElement)];
			//qDebug() << data[t];
			//qDebug() << (double)data[t];
			dataBuffer[index(t,currentElement)] = (( (int)((unsigned char)data[t]) - 128))/128.0;
			//qDebug() << dataBuffer[index(t,currentElement)];
			//qDebug() << (int)data[t];
		}
		data.remove(0, sampleSize);

		//if(currentElement == 63) emit newData();


	}
	emit newData();


	//qDebug() << "-- End data read --";

	QDebug deb = qDebug();
	for(int i = 0; i < sampleSize * numElements; i++){
		//deb << dataBuffer[i];
	}

	

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
