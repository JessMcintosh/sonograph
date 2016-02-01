#ifndef RECEIVER_H
#define RECEIVER_H

#include <QHostAddress>

class QTcpSocket;
class QTcpServer;

class NetworkController : public QObject
{
    Q_OBJECT

public:
    NetworkController();

private slots:
	void sessionOpened();
	void processPendingDatagrams();

private:
    void sendData(QByteArray data);
	QTcpServer *tcpServer;
	QTcpSocket *tcpSocket;
    QHostAddress address;
};

#endif
