QT       += network multimedia
QT       -= gui

TARGET = QtSpotify
TEMPLATE = lib

MOC_DIR = mocs
OBJECTS_DIR = objs
UI_DIR = uis

DEFINES += QTSPOTIFY_LIBRARY

INCLUDEPATH += include
win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify
INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include

SOURCES += \
    src/album.cpp \
    src/albumbrowse.cpp \
    src/artist.cpp \
    src/artistbrowse.cpp \
    src/playlist.cpp \
    src/playlistcontainer.cpp \
    src/playqueue.cpp \
    src/search.cpp \
    src/spotify.cpp \
    src/track.cpp \
    src/deleters.cpp \
    src/user.cpp

HEADERS += \
    include/QtSpotify/Core/album.h \
    include/QtSpotify/Core/albumbrowse.h \
    include/QtSpotify/Core/artist.h \
    include/QtSpotify/Core/artistbrowse.h \
    include/QtSpotify/Core/playlist.h \
    include/QtSpotify/Core/playlistcontainer.h \
    include/QtSpotify/Core/playqueue.h \
    include/QtSpotify/Core/search.h \
    include/QtSpotify/Core/spotify.h \
    include/QtSpotify/Core/track.h \
    include/QtSpotify/Core/global.h \
    include/QtSpotify/Core/deleters.h \
    include/QtSpotify/Core/user.h \
    include/QtSpotify/Core/playlistevents.h \
    include/QtSpotify/Core/playlistcontainerevents.h \
    include/QtSpotify/Core/apikey.h

win32 {
    if(isEmpty(PREFIX)) {
        PREFIX = "C:/workspaces-local/libraries/$$TARGET"
    }

    BINDIR = $$PREFIX/lib
    INCDIR = $$PREFIX/include
}

unix {

}


target.path = $$BINDIR
INSTALLS += target

QMAKE_POST_LINK = $$quote(xcopy /s /q /y /i $$system_path($$absolute_path(include)) $$system_path($$INCDIR))
message($$QMAKE_POST_LINK)
