#ifndef STATEMAIN_H
#define STATEMAIN_H

#include "statemodel/state.h"

class MainHdl;

class StateMain : public StateModel::State {
public:
	virtual StateMain* do_Comm_Disconnected(StateModel::Msg *pMsg);
	virtual StateMain* do_Comm_Connected(StateModel::Msg *pMsg);
	virtual StateMain* do_Comm_Response(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Logoff(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Question(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Buy(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Userpass(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Cancel(StateModel::Msg *pMsg);
	virtual StateMain* do_Comm_Error(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_SignonClk(StateModel::Msg *pMsg);
	virtual StateMain* do_Comm_Unsolicited(StateModel::Msg *pMsg);
	virtual StateMain* do_Gui_Showdone(StateModel::Msg *pMsg);


	StateMain(MainHdl& handler);
	virtual StateMain*	process (StateModel::Msg *pMsg);

protected:
	MainHdl	&m_Handler;
};

#endif // STATEMAIN_H
