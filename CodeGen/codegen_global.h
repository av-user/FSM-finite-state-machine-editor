#ifndef CODEGEN_GLOBAL_H
#define CODEGEN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CODEGEN_LIBRARY)
#  define CODEGENSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CODEGENSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CODEGEN_GLOBAL_H
