#ifndef UTIL_H
#define UTIL_H

#include "utillib_global.h"

class QString;

namespace UtilLib {

	class UTILLIBSHARED_EXPORT Util{

	public:
		static void splitCmd (QString *pCmdstr, QString *pArgstr, const QString &str);
		Util();
	};
}

#endif // UTIL_H
