#ifndef ATSERVER_H
#define ATSERVER_H

#include <QStringList>
#include <QtNetwork/QTcpServer>
#include <ATServer_Thread.h>

class ATServer : public QTcpServer
{
	Q_OBJECT

public:
	ATServer(int port,QObject *parent = 0);
	ATServer_Thread *thread;

protected:
	void incomingConnection(int socketDescriptor);

};


#endif