#ifndef RECEIVER_H
#define RECEIVER_H

#include <QHostAddress>

class QTcpSocket;

class NetworkController : public QObject
{
    Q_OBJECT

public:
    NetworkController();

public slots:
    void processPendingDatagrams();
    void sendData(QByteArray data);

private:
    QTcpSocket *tcpSocket;
    QHostAddress groupAddress;
};

#endif
