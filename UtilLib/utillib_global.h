#ifndef UTILLIB_GLOBAL_H
#define UTILLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILLIB_LIBRARY)
#  define UTILLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILLIB_GLOBAL_H
