#ifndef QTSPOTIFY_GLOBAL_H
#define QTSPOTIFY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTSPOTIFY_LIBRARY)
#  define QTSPOTIFYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTSPOTIFYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTSPOTIFY_GLOBAL_H
