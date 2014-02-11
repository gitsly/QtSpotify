QT       += network multimedia

QT       -= gui

TARGET = QtSpotify
TEMPLATE = lib

DEFINES += QTSPOTIFY_LIBRARY

SOURCES += \

HEADERS += \

INCLUDEPATH += include

win32: LIBS += -L$$PWD/../libraries/libspotify/lib/ -llibspotify

INCLUDEPATH += $$PWD/../libraries/libspotify/include
DEPENDPATH += $$PWD/../libraries/libspotify/include
