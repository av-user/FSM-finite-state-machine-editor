#include "Mainhdl.h"
#include <QDebug>
#include "statemodel/msg.h"
#include "../LogLib/logger.h"
#include "handlers/Comm/Framework/Commhdl.h"

//### <<< custom include beginn
#include "handlers/Main/initialize.h"
//### custom include end >>>

//### <<< custom part beginn
void MainHdl::signal (QVariant cppdata){
    emit sendToQml(cppdata);
}
StateModel::Msg* MainHdl::get_message (QString msgstr) const {
    StateModel::Msg *pMsg = nullptr;
    int pos = msgstr.indexOf(':');
    if (pos == -1){
        if (msgstr == "signonclicked"){
            pMsg = new StateModel::Msg ("MainHdl", StateModel::Msg::Gui_SignonClk, msgstr, msgstr);
        } else if (msgstr == "logoff"){
            pMsg = new StateModel::Msg ("MainHdl", StateModel::Msg::Gui_Logoff, msgstr, msgstr);
        } else if (msgstr == "showdone"){
            pMsg = new StateModel::Msg ("MainHdl", StateModel::Msg::Gui_Showdone, msgstr, msgstr);
        } else if (msgstr == "cancelled"){
            pMsg = new StateModel::Msg ("MainHdl", StateModel::Msg::Gui_Cancel, msgstr, msgstr);
        }
//        pMsg = new StateModel::Msg ("MainHdl", StateModel::Msg::Gui_Logoff, msgstr, msgstr);
    } else {
        QString left = msgstr.left(pos);
        StateModel::Msg::Type msgtype = StateModel::Msg::Comm_Message;
        if (left == "userpass"){
            msgtype = StateModel::Msg::Gui_Userpass;
        } else if (left == "response"){
            msgtype = StateModel::Msg::Comm_Response;
        } else if (left == "error"){
            msgtype = StateModel::Msg::Comm_Error;
        } else if (left == "question"){
            msgtype = StateModel::Msg::Gui_Question;
        } else if (left == "buy"){
            msgtype = StateModel::Msg::Gui_Buy;
        }
        pMsg = new StateModel::Msg ("MainHdl", msgtype, msgstr.left(pos), msgstr.mid(pos+1));
    }
    return pMsg;
}
void MainHdl::receiveFromQml(const QString &qmlstr){
    QString cmdstr;
    QString argstr;
    int pos = qmlstr.indexOf(':');
    if (pos == -1){
        cmdstr = qmlstr;
    } else {
        cmdstr = qmlstr.mid(pos+1);
        argstr = qmlstr.right(qmlstr.size() - pos);
    }

    if (cmdstr == "signonclicked"){
        emit sendToQml ("userpass");
    } else if (cmdstr == "userpass"){
        if (argstr == "cancelled"){
            emit sendToQml ("start");
        }
    } else if (cmdstr == "question"){
        ;
    } else {
        emit sendToQml ("blablabla");
    }
    StateModel::Msg *pMsg = this->get_message(qmlstr);
    this->enqueue(pMsg);
}
//### custom part end >>>

void MainHdl::initialize(){
}

void MainHdl::setComm(CommHdl *pComm){
	m_pComm = pComm;
}
void MainHdl::notify_Comm(StateModel::Msg *pMsg) {
	if (m_pComm != nullptr) {
		m_pComm->enqueue(pMsg);
	}
}

void MainHdl::clear(){
}

MainHdl::MainHdl(QObject *pParent) //MTD fsm_dontedit
: StateModel::Handler ("MainHdl", "1.0", pParent)
{
} //MTD fsm_dontedit

