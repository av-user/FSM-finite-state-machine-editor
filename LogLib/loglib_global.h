#ifndef LOGLIB_GLOBAL_H
#define LOGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGLIB_LIBRARY)
#  define LOGLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGLIB_GLOBAL_H
