#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include "Framework/commhdl.h"

class TcpSocket : public QTcpSocket{
    Q_OBJECT
public:
	explicit TcpSocket(CommHdl &commHdl, QString address, unsigned long port, QObject *pParent = 0);
signals:

public slots:
    void disconnect();
    void connectedSlot();
    void disconnectedSlot();
    void readyReadSlot();
    void errorSlot(QAbstractSocket::SocketError socketError);
    void connect(QString address, unsigned long port);
	qint64 writeSlot(QString msgstr);
private:
    StateModel::Msg *get_message (QString str);
    CommHdl &m_CommHdl;
    QByteArray		m_ReadBufferArray;
	QString			m_Address;
	unsigned long	m_Port;
};

#endif // TCPSOCKET_H
