#ifndef MAINHDL_H
#define MAINHDL_H

#include <QObject>
#include "statemodel/handler.h"

//### <<< custom include beginn
#include <QQmlApplicationEngine>
//### custom include end >>>


class StateModel::Msg;
class CommHdl;


class MainHdl: public StateModel::Handler {
        Q_OBJECT
//### <<< custom part beginn
private:
    StateModel::Msg* get_message (QString msgstr) const ;
signals:
    void sendToQml(QVariant cppdata);
public slots:
    void receiveFromQml(const QString &qmlstr);
public:
    void signal (QVariant cppdata);
//### custom part end >>>

public:

        void setComm (CommHdl *pComm);
        void notify_Comm(StateModel::Msg *pMsg);


    explicit MainHdl(QObject *pParent = 0);
private:
    virtual void clear();
    Q_DISABLE_COPY(MainHdl)
        CommHdl *m_pComm;

    virtual void initialize ();
};
#endif // MAINHDL_H
