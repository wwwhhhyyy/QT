#ifndef ATSERVER_THREAD_H
#define ATSERVER_THREAD_H

#include <QThread>
#include <QStringList>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "Client.h"

class ATServer_Thread : public QThread
{
	Q_OBJECT
public:
	ATServer_Thread(int socketDescriptor,QObject *parent);

	void run();
//	void sendCommand(QString str);
	void display(QString str);

signals:
	void error(QTcpSocket::SocketError socketError);

private:
	int socketDescriptor;
	QString command;
	ClientSocket *tcpSocket;
	QStringList responds;
	quint16 blockSize;

private slots:
	void stop();
};

#endif