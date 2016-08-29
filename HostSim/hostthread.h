#ifndef HOSTTHREAD_H
#define HOSTTHREAD_H


#include <QThread>
#include <QTcpSocket>

class HostThread : public QThread {
	Q_OBJECT

public:
	HostThread(int socketDescriptor, /*const QString &msg, */QObject *pParent);

	void run() Q_DECL_OVERRIDE;

signals:
	void error(QTcpSocket::SocketError socketError);
	void add_output_msg (QString s0, QString s1, QString s2);
	void add_input_msg (QString s0, QString s1, QString s2);

public slots:
	void readyRead();
	void disconnected();
private:
	void send (QString str);

	int				m_SocketDescriptor;
	QTcpSocket		*m_pTcpSocket;
};

#endif // HOSTTHREAD_H
