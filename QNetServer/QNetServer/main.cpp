#include "ATServer.h"
#include "Client.h"
#include <QtNetwork/QtNetwork>
#include <QtCore/QCoreApplication>

ATServer *pServer;
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	pServer = new ATServer(9999);

	return a.exec();
}
