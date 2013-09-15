#include "Client.h"
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	ClientSocket socket("localhost",9999);
	return a.exec();
}
