#include "ATServer_Thread.h"
#include "Client.h"
#include <iostream>
#include <QtNetwork/QtNetwork>
using namespace std;

ATServer_Thread::ATServer_Thread(int socketDescriptor,QObject *parent) : QThread(parent),socketDescriptor(socketDescriptor)
{
}

void ATServer_Thread::run()										
{
	tcpSocket = new ClientSocket();	
	if(!tcpSocket->setSocketDescriptor(socketDescriptor))					//用给与的客户端编号建立服务器端的socket负责通信
	{
		emit error(tcpSocket->error());
		return;
	}
	tcpSocket->sendMessage("Successfully connected to Server");
	tcpSocket->waitForReadyRead();
	
	QString ipAdderss = tcpSocket->peerAddress().toString();					//获取客户端ip，判断是否本地ip
	if (ipAdderss == QHostAddress(QHostAddress::LocalHost).toString())				
	{
		tcpSocket->sendMessage("local command type:wait for command");
	}
	else
	{
		tcpSocket->sendMessage("other type:wait for command");
	}

	while(true){
			tcpSocket->waitForReadyRead();		//暂停线程，等待客户端回应（有更好的方法吗）
			sleep(1);
	}
}
/*
	responds = tcpSocket->responds;
	if (!responds.empty())				//分析客户端发送的命令，此段有待商榷
	{
		for(size_t i = 0;i<responds.size();i++)
		{
			QString res = responds.at(i);
			cout<<res.toStdString()<<endl;
			if(res=="EXIT")
			{
				tcpSocket->disconnectFromHost();
				tcpSocket->waitForDisconnected();
				break;
			}			
		}
	}
	else				
	{
		QString err = "No respond";
		tcpSocket->sendMessage(err);
		//tcpSocket->disconnectFromHost();
		//tcpSocket->waitForDisconnected();
	}
	}

}


void ATServer_Thread::readMessage()
{
	QString message;
	QDataStream in(tcpSocket);  
	in.setVersion(QDataStream::Qt_4_8);    
	if(blockSize==0)
	{  
		if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) 
			return;  
		in >> blockSize;  
	}  
	if(tcpSocket->bytesAvailable() < blockSize) 
		return;  
	in >> message;
	display(message);
	responds.append(message);
}

void ATServer_Thread::sendCommand(QString str)
{
	QByteArray block; 
	QDataStream out(&block,QIODevice::WriteOnly);  
	out.setVersion(QDataStream::Qt_4_8);  
	out<<(quint16) 0;  
	out<<str;  
	out.device()->seek(0);  
	out<<(quint16) (block.size()-sizeof(quint16));
	tcpSocket->write(block);
}

*/


void ATServer_Thread::display(QString str)			//傻逼函数，显示用的
{
	cout<<str.toStdString()<<endl;
}

void ATServer_Thread::stop() {
	this->terminate();
}