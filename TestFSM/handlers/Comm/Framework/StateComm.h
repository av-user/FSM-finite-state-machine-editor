#ifndef STATECOMM_H
#define STATECOMM_H

#include "statemodel/state.h"

class CommHdl;

class StateComm : public StateModel::State {
public:
	virtual StateComm* do_Comm_Error(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Response(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Connected(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Disconnected(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Dataread(StateModel::Msg *pMsg);
	virtual StateComm* do_Ctrl_Request(StateModel::Msg *pMsg);
	virtual StateComm* do_Ctrl_Response(StateModel::Msg *pMsg);
	virtual StateComm* do_Ctrl_Timeout(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Request(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Message(StateModel::Msg *pMsg);
	virtual StateComm* do_Comm_Unsolicited(StateModel::Msg *pMsg);


	StateComm(CommHdl& handler);
	virtual StateComm*	process (StateModel::Msg *pMsg);

protected:
	CommHdl	&m_Handler;
};

#endif // STATECOMM_H
