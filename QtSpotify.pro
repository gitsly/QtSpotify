QT       += network multimedia

QT       -= gui

TARGET = QtSpotify
TEMPLATE = lib

DEFINES += QTSPOTIFY_LIBRARY

SOURCES += \
        src/qtspotify_global.cpp \
        src/album.cpp \
        src/albumbrowse.cpp \
        src/artist.cpp \
        src/artistbrowse.cpp \
        src/playlist.cpp \
        src/playqueue.cpp \
        src/search.cpp \
        src/session.cpp \
        src/toplist.cpp \
        src/track.cpp \
    src/audioplayer.cpp

HEADERS += \
        include/QtSpotify/qtspotify_global.h \
        include/QtSpotify/album.h \
        include/QtSpotify/albumbrowse.h \
        include/QtSpotify/artist.h \
        include/QtSpotify/artistbrowse.h \
        include/QtSpotify/playlist.h \
        include/QtSpotify/playqueue.h \
        include/QtSpotify/search.h \
        include/QtSpotify/session.h \
        include/QtSpotify/toplist.h \
        include/QtSpotify/track.h \
        sessionevents.h \
        sessioncallbacks.h \
    audioplayer.h

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
