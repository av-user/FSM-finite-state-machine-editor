#include "tcpclient.h"
#include <QDebug>
#include <QtNetwork>
#include "../LogLib/logger.h"

qint64 TcpClient::write(QByteArray *pByteArray){
	qint64 retval = 0;
	if (m_pTcpSocket != nullptr){
		retval = m_pTcpSocket->write(*pByteArray);
		m_pTcpSocket->flush();
	}
	return retval;
}

bool TcpClient::setHandle (int descriptor){
	bool retval = false;
	delete this->m_pTcpSocket;
	this->m_pTcpSocket = new QTcpSocket (this);
	connect (m_pTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
	connect (m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect (m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	retval = m_pTcpSocket->setSocketDescriptor(descriptor);
	if (retval){
		retval = true;
		qDebug() << "client connected";
	} else {
		qDebug() << "setHandle() failed";
	}
	return retval;
}

void TcpClient::connected(){
	qDebug() << "TcpClient::connected()";
}
void TcpClient::disconnected(){
	qDebug() << "TcpClient::disconnected()";
}
void TcpClient::readyRead(){
	QByteArray bArray = this->m_pTcpSocket->readAll();
	qDebug() << "TcpClient::readyRead(): " << bArray;
    Log::Logger::traceV("HOSTSIM", "TcpClient::readyRead(): %s", bArray.toStdString().c_str());
}
void TcpClient::result (int number){

}

TcpClient::TcpClient(QObject *parent)
: QObject(parent)
, m_pTcpSocket (nullptr)
{

}
