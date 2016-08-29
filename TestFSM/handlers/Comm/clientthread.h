#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "tcpsocket.h"
#include "Framework/commhdl.h"

class ClientThread : public QThread {
	static const char *HOST_IP;
	static const int HOST_PORT;
	Q_OBJECT

public:
	void write(QString str);

    explicit ClientThread(CommHdl &commHdl/*, QObject *pParent*/);

	void run() Q_DECL_OVERRIDE;
signals:
	void disconnectSocket ();
	void send(QString str);
	void connectSocket (QString address, unsigned long port);

public slots:
	void error(QAbstractSocket::SocketError socketError);
	void connected();
	void disconnected();
	void bytesWritten(qint64 bytes);
	void readyRead();
	void disconnectFromHost();
private:

	QByteArray		m_ReadBufferArray;
    TcpSocket		*m_pTcpSocket;
    CommHdl			&m_CommHdl;
};

#endif // CLIENTTHREAD_H
