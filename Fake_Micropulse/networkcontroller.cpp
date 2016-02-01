#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QProcess>

#include "networkcontroller.h"

NetworkController::NetworkController()
{
	address = QHostAddress("127.0.0.1");
	tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(address, 45454))
		qDebug() << "Error starting server" << tcpServer->errorString();
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sessionOpened()));

}

void NetworkController::sessionOpened(){
	
	qDebug() << "NEW CONNECTION";
	tcpSocket = tcpServer->nextPendingConnection();
	QObject::connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
}

void NetworkController::sendData(QByteArray data){
	
	//QByteArray data = QByteArray("foo");
	tcpSocket->write(data);
}

void NetworkController::processPendingDatagrams()
{
	QByteArray data = tcpSocket->readAll();
	QByteArray dummyData;
	//qDebug() << data.toHex();
	if(data.startsWith("CALS 0")){
		qDebug() << "Received Cals 0";
		dummyData = QByteArray("");
		for(int i = 0; i < 64; i++){
		dummyData.append("1af00300ff0801");
		QByteArray elementHex = QByteArray(1,(char)(i+1));
		dummyData.append(elementHex.toHex());
		dummyData.append("8080808080808080808080817e7c56000000000000000000000002073194a84b1d64c3f090170000001b497bcbffffffffffffffffffffffffffffffe9e0fdfffffffffdfaf7f3f0edeae5d8d0cfc8c9d4dad7d2cfc7a8806a77a0c4cec7b4865b4a54749bb7bfb3977e6f64584e4c546172808884745f493c3b434f5b6467635c524b484b505454514e4e5052514b454141444b5155534b433f4249505353504a444040454d565e615f584d423d3f4855626a6b65594d444247515c65696967625d595656585b5d5e5f6061626466686c6c6b686666686b6d7070706e6d6e70727372706e6d6e707376777673727173767b7e807e7a76737375787b7f808181818181817f7e7d7d7d7f828587888786848381818081828384868686868788898b8b8b89888787888a8b8d8c8b89888788898b8c8c8c8b8b8c8c8c8c8c8c8c8c8c8d8d8e8e8e8d8c8c8c8d8e8f8f8e8d8c8b8b8b8c8d8e8e8f8f8f8e8d8d8d8d8e8e8e8e8e8d8d8d8d8d8d8d8d8e8e8e8e8e8e8e8d8d8d8c8c8c8c8c8c8c8c8c8d8d8d8d8d8c8b8b8b8a8a8b8b8b8c8c8c8c8c8c8b8b8b8a8a8b8a8b8b8b8b8b8b8a8a8a8a8a8a8a8a8a8989898888888888888989898a898988888787878787878888888887868686868686868686868686868685858585868686868686868685858484848484848484848383848484848484838383828383838383838383838383828282828282828281818282828282828282828281818181818181818181818181818181808081808081808080808080808080808080807f7f80807f7f7f807f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7e7e7f7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7f7f7e7f7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7f7e7e7e7e7e7e7e7e7e7d7e7e7d7e7e7e7e7d7e7e7e7e7e7e7e7e7e7e7e7e7e7d7e7e7e7e7e7e7e7e7e7e7e7e7d7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7d7d7d7e7d7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7f7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7f7e7e7e7e7e7e7e7f7e7f7f7e7e7f7f7f7f7f7f7f7f7f7f7f7f7f7e7e7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f807f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f807f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f807f7f7f7f7f7f7f807f7f807f7f7f8080808080808080807f7f8080807f7f80807f808080808080807f7f807f80808080808080808080");
		}
		dummyData = QByteArray::fromHex(dummyData);
	}
	else{
		qDebug() << "Received setup info";
		qDebug() << data;
		dummyData = QByteArray("setup info");
	}


	sendData(dummyData);
    //while (tcpSocket->hasPendingDatagrams()) {
    //    QByteArray datagram;
    //    datagram.resize(tcpSocket->pendingDatagramSize());
    //    tcpSocket->readDatagram(datagram.data(), datagram.size());
    //    statusLabel->setText(tr("Received datagram: \"%1\"")
    //                         .arg(datagram.data()));

	//	//char value = (datagram.data())[0];
    //}
}

//void NetworkController::processPendingDatagrams()
//{
//	QByteArray data = tcpSocket->readAll();
//	qDebug() << data.toHex();
//    //while (tcpSocket->hasPendingDatagrams()) {
//    //    QByteArray datagram;
//    //    datagram.resize(tcpSocket->pendingDatagramSize());
//    //    tcpSocket->readDatagram(datagram.data(), datagram.size());
//    //    statusLabel->setText(tr("Received datagram: \"%1\"")
//    //                         .arg(datagram.data()));
//
//	//	//char value = (datagram.data())[0];
//    //}
//}

//void NetworkController::sendData(QByteArray data)
//{
//	//QByteArray data("foobar\r\n\0");
//	tcpSocket->write(data);
//	//if( tcpSocket->waitForConnected() ) {
//		//tcpSocket->write( data );
//	//}
//}
