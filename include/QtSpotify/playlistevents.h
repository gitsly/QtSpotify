#ifndef QTSPOTIFY_PLAYLISTEVENTS_H_
#define QTSPOTIFY_PLAYLISTEVENTS_H_

#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <libspotify/api.h>

#include <QtSpotify/spotifyuser.h>
#include <QtSpotify/spotifytrack.h>

class PlaylistTracksAddedEvent : public QEvent
{

public:

    PlaylistTracksAddedEvent(sp_track* const* tracks, int numTracks, int position) :
        QEvent(QEvent::Type(QEvent::User + 1)),
        m_position(position)
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_tracks.append(new SpotifyTrack(tracks[i]));
        }
    }

    QList<SpotifyTrack*> tracks() const { return m_tracks; }
    qint32 position() const { return m_position; }

private:

    QList<SpotifyTrack*> m_tracks;
    qint32 m_position;
};

class PlaylistTracksRemovedEvent : public QEvent
{

public:

    PlaylistTracksRemovedEvent(const int* tracks, int numTracks) :
        QEvent(QEvent::Type(QEvent::User + 2))
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_indices += tracks[i];
        }
    }

    QList<qint32> indices() const { return m_indices; }

private:

    QList<qint32> m_indices;
};

class PlaylistTracksMovedEvent : public QEvent
{

public:

    PlaylistTracksMovedEvent(const int* tracks, int numTracks, int newPosition) :
        QEvent(QEvent::Type(QEvent::User + 3)),
        m_position(newPosition)
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_indices.append(tracks[i]);
        }
    }

    QList<qint32> indices() const { return m_indices; }
    qint32 position() const { return m_position; }

private:

    QList<qint32> m_indices;
    qint32 m_position;

};

class PlaylistRenamedEvent : public QEvent
{

public:

    PlaylistRenamedEvent(sp_playlist* playlist) :
        QEvent(QEvent::Type(QEvent::User + 4)),
        m_name("")
    {
        m_name = QString::fromUtf8(sp_playlist_name(playlist));
    }

    QString name() const { return m_name; }

private:

    QString m_name;
};

class PlaylistTrackCreatedChangedEvent : public QEvent
{

public:

    PlaylistTrackCreatedChangedEvent(int position, sp_user* user, int when) :
        QEvent(QEvent::Type(QEvent::User + 8)),
        m_user(new SpotifyUser(user)),
        m_position(position)
    {
        m_creationTime = QDateTime::fromTime_t(when);
    }

    QDateTime creationTime() const { return m_creationTime; }
    SpotifyUser* user() const { return m_user; }
    qint32 position() const { return m_position; }

private:

    QDateTime m_creationTime;
    SpotifyUser* m_user;
    qint32 m_position;

};

class PlaylistTrackSeenChangedEvent : public QEvent
{

public:

    PlaylistTrackSeenChangedEvent(int position, bool seen) :
        QEvent(QEvent::Type(QEvent::User + 9)),
        m_position(position),
        m_seen(seen)
    { }

    qint32 position() const { return m_position; }
    bool seen() const { return m_seen; }

private:

    qint32 m_position;
    bool m_seen;

};

class PlaylistDescriptionChangedEvent : public QEvent
{

public:

    PlaylistDescriptionChangedEvent(const char* description) :
        QEvent(QEvent::Type(QEvent::User + 10)),
        m_description(description)
    { }

    QString description() const { return m_description; }

private:

    QString m_description;
};

class PlaylistSubscribersChangedEvent : public QEvent
{

public:

    PlaylistSubscribersChangedEvent(sp_playlist* playlist) :
        QEvent(QEvent::Type(QEvent::User + 13))
    {
        sp_subscribers* sub = sp_playlist_subscribers(playlist);
        for(quint32 i=0 ; i<sub->count ; ++i) {
            m_subscribers.append(QString::fromUtf8(sub->subscribers[i]));
        }
    }

    QStringList subscribers() const { return m_subscribers; }

private:

    QStringList m_subscribers;
};

#endif
