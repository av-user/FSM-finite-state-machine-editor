#include "hostserver.h"
#include "hostthread.h"
#include "tcpclient.h"

#include "mainwindow.h"

#include <stdlib.h>
#include "../LogLib/logger.h"

void HostServer::setWindow (MainWindow *pWnd){
	m_pWnd = pWnd;
}

void HostServer::incomingConnection(qintptr socketDescriptor){
	Log::Logger::traceV("HOSTSIM", "HostServer::incomingConnection(0x%08x)", socketDescriptor);
	HostThread *pHThread = new HostThread (socketDescriptor, /*msg, */this);
	connect(pHThread, SIGNAL(finished()), pHThread, SLOT(deleteLater()));
	connect(pHThread, SIGNAL(add_output_msg(QString,QString,QString)), m_pWnd, SLOT(add_output_msg (QString, QString, QString)));
	connect(pHThread, SIGNAL(add_input_msg(QString,QString,QString)), m_pWnd, SLOT(add_input_msg (QString, QString, QString)));
	pHThread->start();
}

HostServer::HostServer(QObject *parent)
: QTcpServer(parent)
, m_pWnd (nullptr)
{
}


