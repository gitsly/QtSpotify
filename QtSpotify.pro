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
    src/spotifyuser.cpp \
    src/spotifyplaylistcontainer.cpp \
    src/spotifyartistbrowse.cpp \
    src/spotifyalbumbrowse.cpp

HEADERS += \
    include/QtSpotify/qtspotify_global.h \
    include/QtSpotify/spotifytrack.h \
    include/QtSpotify/spotifysession.h \
    include/QtSpotify/spotifyalbum.h \
    include/QtSpotify/spotifyartist.h \
    include/QtSpotify/spotifyplaylist.h \
    include/QtSpotify/spotifyuser.h \
    include/QtSpotify/spotifyplaylistcontainer.h \
    include/QtSpotify/playlistcallbacks.h \
    include/QtSpotify/playlistcontainercallbacks.h \
    include/QtSpotify/playlistevents.h \
    include/QtSpotify/spotifyartistbrowse.h \
    include/QtSpotify/artistbrowsecallbacks.h \
    include/QtSpotify/spotifyalbumbrowse.h \
    include/QtSpotify/albumbrowsecallbacks.h

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
