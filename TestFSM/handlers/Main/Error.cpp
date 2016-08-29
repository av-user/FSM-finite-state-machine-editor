#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Error.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Main::Error::NAME = "MainError"; //NME fsm_dontedit

void Main::Error::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Error::initialize ()");
}  //MTD fsm_dontedit


StateMain* Main::Error::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Error::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Error::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Error::do_Comm_Unsolicited (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit


Main::Error::Error(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
