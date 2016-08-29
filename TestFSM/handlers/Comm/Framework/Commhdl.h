#ifndef COMMHDL_H
#define COMMHDL_H

#include <QObject>
#include "statemodel/handler.h"

//### <<< custom include beginn
class ClientThread;
//### custom include end >>>


class StateModel::Msg;
class MainHdl;


class CommHdl: public StateModel::Handler {
        Q_OBJECT
//### <<< custom part beginn
public:
    void tcpEvent	(QString str);
    void disconnect	();
    void send		(QString msgstr);
private:
    ClientThread *m_pClientThread;
signals:
//### custom part end >>>

public:

        void setMain (MainHdl *pMain);
        void notify_Main(StateModel::Msg *pMsg);


    explicit CommHdl(QObject *pParent = 0);
private:
    virtual void clear();
    Q_DISABLE_COPY(CommHdl)
        MainHdl *m_pMain;

    virtual void initialize ();
};
#endif // COMMHDL_H
