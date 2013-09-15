#include <QTcpSocket>
#include <iostream>
#include <string>
#include <QThread>
#include <QString>
#include "Client.h"
using namespace std;

ClientSocket::ClientSocket(QObject *parent/* =0 */)						//��������socket����������host
{
	blockSize = 0;																				//��ʼ�����С
	setSocketType("Server");
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));		//�����ݴ���ʱsocket����readyread�źţ���readMessage�۴���
	connect(this,SIGNAL(disconnected()),this,SLOT(connetcionClosedByServer()));
}

ClientSocket::ClientSocket(QString host,int port,QObject *parent/* =0 */)				//�ͻ�����socket������������������ַ��˿ںţ�������
{
	connectToHost(host,port);
	setSocketType("Client");
	blockSize = 0;
	connect(this,SIGNAL(readyRead()),this,SLOT(readMessage()));
	connect(this,SIGNAL(disconnected()),this,SLOT(connetcionClosedByServer()));

}

void ClientSocket::sendMessage(QString str)	//������Ϣ�����鷢�ͣ���0��Ϊ��ʼ��ǣ�д�����ݿ鲢�����ݿ��С�������ݿ�֮ǰ��ȷ������д��
{	
	QByteArray block; 
	char number[30];
	QDataStream out(&block,QIODevice::WriteOnly);  
	out.setVersion(QDataStream::Qt_4_8);  
	out<<(quint16) 0;
	out<<socketType+itoa(socketDescriptor(),number,10)+":"+str;									//��ʾ�ǿͻ��˻��Ƿ���˷����źţ��½�������ֵò�Ƹ��ã�
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
	if(responds.contains("Disconnected"))					//�յ��˳�����Ͽ�����
	{
		disconnectFromHost();
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
		else
		{
			sendMessage("connected");
		}

	}
	else 
	{
		
		sendMessage("this is server respond");
	}
	blockSize=0;
	}
}

void ClientSocket::setSocketType(QString type)						//socket���ͣ���������ͻ���
{
	socketType = type;
}

void ClientSocket::connetcionClosedByServer()
{
	if (this->state()==this->UnconnectedState)
		display("connection closed by server");
	this->close();
}


void ClientSocket::display(QString str)		
{
	cout<<str.toStdString()<<endl;
}

