#include "Commhdl.h"
#include <QDebug>
#include "statemodel/msg.h"
#include "../LogLib/logger.h"
#include "handlers/Main/Framework/Mainhdl.h"

//### <<< custom include beginn
#include "handlers/Comm/disconnected.h"
#include "../clientthread.h"
//### custom include end >>>

//### <<< custom part beginn
void CommHdl::tcpEvent (QString str){
    if (str == "error"){

    } else {

    }
}
void CommHdl::disconnect(){
    Log::Logger::trace("COMMHDL", "disconnect()");
    m_pClientThread->disconnectFromHost();
}
 void CommHdl::send (QString msgstr){
    Log::Logger::traceV("COMMHDL", "send() msgstr: %s", msgstr.toStdString().c_str());
    m_pClientThread->write(msgstr);
}
//### custom part end >>>

void CommHdl::initialize(){
}

void CommHdl::setMain(MainHdl *pMain){
	m_pMain = pMain;
}
void CommHdl::notify_Main(StateModel::Msg *pMsg) {
	if (m_pMain != nullptr) {
		m_pMain->enqueue(pMsg);
	}
}

void CommHdl::clear(){
}

CommHdl::CommHdl(QObject *pParent) //MTD fsm_dontedit
: StateModel::Handler ("CommHdl", "1.0", pParent)
{
} //MTD fsm_dontedit

