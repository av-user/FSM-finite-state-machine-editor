#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
	Q_OBJECT
public:
	explicit TcpClient(QObject *parent = 0);
	bool setHandle (int descriptor);
	qint64 write(QByteArray *pByteArray);
//	bool flush();
signals:

public slots:
	void connected();
	void disconnected();
	void readyRead();
	void result (int number);
private:
	QTcpSocket *m_pTcpSocket;
};

#endif // TCPCLIENT_H
