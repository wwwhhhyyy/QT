#include "ATServer.h"
#include "ATServer_Thread.h"
#include <iostream>
#include <stdlib.h>

ATServer::ATServer(int port,QObject *parent)				
	: QTcpServer(parent)
{
	if(!listen(QHostAddress::Any,port))						//��������������������˿ڣ���ָ������������˿ڣ�����������ip
	{
		std::cerr<<"Fail to bind port"<<std::endl;
		return;
	}
}


void ATServer::incomingConnection(int socketDescriptor)			//������ʱ�����incomingconnection�������ڴ˺����ﴴ���߳���ͻ���ͨ��
{

	thread  = new ATServer_Thread(socketDescriptor,this);			//����������Զ������socketDescriptor��������������������ӵĶ˿ڱ��
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));		//ͨ�������������ֹ�߳�
	thread->start();
}