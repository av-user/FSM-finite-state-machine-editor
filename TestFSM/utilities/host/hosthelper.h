#ifndef HOSTHELPER_H
#define HOSTHELPER_H

#include <QString>
#include <vector>

class HostHelper {
public:
	static bool getLogonReq (std::vector<unsigned char> *pV, QString user, QString pass);
	static bool getLogonReq (QString *pStr, QString user, QString pass);
	static bool getLogoffReq (QString *pStr);
	static bool getBuyorderReq (QString *pStr, const QString prodName, unsigned int prodID);

	HostHelper();
};

#endif // HOSTHELPER_H
