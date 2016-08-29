#ifndef BUY_H
#define BUY_H
#include "./handlers/Main/Framework/StateMain.h"
//### <<< custom include beginn
//### custom include end >>>
namespace Main {
	class Buy : public StateMain {
//### <<< custom part beginn
//### custom part end >>>
		static const char *NAME;
	public:
		virtual void	initialize ();
		virtual StateMain* do_Comm_Response(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
		virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);

		virtual QString name() const { return NAME; }
		Buy(MainHdl& handler);
	};
}
#endif // BUY_H
