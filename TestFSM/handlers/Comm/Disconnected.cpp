#include "Disconnected.h" //INC fsm_dontedit
#include "Handshake.h" //INC fsm_dontedit
#include "./Framework/Commhdl.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Comm::Disconnected::NAME = "CommDisconnected"; //NME fsm_dontedit

void Comm::Disconnected::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("COMMHDL", "Disconnected::initialize ()");
    this->m_Handler.notify_Main (new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Disconnected, "disconnected"));
}  //MTD fsm_dontedit


StateComm* Comm::Disconnected::do_Comm_Error (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    this->m_Handler.notify_Main (new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Error, "error"));
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Disconnected::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = new Comm::Handshake(this->m_Handler);
	return pNewState;
//	pNewState = new Comm::Handshake(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Disconnected::do_Comm_Dataread (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Disconnected::do_Ctrl_Request (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Disconnected::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit


Comm::Disconnected::Disconnected(CommHdl& handler) //MTD fsm_dontedit
: StateComm (handler)
{
}  //MTD fsm_dontedit
