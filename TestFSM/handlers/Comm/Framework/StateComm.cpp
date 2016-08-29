#include "stateComm.h"
#include <QDebug>
#include "../LogLib/logger.h"

StateComm* StateComm::do_Comm_Error(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Error msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Response(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Response msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Connected(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Connected msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Disconnected(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Disconnected msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Dataread(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Dataread msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Ctrl_Request(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Ctrl_Request msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Ctrl_Response(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Ctrl_Response msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Ctrl_Timeout(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Ctrl_Timeout msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Request(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Request msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Message(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Message msg not allowed");
	throw 0; //todo
}
StateComm* StateComm::do_Comm_Unsolicited(StateModel::Msg *pMsg){
	Log::Logger::trace("StateComm", "do_Comm_Unsolicited msg not allowed");
	throw 0; //todo
}


StateComm*	StateComm::process (StateModel::Msg *pMsg){

	StateComm *pNext_state = nullptr;

	StateModel::Msg::Type type = pMsg->type();
	Log::Logger::traceV("StateComm", "process msg: %s", pMsg->ToString().toStdString().c_str());
	switch (type) {
	case StateModel::Msg::Comm_Error:
		pNext_state = do_Comm_Error(pMsg);
		break;
	case StateModel::Msg::Comm_Response:
		pNext_state = do_Comm_Response(pMsg);
		break;
	case StateModel::Msg::Comm_Connected:
		pNext_state = do_Comm_Connected(pMsg);
		break;
	case StateModel::Msg::Comm_Disconnected:
		pNext_state = do_Comm_Disconnected(pMsg);
		break;
	case StateModel::Msg::Comm_Dataread:
		pNext_state = do_Comm_Dataread(pMsg);
		break;
	case StateModel::Msg::Ctrl_Request:
		pNext_state = do_Ctrl_Request(pMsg);
		break;
	case StateModel::Msg::Ctrl_Response:
		pNext_state = do_Ctrl_Response(pMsg);
		break;
	case StateModel::Msg::Ctrl_Timeout:
		pNext_state = do_Ctrl_Timeout(pMsg);
		break;
	case StateModel::Msg::Comm_Request:
		pNext_state = do_Comm_Request(pMsg);
		break;
	case StateModel::Msg::Comm_Message:
		pNext_state = do_Comm_Message(pMsg);
		break;
	case StateModel::Msg::Comm_Unsolicited:
		pNext_state = do_Comm_Unsolicited(pMsg);
		break;

	default:
		pNext_state = this;//todo!!!
		qDebug() << "StateComm::process() msg: " << pMsg->ToString();
	}
	return pNext_state;
}

StateComm::StateComm(CommHdl& handler)
: m_Handler (handler)
{

}
