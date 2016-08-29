#ifndef IDLE_H
#define IDLE_H
#include "./handlers/Comm/Framework/StateComm.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Comm {
	class Idle : public StateComm {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateComm* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Request(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Message(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Response(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Dataread(StateModel::Msg *pMsg);
		virtual StateComm* do_Ctrl_Request(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Error(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Idle(CommHdl& handler);
	};
}
#endif // IDLE_H
