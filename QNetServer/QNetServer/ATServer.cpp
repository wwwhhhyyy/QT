#include "ATServer.h"
#include "ATServer_Thread.h"
#include <iostream>
#include <stdlib.h>

ATServer::ATServer(int port,QObject *parent)				
	: QTcpServer(parent)
{
	if(!listen(QHostAddress::Any,port))						//服务器，负责监听给定端口（不指定则随机监听端口）的所有连接ip
	{
		std::cerr<<"Fail to bind port"<<std::endl;
		return;
	}
}


void ATServer::incomingConnection(int socketDescriptor)			//有连接时会调用incomingconnection函数，在此函数里创建线程与客户端通信
{

	thread  = new ATServer_Thread(socketDescriptor,this);			//传入服务器自动分配的socketDescriptor参数，标明与服务器连接的端口编号
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));		//通信任务完成则终止线程
	thread->start();
}