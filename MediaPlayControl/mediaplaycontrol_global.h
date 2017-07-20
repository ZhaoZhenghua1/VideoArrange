#ifndef MEDIAPLAYCONTROL_GLOBAL_H
#define MEDIAPLAYCONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MEDIAPLAYCONTROL_LIB
# define MEDIAPLAYCONTROL_EXPORT Q_DECL_EXPORT
#else
# define MEDIAPLAYCONTROL_EXPORT Q_DECL_IMPORT
#endif

#endif // MEDIAPLAYCONTROL_GLOBAL_H