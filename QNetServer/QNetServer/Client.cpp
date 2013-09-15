#include <QTcpSocket>
#include <iostream>
#include <string>
#include <QThread>
#include <QString>
#include "Client.h"
#include <ATServer.h>

using namespace std;

ClientSocket::ClientSocket(QObject *parent/* =0 */)						//服务器用socket，不需连接host
{
	blockSize = 0;																				//初始化块大小
	setSocketType("Server");
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));		//有数据传入时socket发送readyread信号，用readMessage槽处理
	connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));


}

ClientSocket::ClientSocket(QString host,int port,QObject *parent/* =0 */)				//客户端用socket，构造器传入主机地址与端口号，并连接
{
	connectToHost(host,port);
	setSocketType("Client");
	blockSize = 0;
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
	connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}

void ClientSocket::sendMessage(QString str)	//发送信息，按块发送，用0作为起始标记，写入数据块并将数据块大小插入数据块之前，确保数据写完
{	
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);  
	out.setVersion(QDataStream::Qt_4_8);  
	out<<(quint16) 0;
	out<<socketType+":"+str;									//显示是客户端还是服务端发的信号（新建个布尔值貌似更好）
	out.device()->seek(0);  
	out<<(quint16) (block.size()-sizeof(quint16));
	write(block);
}

void ClientSocket::readMessage()							//读取信息，按块接收，先接收块数据大小，与可接受数据比较，若小于块大小，则未发送完，终止接收
{
	QDataStream in(this);  
	in.setVersion(QDataStream::Qt_4_8);    
	forever{
	if(blockSize==0)
	{  
		if(bytesAvailable() < (int)sizeof(quint16)) 
			break;  
		in >> blockSize;  
	}  
	if(blockSize==0xFFFF){
		break;
	}

	if(bytesAvailable() < blockSize) 
		break;  
	in >> responds;
	display(responds);
	if(responds.contains("EXIT"))					//收到退出命令，断开连接
	{
		display(responds.mid(0,10)+":disconnected");
		this->close();
	}

	if (socketType=="Client")
	{
		if(responds.contains("local command type"))
		{	
			commandType = 1;
		}
		else if(responds.contains("other type"))
		{
			commandType = 2;	
		}
		else if(responds.contains("Successful"))
		{
			commandType =3;
		}
		
		if (commandType==1)
		{
			string localMessage;
			cin>>localMessage;
			QString msg;
			msg=localMessage.c_str();
			sendMessage(msg);		
		}
		else if(commandType==2)
		{
			string otherMessage;
			cin>>otherMessage;
			QString msg;
			msg=otherMessage.c_str();
			sendMessage(msg);	
		}
		else if(commandType=3)
		{
			sendMessage("connected");
		}

	}
	else 
	{
		if(responds.contains("connected"))
		{
			blockSize=0;
			responds.clear();
			return;
		}
		string severMessage;
		cin>>severMessage;
		QString msg;
		msg=severMessage.c_str();
		sendMessage(msg);	
	}
	blockSize=0;
	
	}
}

void ClientSocket::setSocketType(QString type)						//socket类型：服务器或客户端
{
	socketType = type;
}




void ClientSocket::display(QString str)		
{
	cout<<str.toStdString()<<endl;
}

