#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QStringList>

class ClientSocket : public QTcpSocket
{
	Q_OBJECT
public:
	ClientSocket(QObject *parent=0);
	ClientSocket(QString host,int port,QObject *parent=0);

	void display(QString str);
	void sendMessage(QString str);
	void setSocketType(QString type);

	


private slots:
	
	void readMessage();
	void connetcionClosedByServer();

private:
	QString responds;	
	QString socketType;
	quint16 blockSize;
	int commandType;
};

#endif