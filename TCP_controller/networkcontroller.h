#ifndef RECEIVER_H
#define RECEIVER_H

#include <QHostAddress>

class QTcpSocket;

class NetworkController : public QObject
{
    Q_OBJECT

public:
    NetworkController();
	void setSize(int x, int t){
		numElements = x;
		sampleSize = t;
		qDebug() << "Set size: " << x << t;
	}
	void initBuffer() { 
		if(dataBuffer = new double[numElements * sampleSize]){
			qDebug() << "could not alloc";
		}
	}
	//int index(int t, int x) { return x * sampleSize + t; }
	int index(int t, int x) { return t + x * sampleSize; }
	bool ignoreNext;
	bool bufferFull;
	double *dataBuffer;

public slots:
    void processPendingDatagrams();
    void sendData(QByteArray data);

signals:
   void newData();  


private:
    QTcpSocket *tcpSocket;
    QHostAddress groupAddress;
	int numElements;
	int sampleSize;
	int currentElement;
	int currentTime;
};

#endif
