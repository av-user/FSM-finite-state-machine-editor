#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "Logon.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Main::Initialize::NAME = "MainInitialize"; //NME fsm_dontedit

void Main::Initialize::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Initialize::initialize ()");
}  //MTD fsm_dontedit


StateMain* Main::Initialize::do_Comm_Error (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Initialize::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    Log::Logger::trace("MAINHDL", "Initialize::do_Comm () connected");
    m_Handler.signal ("comm connected");
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Initialize::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    Log::Logger::trace("MAINHDL", "Initialize::do_Comm () disconnected");
    m_Handler.signal ("comm disconnected");
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Initialize::do_Gui_SignonClk (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    Log::Logger::trace("MAINHDL", "Initialize::do_Gui_SignonClk ()");
    pNewState = new Main::Logon(this->m_Handler);
	return pNewState;
//	pNewState = new Main::Logon(this->m_Handler);
//	pNewState = this;

}  //HDL fsm_dontedit

StateMain* Main::Initialize::do_Comm_Unsolicited (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    Log::Logger::trace("MAINHDL", "Initialize::do_Comm_Unsolicited ()");
    m_Handler.signal ("unsolicited message");
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit


Main::Initialize::Initialize(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
