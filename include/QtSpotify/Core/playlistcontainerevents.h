#ifndef QTSPOTIFY_PLAYLISTCONTAINEREVENTS_H_
#define QTSPOTIFY_PLAYLISTCONTAINEREVENTS_H_

#include <QtSpotify/Core/playlist.h>

#include <QtCore/QEvent>

#include <memory>
#include <libspotify/api.h>

class PlaylistAddedEvent : public QEvent
{

public:

    PlaylistAddedEvent(sp_playlist* playlist, int position) :
        QEvent(QEvent::Type(QEvent::User)),
        m_position(position)
    {
        m_playlist = std::make_shared<Playlist>(playlist);
    }

    std::shared_ptr<Playlist> playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }

private:

    std::shared_ptr<Playlist> m_playlist;
    qint32 m_position;

};

class PlaylistRemovedEvent : public QEvent
{

public:

    PlaylistRemovedEvent(sp_playlist* playlist, int position) :
        QEvent(QEvent::Type(QEvent::User + 1)),
        m_position(position)
    {
        m_playlist = std::make_shared<Playlist>(playlist);
    }

    std::shared_ptr<Playlist> playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }

private:

    std::shared_ptr<Playlist> m_playlist;
    qint32 m_position;

};

class PlaylistMovedEvent : public QEvent
{

public:

    PlaylistMovedEvent(sp_playlist* playlist, int position, int newPosition) :
        QEvent(QEvent::Type(QEvent::User + 2)),
        m_position(position),
        m_newPosition(newPosition)
    {
        m_playlist = std::make_shared<Playlist>(playlist);
    }

    std::shared_ptr<Playlist> playlist() const { return m_playlist; }
    qint32 position() const { return m_position; }
    qint32 newPosition() const { return m_newPosition; }

private:

    std::shared_ptr<Playlist> m_playlist;
    qint32 m_position;
    qint32 m_newPosition;

};



#endif
