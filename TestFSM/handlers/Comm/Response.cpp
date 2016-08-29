#include "handlers/Comm/Framework/Commhdl.h" //INC fsm_dontedit
#include "Response.h" //INC fsm_dontedit
#include "Disconnected.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Comm::Response::NAME = "CommResponse"; //NME fsm_dontedit

void Comm::Response::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("COMMHDL", "Response::initialize ()");
    m_Handler.startTimer(3000);
}  //MTD fsm_dontedit


StateComm* Comm::Response::do_Comm_Error (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Comm_Error ()");
    this->m_Handler.notify_Main (new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Error, "error"));
    pNewState = new Comm::Disconnected(m_Handler);
	return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Comm_Response ()");
    m_Handler.stopTimer();
    this->m_Handler.notify_Main (new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Response, pMsg->str(), pMsg->text()));
    pNewState = new Comm::Idle(m_Handler);
    return pNewState;
//	pNewState = new Comm::Idle(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Comm_Connected () todo");
    pNewState = new Comm::Idle(m_Handler);
    return pNewState;
//	pNewState = new Comm::Idle(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Comm_Disconnected ()");
    pNewState = new Comm::Disconnected(m_Handler);
    return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Comm_Dataread (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Comm_Dataread ()");
    //todo
    StateModel::Msg *pMessage = new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Dataread, "dataread");
    pMsg->setVector(&pMessage->vect());
    m_Handler.notify_Main(pMessage);
    pNewState = new Comm::Idle(m_Handler);
    return pNewState;
//	pNewState = new Comm::Idle(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Ctrl_Request (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Ctrl_Request () todo");
    pNewState = this;
    return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Ctrl_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Ctrl_Response ()");
    m_Handler.notify_Main(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Disconnected, "disconnected"));
    pNewState = this;
    return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Response::do_Ctrl_Timeout (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Response::do_Ctrl_Timeout ()");
    m_Handler.notify_Main(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Disconnected, "disconnected"));
    pNewState = new Comm::Idle(m_Handler);
    return pNewState;
//	pNewState = new Comm::Idle(this->m_Handler);
}  //HDL fsm_dontedit


Comm::Response::Response(CommHdl& handler) //MTD fsm_dontedit
: StateComm (handler)
{
}  //MTD fsm_dontedit
