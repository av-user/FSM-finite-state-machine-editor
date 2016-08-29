#include "hosthelper.h"

bool HostHelper::getLogonReq (std::vector<unsigned char> *pV, QString user, QString pass){
	bool retval = false;
	//todo
	pV->clear();
	pV->push_back(0);
	pV->push_back(2);
	pV->push_back('\x0f');
	retval = true;
	return retval;
}
bool HostHelper::getLogonReq (QString *pStr, QString user, QString pass){
	bool retval = false;
	*pStr = "login:" + user + ';' + pass;
	retval = true;
	return retval;
}
bool HostHelper::getLogoffReq (QString *pStr){
	bool retval = false;
	*pStr = "logoff";
	retval = true;
	return retval;
}
bool HostHelper::getBuyorderReq (QString *pStr, const QString prodName, unsigned int prodID){
	bool retval = false;
	*pStr = "buy_order:" + prodID + ';' + prodName;
	retval = true;
	return retval;

}


HostHelper::HostHelper()
{

}
