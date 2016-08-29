#include "util.h"
#include <QString>

void UtilLib::Util::splitCmd (QString *pCmdstr, QString *pArgstr, const QString &str){
	int pos = str.indexOf(':');
	if (pos == -1){
		*pCmdstr = str;
		*pArgstr = "";
	} else {
		*pCmdstr = str.left(pos);
		*pArgstr = str.right(str.size() - pos - 1);
	}
}


UtilLib::Util::Util(){}
