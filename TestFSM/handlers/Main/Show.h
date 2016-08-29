#ifndef SHOW_H
#define SHOW_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Show : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Gui_Showdone(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Response(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Show(MainHdl& handler);
	};
}
#endif // SHOW_H
