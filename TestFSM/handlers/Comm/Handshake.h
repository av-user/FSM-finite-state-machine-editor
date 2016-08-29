#ifndef HANDSHAKE_H
#define HANDSHAKE_H
#include "./handlers/Comm/Framework/StateComm.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Comm {
	class Handshake : public StateComm {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateComm* do_Comm_Error(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateComm* do_Comm_Unsolicited(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Handshake(CommHdl& handler);
	};
}
#endif // HANDSHAKE_H
