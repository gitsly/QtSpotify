QT       += network multimedia

QT       -= gui

TARGET = QtSpotify
TEMPLATE = lib

DEFINES += QTSPOTIFY_LIBRARY

SOURCES += \
    src/spotifytrack.cpp

HEADERS += \
    include/QtSpotify/qtspotify_global.h \
    include/QtSpotify/spotifytrack.h \
    include/QtSpotify/spotifysession.h

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
