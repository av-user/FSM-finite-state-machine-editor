#include "handlers/Comm/Framework/Commhdl.h" // INC fsm_dontedit
#include "Handshake.h" //INC fsm_dontedit
#include "Disconnected.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Comm::Handshake::NAME = "CommHandshake"; //NME fsm_dontedit

void Comm::Handshake::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("COMMHDL", "Handshake::initialize ()");
}  //MTD fsm_dontedit


StateComm* Comm::Handshake::do_Comm_Error (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = new Comm::Disconnected(this->m_Handler);
	return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Handshake::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateComm* Comm::Handshake::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    pNewState = new Comm::Disconnected(this->m_Handler);
	return pNewState;
//	pNewState = new Comm::Disconnected(this->m_Handler);
}  //HDL fsm_dontedit

StateComm* Comm::Handshake::do_Comm_Unsolicited (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateComm *pNewState = nullptr;
    Log::Logger::trace("COMMHDL", "Handshake::do_Comm_Unsolicited () message");
    if (pMsg->text().compare("HELLO") == 0){
        this->m_Handler.notify_Main (new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Connected, "connected"));
        pNewState = new Comm::Idle(this->m_Handler);
    } else {
        Log::Logger::trace("COMMHDL", "Handshake::do_Comm () handshake failed, disconnecting...");
        m_Handler.disconnect();
        pNewState = this; //???
    }
	return pNewState;
//	pNewState = new Comm::Idle(this->m_Handler);//	pNewState = this;
}  //HDL fsm_dontedit


Comm::Handshake::Handshake(CommHdl& handler) //MTD fsm_dontedit
: StateComm (handler)
{
}  //MTD fsm_dontedit
