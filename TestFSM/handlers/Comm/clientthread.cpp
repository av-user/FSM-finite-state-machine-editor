#include "clientthread.h"
#include <QDataStream>
#include "handlers/Comm/Framework/commhdl.h"
#include "statemodel/msg.h"
#include "../LogLib/logger.h"
#include "tcpsocket.h"

const char *ClientThread::HOST_IP = "192.168.101.14";
const int ClientThread::HOST_PORT = 2067;

void ClientThread::run(){
	Log::Logger::traceV("COMMSKT", "ClientThread started in thread %u", QThread::currentThreadId());
	qDebug() << " ClientThread started in thread " << QThread::currentThreadId();
	m_pTcpSocket = new TcpSocket(this->m_CommHdl, HOST_IP, HOST_PORT);

	qDebug() << "this thread: " << this->thread()
		<< "socket thread: " << m_pTcpSocket->thread()
		<< ", current thread:" << QThread::currentThread();

	qRegisterMetaType< QAbstractSocket::SocketError >();
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), m_pTcpSocket, SLOT(errorSlot(QAbstractSocket::SocketError)));
    connect(m_pTcpSocket, SIGNAL(connected()), m_pTcpSocket, SLOT(connectedSlot()));
    connect(m_pTcpSocket, SIGNAL(disconnected()), m_pTcpSocket, SLOT(disconnectedSlot()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), m_pTcpSocket, SLOT(readyReadSlot()));
	connect(m_pTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
	connect(this, SIGNAL(send(QString)), m_pTcpSocket, SLOT(writeSlot(QString)));

	qDebug() << "Connecting,..";
	m_pTcpSocket->connectToHost(HOST_IP, HOST_PORT);
	this->exec();
}
void ClientThread::disconnectFromHost(){
	Log::Logger::trace("COMMSKT", "ClientThread::disconnectFromHost()");
	emit disconnectSocket();
}
void ClientThread::write(QString str){
	Log::Logger::traceV("COMMSKT", "ClientThread::write() str: %s", str.toStdString().c_str());
	emit send(str);
}

void ClientThread::error(QAbstractSocket::SocketError socketError){
	Log::Logger::traceV("COMMSKT", "ClientThread::error() %s", m_pTcpSocket->errorString().toStdString().c_str());
    m_CommHdl.enqueue(new StateModel::Msg ("ClientThread", StateModel::Msg::Comm_Error, "error"));
	qDebug() << "Socket error: " << socketError;
	QThread::usleep(10000000);
	if (m_pTcpSocket->state() == QTcpSocket::UnconnectedState){
		emit connectSocket(HOST_IP, HOST_PORT);
	}
}
void ClientThread::connected(){
	Log::Logger::trace("COMMSKT", "socket connected");
	qDebug() << "Connected!";
    m_CommHdl.enqueue(new StateModel::Msg ("ClientThread", StateModel::Msg::Comm_Connected, "connected"));
}
void ClientThread::disconnected(){
	Log::Logger::trace("COMMSKT", "socket disconnected");
	qDebug() << "Disconnected!";
    m_CommHdl.enqueue(new StateModel::Msg ("ClientThread", StateModel::Msg::Comm_Disconnected, "disconnected"));
	QThread::usleep(10000000);
	if (m_pTcpSocket->state() == QTcpSocket::UnconnectedState){
		emit connectSocket(HOST_IP, HOST_PORT);
	}
}
void ClientThread::bytesWritten(qint64 bytes){
	Log::Logger::traceV("COMMSKT", "%lu bytes written", bytes);
	qDebug() << "We wrote: " << bytes;
}
void ClientThread::readyRead(){
	qDebug() << "Reading...";
	QByteArray barray = m_pTcpSocket->readAll();
	qDebug() << "read: " << barray;
	m_ReadBufferArray.append(barray);
	int size = m_ReadBufferArray.size();
	if (m_ReadBufferArray.size() >= sizeof (quint16)){
		QDataStream in(&m_ReadBufferArray, QIODevice::ReadOnly);
		quint16 msglen = 0;
		in >> msglen;
		while (size >= msglen + sizeof (quint16)){
			QString msgstr;
			in >> msgstr;
			m_ReadBufferArray.remove(0, msglen + sizeof (quint16));
			size = m_ReadBufferArray.size();
            StateModel::Msg *pMsg = new StateModel::Msg ("ClientThread", StateModel::Msg::Comm_Message, "message", msgstr);
            m_CommHdl.enqueue(pMsg);
		}
	}
}
ClientThread::ClientThread(CommHdl &commHdl/*, QObject *pParent*/)
: m_CommHdl (commHdl)
, m_pTcpSocket (nullptr)
{
qDebug() << "ClientThread constructor";
}
