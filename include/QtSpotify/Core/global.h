#ifndef QTSPOTIFY_GLOBAL_H
#define QTSPOTIFY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTSPOTIFY_LIBRARY)
#  define QTS_EXPORT Q_DECL_EXPORT
#else
#  define QTS_EXPORT Q_DECL_IMPORT
#endif

template <class T, class U>
bool exchange(T& obj, U&& newValue) {
    if(obj != newValue) {
        obj = std::forward<U>(newValue);
        return true;
    }

    return false;
}

#endif // QTSPOTIFY_GLOBAL_H
