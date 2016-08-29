#include "handlers/Main/Framework/Mainhdl.h" //INC fsm_dontedit
#include "Logon.h" //INC fsm_dontedit
#include "Initialize.h" //INC fsm_dontedit
#include "Idle.h" //INC fsm_dontedit
#include "../LogLib/logger.h"
#include "../UtilLib/util.h"
#include "utilities/host/hosthelper.h"
const char *Main::Logon::NAME = "MainLogon"; //NME fsm_dontedit

void Main::Logon::initialize (){  //MTD fsm_dontedit
	Log::Logger::trace("MAINHDL", "Logon::initialize ()");
    m_Handler.signal ("userpass");
}  //MTD fsm_dontedit


StateMain* Main::Logon::do_Gui_Userpass (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;

    QString str = pMsg->str();

    QString cmdstr;
    QString argstr;

    UtilLib::Util::splitCmd(&cmdstr, &argstr, str);
    if (cmdstr == "userpass"){
        if (argstr == "cancelled"){
            m_Handler.signal ("start");
            pNewState = new Main::Initialize (this->m_Handler);
        } else {
            argstr = pMsg->text();
            QStringList parmlist = argstr.split (";");
            if (parmlist.count() == 2 && parmlist.at(0).length() == 8 && parmlist.at(1).length() == 8){
                QString reqstr;
                if (HostHelper::getLogonReq (&reqstr, parmlist.at(0), parmlist.at(1))){
                    m_Handler.notify_Comm(new StateModel::Msg (&m_Handler, this, StateModel::Msg::Comm_Request, "request", reqstr));
                    pNewState = this;
                } else {
                    throw 0;//todo
                }
            } else {
                m_Handler.signal ("gui invalid");
                pNewState = this;
            }
        }
    } else {
        m_Handler.signal ("gui invalid");
        pNewState = this;
    }
	return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
//	pNewState = this;

}  //HDL fsm_dontedit

StateMain* Main::Logon::do_Comm_Disconnected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    pNewState = new Main::Initialize(this->m_Handler);
	return pNewState;
//	pNewState = new Main::Initialize(this->m_Handler);
}  //HDL fsm_dontedit

StateMain* Main::Logon::do_Comm_Connected (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
    pNewState = this;
	return pNewState;
//	pNewState = this;
}  //HDL fsm_dontedit

StateMain* Main::Logon::do_Comm_Response (StateModel::Msg *pMsg){  //HDL fsm_dontedit
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


Main::Logon::Logon(MainHdl& handler) //MTD fsm_dontedit
: StateMain (handler)
{
}  //MTD fsm_dontedit
StateMain* Main::Logon::do_Gui_Cancel (StateModel::Msg *pMsg){  //HDL fsm_dontedit
	StateMain *pNewState = nullptr;
	return pNewState;
}  //HDL fsm_dontedit

