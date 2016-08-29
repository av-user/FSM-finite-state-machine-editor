#include "hostthread.h"
#include "tcpclient.h"
#include <QtNetwork>
#include "../LogLib/logger.h"
#include "utility.h"

void HostThread::send (QString str){
	Log::Logger::traceV("HOSTSIM", "HostThread::send(): %s", str.toStdString().c_str());
	QByteArray byteArray;
	QDataStream out(&byteArray, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << (quint16)0;
	out << str;
	out.device()->seek(0);
	out << (quint16)(byteArray.size() - sizeof(quint16));
	m_pTcpSocket->write(byteArray);
	m_pTcpSocket->flush();
	emit add_output_msg (QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz"), "sent", str);
}
void HostThread::run(){
	qDebug() << " Thread started";
	Log::Logger::trace("HOSTSIM", "HostThread started");
	m_pTcpSocket = new QTcpSocket();
	// set the ID
	if(m_pTcpSocket->setSocketDescriptor(this->m_SocketDescriptor)){
        connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
		connect(m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		qDebug() << m_SocketDescriptor << " Client connected";
		Log::Logger::trace("HOSTSIM", "HostThread client connected");
		send("unsolicited:HELLO");
		exec();
	} else {
		Log::Logger::traceV("HOSTSIM", "HostThread socket error: %s", m_pTcpSocket->errorString().toStdString().c_str());
		emit error(m_pTcpSocket->error());
	}
}
void HostThread::readyRead(){
	QByteArray data = m_pTcpSocket->readAll();
	qDebug() << m_SocketDescriptor << " Data in: " << data;
	Log::Logger::traceV("HOSTSIM", "HostThread::readyRead() data read: %s", data.toStdString().c_str());
	QString str(data);
	emit add_input_msg (QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss.zzz"), "read", str);
	QString cmdstr;
	QString argstr;

	int pos = str.indexOf(':');
	if (pos == -1){
		cmdstr = str;
	} else {
		cmdstr = str.left(pos);
		argstr = str.right(str.size() - pos - 1);
	}
	if (cmdstr == "logoff"){
		;
	} else if (cmdstr == "question"){
		QString answer;
		Utility::answer_question(&answer, argstr);
		send ("response:" + answer);
	} else if (cmdstr == "buy"){
		QString response;
		Utility::response_buy(&response, argstr);
		send ("response:" + response);
	} else if (cmdstr == "login"){
		send ("response:test ok");
	} else if (cmdstr == "buy_order"){
		send ("response:test ok");
	} else {
		send ("response:request unknown");
	}
//	m_pTcpSocket->write(Data);
}
void HostThread::disconnected(){
	qDebug() << m_SocketDescriptor << " Disconnected";
	Log::Logger::traceV("HOSTSIM", "HostThread::disconnected() discriptor: 0x%08x", m_SocketDescriptor);
	m_pTcpSocket->deleteLater();
	exit(0);
}

HostThread::HostThread(int socketDescriptor/*, const QString &msg*/, QObject *pParent)
: QThread(pParent)
, m_SocketDescriptor(socketDescriptor)
, m_pTcpSocket (nullptr)
{
}
