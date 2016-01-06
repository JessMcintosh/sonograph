#ifndef RECEIVER_H
#define RECEIVER_H

#include <QDialog>
#include <QHostAddress>

class QLabel;
class QPushButton;
class QTcpSocket;

class NetworkController : public QDialog
{
    Q_OBJECT

public:
    NetworkController(QWidget *parent = 0);

private slots:
    void processPendingDatagrams();
    void sendData();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QPushButton *sendButton;
    QTcpSocket *tcpSocket;
    QHostAddress groupAddress;
};

#endif
