#ifndef UIHOME_GLOBAL_H
#define UIHOME_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UIHOME_LIBRARY)
#  define UIHOMESHARED_EXPORT Q_DECL_EXPORT
#else
#  define UIHOMESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UIHOME_GLOBAL_H

