#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Show.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
#include "../UtilLib/util.h"
const char *Main::Show::NAME = "MainShow"; //NME fsm_dontedit

void Main::Show::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Show::initialize ()");
}  //MTD fsm_dontedit


StateMain* Main::Show::do_Gui_Showdone (StateModel::Msg *pMsg){  //HDL fsm_dontedit
    Log::Logger::trace("MAINHDL", "Show::do_Gui_Showdone ()");
    StateMain *pNewState = nullptr;
    QString str = pMsg->str();
    QString cmdstr;
    QString argstr;
    UtilLib::Util::splitCmd(&cmdstr, &argstr, str);
    if (cmdstr == "showdone"){
        m_Handler.signal ("mainmenu");
        pNewState = new Main::Idle (this->m_Handler);
    }
	return pNewState;
//	pNewState = new Main::Idle(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Show::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    m_Handler.signal ("comm disconnected");
    pNewState = new Main::Initialize(this->m_Handler);
    return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Show::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    m_Handler.signal ("comm connected");
    pNewState = this;
    return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Show::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    if (pMsg->text() == "test ok"){
        m_Handler.signal ("login successful");
        pNewState = new Main::Idle(this->m_Handler);
    } else {
        pNewState = new Main::Initialize(this->m_Handler);
    }
    return pNewState;
//	pNewState = new Main::Idle(this->m_Handler);
//	pNewState = new Main::Initialize(this->m_Handler);

}  //HDL fsm_dontedit


Main::Show::Show(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
