#ifndef HOSTSERVER_H
#define HOSTSERVER_H

#include <QStringList>
#include <QTcpServer>

class MainWindow;

class HostServer: public QTcpServer {
	Q_OBJECT
public:
	HostServer(QObject *parent = 0);
	void setWindow (MainWindow *pWnd);
protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
	MainWindow *m_pWnd;
//	QStringList m_MsgList;
};

#endif // HOSTSERVER_H
