#include "tcpsocket.h"
#include "../LogLib/logger.h"
#include <QThread>
#include <QDataStream>
#include "statemodel/msg.h"

void TcpSocket::readyReadSlot(){
    qDebug() << "Reading...";
    QByteArray barray = readAll();
    qDebug() << "read: " << barray;
    m_ReadBufferArray.append(barray);
    int size = m_ReadBufferArray.size();
    if (m_ReadBufferArray.size() >= sizeof (quint16)){
        QDataStream in(&m_ReadBufferArray, QIODevice::ReadOnly);
        quint16 msglen = 0;
        in >> msglen;
        while (size >= msglen + sizeof (quint16)){
            QString msgstr;
			StateModel::Msg *pMsg = nullptr;
            in >> msgstr;
            m_ReadBufferArray.remove(0, msglen + sizeof (quint16));
            size = m_ReadBufferArray.size();
			int pos = msgstr.indexOf(':');
			if (pos == -1){
                pMsg = new StateModel::Msg ("TcpSocket", StateModel::Msg::Comm_Message, "message", msgstr);
			} else {
                pMsg = get_message (msgstr);
			}
            m_CommHdl.enqueue(pMsg);
        }
    }
}
StateModel::Msg* TcpSocket::get_message (QString msgstr){
    StateModel::Msg *pMsg = nullptr;
    int pos = msgstr.indexOf(':');
    if (pos == -1){
        pMsg = new StateModel::Msg ("TcpSocket", StateModel::Msg::Comm_Message, "message", msgstr);
    } else {
        QString left = msgstr.left(pos);
        StateModel::Msg::Type msgtype = StateModel::Msg::Comm_Message;
        if (left == "unsolicited"){
            msgtype = StateModel::Msg::Comm_Unsolicited;
        } else if (left == "response"){
            msgtype = StateModel::Msg::Comm_Response;
        } else if (left == "error"){
            msgtype = StateModel::Msg::Comm_Error;
        }
        pMsg = new StateModel::Msg ("TcpSocket", msgtype, msgstr.left(pos), msgstr.mid(pos+1));
    }
    return pMsg;
}
void TcpSocket::disconnectedSlot(){
    Log::Logger::trace("COMMSKT", "socket disconnected");
    qDebug() << "Disconnected!";
    m_CommHdl.enqueue(new StateModel::Msg ("TcpSocket", StateModel::Msg::Comm_Disconnected, "disconnected"));
    QThread::usleep(10000000);
    if (state() == QTcpSocket::UnconnectedState){
		connectToHost(m_Address, m_Port);
    }
}
void TcpSocket::connectedSlot(){
    Log::Logger::trace("COMMSKT", "TcpSocket::connectedSlot()");
    qDebug() << "TcpSocket::connectedSlot()";
    m_CommHdl.enqueue(new StateModel::Msg ("TcpSocket", StateModel::Msg::Comm_Connected, "connected"));
}
void TcpSocket::errorSlot(QAbstractSocket::SocketError socketError){
    Log::Logger::traceV("COMMSKT", "TcpSocket::error() %s", errorString().toStdString().c_str());
    m_CommHdl.enqueue(new StateModel::Msg ("TcpSocket", StateModel::Msg::Comm_Error, "error"));
    qDebug() << "Socket error: " << socketError;
    QThread::usleep(10000000);
    if (state() == QTcpSocket::UnconnectedState){
		connectToHost(m_Address, m_Port);
    }
}
void TcpSocket::disconnect(){
    Log::Logger::trace("COMMSKT", "TcpSocket::disconnect()");
    qDebug() << "this thread: " << this->thread() << ", current thread:" << QThread::currentThread();
    this->disconnectFromHost();
}
qint64 TcpSocket::writeSlot(QString msgstr){
    Log::Logger::traceV("COMMSKT", "TcpSocket::write() msgstr: %s", msgstr.toStdString().c_str());
    qint64 count = this->write(msgstr.toStdString().c_str());
	Log::Logger::traceV("COMMSKT", "TcpSocket::writeSlot() %u written", count);
    return count;
}
void TcpSocket::connect(QString address, unsigned long port){
    Log::Logger::trace("COMMSKT", "TcpSocket::connect()");
	m_Address = address;
	m_Port = port;
	qDebug() << "this thread: " << this->thread() << ", current thread:" << QThread::currentThread();
    this->connectToHost(address, port);
}

TcpSocket::TcpSocket(CommHdl &commHdl, QString address, unsigned long port, QObject *pParent)
: QTcpSocket(pParent)
, m_CommHdl (commHdl)
, m_Address (address)
, m_Port	(port)
{

}
