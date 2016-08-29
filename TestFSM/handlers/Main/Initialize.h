#ifndef INITIALIZE_H
#define INITIALIZE_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Initialize : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Comm_Error(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateMain* do_Gui_SignonClk(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Unsolicited(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Initialize(MainHdl& handler);
	};
}
#endif // INITIALIZE_H
