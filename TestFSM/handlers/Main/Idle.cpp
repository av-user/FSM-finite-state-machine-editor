#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "Ask.h" //INC fsm_dontedit
#include "Buy.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
#include "utilities/host/hosthelper.h"
const char *Main::Idle::NAME = "MainIdle"; //NME fsm_dontedit

void Main::Idle::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Idle::initialize ()");
}  //MTD fsm_dontedit


StateMain* Main::Idle::do_Gui_Logoff (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    if (HostHelper::getLogoffReq (&pMsg->str())){
        m_Handler.notify_Comm(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Message, "message", pMsg->str()));
        pNewState = new Main::Initialize(this->m_Handler);
        m_Handler.signal ("logged off");
    } else {
        throw 0;//todo
    }
	return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Idle::do_Gui_Question (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    m_Handler.notify_Comm(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Request, "request", pMsg->str()));
    pNewState = new Main::Ask(this->m_Handler);
	return pNewState;
//	pNewState = new Main::Ask(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Idle::do_Gui_Buy (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    m_Handler.notify_Comm(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Request, "request", pMsg->str()));
    pNewState = new Main::Buy(this->m_Handler);
    return pNewState;
//	pNewState = new Main::Buy(this->m_Handler);
}  //HDL fsm_dontedit


Main::Idle::Idle(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
