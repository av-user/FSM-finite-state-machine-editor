#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Buy.h" //INC fsm_dontedit
#include "Show.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
const char *Main::Buy::NAME = "MainBuy"; //NME fsm_dontedit

void Main::Buy::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Buy::initialize ()");
}  //MTD fsm_dontedit


StateMain* Main::Buy::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
    Log::Logger::trace("MAINHDL", "Buy::do_Comm_Response ()");
    StateMain *pNewState = nullptr;
    m_Handler.signal ("show:" + pMsg->text());
    pNewState = new Main::Show(this->m_Handler);
    return pNewState;
//	pNewState = new Main::Show(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Buy::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
    Log::Logger::trace("MAINHDL", "Buy::do_Comm_Disconnected ()");
	StateMain *pNewState = nullptr;
    m_Handler.signal ("comm disconnected");
    pNewState = new Main::Initialize(this->m_Handler);
    return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Buy::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
    Log::Logger::trace("MAINHDL", "Buy::do_Comm_Connected ()");
	StateMain *pNewState = nullptr;
    m_Handler.signal ("comm connected");
    pNewState = this;
    return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit


Main::Buy::Buy(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
