QT       += network multimedia

QT       -= gui

OBJECTS_DIR = objs
MOC_DIR = mocs
UI_DIR = uis

TARGET = QtSpotify
TEMPLATE = lib

DEFINES += QTSPOTIFY_LIBRARY

SOURCES += \
    src/album.cpp \
    src/albumbrowse.cpp \
    src/artist.cpp \
    src/artistbrowse.cpp \
    src/playlist.cpp \
    src/playlistcontainer.cpp \
    src/search.cpp \
    src/track.cpp \
    src/user.cpp \
    src/session.cpp \
    src/audioplayer.cpp

HEADERS += \
    include/QtSpotify/Album \
    include/QtSpotify/AlbumBrowse \
    include/QtSpotify/Artist \
    include/QtSpotify/ArtistBrowse \
    include/QtSpotify/Playlist \
    include/QtSpotify/PlaylistContainer \
    include/QtSpotify/Search \
    include/QtSpotify/Session \
    include/QtSpotify/Track \
    include/QtSpotify/User \
    include/QtSpotify/Core/album.h \
    include/QtSpotify/Core/albumbrowse.h \
    include/QtSpotify/Core/artist.h \
    include/QtSpotify/Core/artistbrowse.h \
    include/QtSpotify/Core/global.h \
    include/QtSpotify/Core/playlist.h \
    include/QtSpotify/Core/playlistcontainer.h \
    include/QtSpotify/Core/search.h \
    include/QtSpotify/Core/session.h \
    include/QtSpotify/Core/track.h \
    include/QtSpotify/Core/user.h \
    include/QtSpotify/Core/playlistcontainercallbacks.h \
    include/QtSpotify/Core/searchcallbacks.h \
    include/QtSpotify/Core/playlistcallbacks.h \
    include/QtSpotify/Core/albumbrowsecallbacks.h \
    include/QtSpotify/Core/artistbrowsecallbacks.h \
    include/QtSpotify/Audio/audioplayer.h \
    include/QtSpotify/Audio/audioevents.h \
    include/QtSpotify/Core/sessioncallbacks.h \
    apikey.h

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
