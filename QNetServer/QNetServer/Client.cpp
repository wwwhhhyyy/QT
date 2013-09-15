#include <QTcpSocket>
#include <iostream>
#include <string>
#include <QThread>
#include <QString>
#include "Client.h"
#include <ATServer.h>

using namespace std;

ClientSocket::ClientSocket(QObject *parent/* =0 */)						//��������socket����������host
{
	blockSize = 0;																				//��ʼ�����С
	setSocketType("Server");
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));		//�����ݴ���ʱsocket����readyread�źţ���readMessage�۴���
	connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));


}

ClientSocket::ClientSocket(QString host,int port,QObject *parent/* =0 */)				//�ͻ�����socket������������������ַ��˿ںţ�������
{
	connectToHost(host,port);
	setSocketType("Client");
	blockSize = 0;
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
	connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}

void ClientSocket::sendMessage(QString str)	//������Ϣ�����鷢�ͣ���0��Ϊ��ʼ��ǣ�д�����ݿ鲢�����ݿ��С�������ݿ�֮ǰ��ȷ������д��
{	
	QByteArray block;
	QDataStream out(&block,QIODevice::WriteOnly);  
	out.setVersion(QDataStream::Qt_4_8);  
	out<<(quint16) 0;
	out<<socketType+":"+str;									//��ʾ�ǿͻ��˻��Ƿ���˷����źţ��½�������ֵò�Ƹ��ã�
	out.device()->seek(0);  
	out<<(quint16) (block.size()-sizeof(quint16));
	write(block);
}

void ClientSocket::readMessage()							//��ȡ��Ϣ��������գ��Ƚ��տ����ݴ�С����ɽ������ݱȽϣ���С�ڿ��С����δ�����꣬��ֹ����
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
	if(responds.contains("EXIT"))					//�յ��˳�����Ͽ�����
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

void ClientSocket::setSocketType(QString type)						//socket���ͣ���������ͻ���
{
	socketType = type;
}




void ClientSocket::display(QString str)		
{
	cout<<str.toStdString()<<endl;
}

