QT       += network multimedia

QT       -= gui

TARGET = QtSpotify
TEMPLATE = lib

DEFINES += QTSPOTIFY_LIBRARY

SOURCES += \
    src/spotifytrack.cpp \
    src/spotifyalbum.cpp \
    src/spotifyartist.cpp \
    src/spotifysession.cpp \
    src/spotifyplaylist.cpp \
    src/spotifyuser.cpp

HEADERS += \
    include/QtSpotify/qtspotify_global.h \
    include/QtSpotify/spotifytrack.h \
    include/QtSpotify/spotifysession.h \
    include/QtSpotify/spotifyalbum.h \
    include/QtSpotify/spotifyartist.h \
    include/QtSpotify/spotifyplaylist.h \
    playlistcallbacks.h \
    playlistevents.h \
    include/QtSpotify/spotifyuser.h

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
