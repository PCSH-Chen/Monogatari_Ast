#ifndef BASEINFO_GLOBAL_H
#define BASEINFO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BASEINFO_LIBRARY)
#define BASEINFO_EXPORT Q_DECL_EXPORT
#else
#define BASEINFO_EXPORT Q_DECL_IMPORT
#endif

#endif // BASEINFO_GLOBAL_H
