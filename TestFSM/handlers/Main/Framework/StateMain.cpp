#include "stateMain.h"
#include <QDebug>
#include "../LogLib/logger.h"

StateMain* StateMain::do_Comm_Disconnected(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Comm_Disconnected msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Comm_Connected(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Comm_Connected msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Comm_Response(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Comm_Response msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Logoff(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Logoff msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Question(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Question msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Buy(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Buy msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Userpass(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Userpass msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Cancel(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Cancel msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Comm_Error(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Comm_Error msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_SignonClk(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_SignonClk msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Comm_Unsolicited(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Comm_Unsolicited msg not allowed");
	throw 0; //todo
}
StateMain* StateMain::do_Gui_Showdone(StateModel::Msg *pMsg){
	Log::Logger::trace("StateMain", "do_Gui_Showdone msg not allowed");
	throw 0; //todo
}


StateMain*	StateMain::process (StateModel::Msg *pMsg){

	StateMain *pNext_state = nullptr;

	StateModel::Msg::Type type = pMsg->type();
	Log::Logger::traceV("StateMain", "process msg: %s", pMsg->ToString().toStdString().c_str());
	switch (type) {
	case StateModel::Msg::Comm_Disconnected:
		pNext_state = do_Comm_Disconnected(pMsg);
		break;
	case StateModel::Msg::Comm_Connected:
		pNext_state = do_Comm_Connected(pMsg);
		break;
	case StateModel::Msg::Comm_Response:
		pNext_state = do_Comm_Response(pMsg);
		break;
	case StateModel::Msg::Gui_Logoff:
		pNext_state = do_Gui_Logoff(pMsg);
		break;
	case StateModel::Msg::Gui_Question:
		pNext_state = do_Gui_Question(pMsg);
		break;
	case StateModel::Msg::Gui_Buy:
		pNext_state = do_Gui_Buy(pMsg);
		break;
	case StateModel::Msg::Gui_Userpass:
		pNext_state = do_Gui_Userpass(pMsg);
		break;
	case StateModel::Msg::Gui_Cancel:
		pNext_state = do_Gui_Cancel(pMsg);
		break;
	case StateModel::Msg::Comm_Error:
		pNext_state = do_Comm_Error(pMsg);
		break;
	case StateModel::Msg::Gui_SignonClk:
		pNext_state = do_Gui_SignonClk(pMsg);
		break;
	case StateModel::Msg::Comm_Unsolicited:
		pNext_state = do_Comm_Unsolicited(pMsg);
		break;
	case StateModel::Msg::Gui_Showdone:
		pNext_state = do_Gui_Showdone(pMsg);
		break;

	default:
		pNext_state = this;//todo!!!
		qDebug() << "StateMain::process() msg: " << pMsg->ToString();
	}
	return pNext_state;
}

StateMain::StateMain(MainHdl& handler)
: m_Handler (handler)
{

}
