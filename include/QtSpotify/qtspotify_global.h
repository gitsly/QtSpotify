#ifndef QTSPOTIFY_GLOBAL_H_
#define QTSPOTIFY_GLOBAL_H_

#include <QtCore/qglobal.h>
#include <utility>

#if defined(QTSPOTIFY_LIBRARY)
#  define QTSPOTIFYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTSPOTIFYSHARED_EXPORT Q_DECL_IMPORT
#endif

template <class T, class U>
bool exchange(T& obj, U&& newValue) {
    if(obj != newValue) {
        obj = std::forward<U>(newValue);
        return true;
    }

    return false;
}


#endif
