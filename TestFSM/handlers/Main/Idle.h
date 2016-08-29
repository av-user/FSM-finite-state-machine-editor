#ifndef IDLE_H
#define IDLE_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Idle : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Gui_Logoff(StateModel::Msg *pMsg);
		virtual StateMain* do_Gui_Question(StateModel::Msg *pMsg);
		virtual StateMain* do_Gui_Buy(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Idle(MainHdl& handler);
	};
}
#endif // IDLE_H
