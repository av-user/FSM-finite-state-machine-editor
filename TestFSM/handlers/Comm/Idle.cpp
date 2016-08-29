#include "handlers/Comm/Framework/Commhdl.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "Disconnected.h" //INC fsm_dontedit
#include "Response.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Comm::Idle::NAME = "CommIdle"; //NME fsm_dontedit

void Comm::Idle::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("COMMHDL", "Idle::initialize ()");
}  //MTD fsm_dontedit


StateComm* Comm::Idle::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = new Comm::Disconnected(this->m_Handler);
    return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Request (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    QString str = pMsg->text();
    m_Handler.send (str);
    pNewState = new Comm::Response(this->m_Handler);
	return pNewState;
//	pNewState = new Comm::Response(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Message (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    QString str = pMsg->text();
    m_Handler.send (str);
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Dataread (StateModel::Msg *pMsg){  //HDL fsm_dontedit
    StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Idle::do_Comm () dataread");
    StateModel::Msg *pMessage = new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Unsolicited, "unsolicited");
    pMsg->setVector(&pMessage->vect());
    m_Handler.notify_Main(pMessage);
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Ctrl_Request (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
}  //HDL fsm_dontedit

StateComm* Comm::Idle::do_Comm_Error (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = new Comm::Disconnected(this->m_Handler);
    return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit


Comm::Idle::Idle(CommHdl& handler) //MTD fsm_dontedit
: StateComm (handler)
{
}  //MTD fsm_dontedit
