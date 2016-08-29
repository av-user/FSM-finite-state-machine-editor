#ifndef LOGON_H
#define LOGON_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Logon : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Gui_Userpass(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Response(StateModel::Msg *pMsg);
		virtual StateMain* do_Gui_Cancel(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Logon(MainHdl& handler);
	};
}
#endif // LOGON_H
