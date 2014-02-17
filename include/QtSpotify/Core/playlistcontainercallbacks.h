#ifndef QTSPOTIFY_PLAYLISTCONTAINERCALLBACKS_H_
#define QTSPOTIFY_PLAYLISTCONTAINERCALLBACKS_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/qglobal.h>
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtSpotify/PlaylistContainer>
#include <libspotify/api.h>

class QTS_EXPORT PlaylistAddedEvent : public QEvent
{

public:

    PlaylistAddedEvent(sp_playlist* playlist, qint32 position) :
        QEvent(QEvent::Type(QEvent::User)),
        m_playlist(playlist),
        m_position(position)
    { }

    sp_playlist* playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }

private:

    sp_playlist* m_playlist;
    qint32 m_position;

};

class QTS_EXPORT PlaylistRemovedEvent : public QEvent
{

public:

    PlaylistRemovedEvent(sp_playlist* playlist, qint32 position) :
        QEvent(QEvent::Type(QEvent::User + 1)),
        m_playlist(playlist),
        m_position(position)
    { }

    sp_playlist* playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }

private:

    sp_playlist* m_playlist;
    qint32 m_position;

};

class QTS_EXPORT PlaylistMovedEvent : public QEvent
{

public:

    PlaylistMovedEvent(sp_playlist* playlist, qint32 position, qint32 newPosition) :
        QEvent(QEvent::Type(QEvent::User + 2)),
        m_playlist(playlist),
        m_position(position),
        m_newPosition(newPosition)
    { }

    sp_playlist* playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }
    qint32 newPosition() const { return m_newPosition; }

private:

    sp_playlist* m_playlist;
    qint32 m_position;
    qint32 m_newPosition;

};

static void SP_CALLCONV playlistAddedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(pc), new PlaylistAddedEvent(playlist, position));
}

static void SP_CALLCONV playlistRemovedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(pc), new PlaylistRemovedEvent(playlist, position));
}

static void SP_CALLCONV playlistMovedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, int newPosition, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(pc), new PlaylistMovedEvent(playlist, position, newPosition));
}

static void SP_CALLCONV containerLoadedCallback(sp_playlistcontainer* pc, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(pc), new QEvent(QEvent::Type(QEvent::User + 3)));
}

#endif
