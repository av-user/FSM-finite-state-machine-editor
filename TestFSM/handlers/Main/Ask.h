#ifndef ASK_H
#define ASK_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Ask : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Response(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Ask(MainHdl& handler);
	};
}
#endif // ASK_H
