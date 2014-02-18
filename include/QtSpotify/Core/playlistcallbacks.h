#ifndef QTSPOTIFY_PLAYLISTCALLBACKS_H_
#define QTSPOTIFY_PLAYLISTCALLBACKS_H_

#include <QtSpotify/Playlist>
#include <libspotify/api.h>

#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>

class TracksAddedEvent : public QEvent
{

public:

    TracksAddedEvent(sp_track* const* tracks, int numTracks, int position) :
        QEvent(QEvent::Type(QEvent::User)),
        m_position(position)
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_tracks.append(tracks[i]);
        }
    }

    QList<sp_track*> tracks() const { return m_tracks; }
    qint32 position() const { return m_position; }

private:

    QList<sp_track*> m_tracks;
    qint32 m_position;

};

class TracksRemovedEvent : public QEvent
{

public:

    TracksRemovedEvent(const int* tracks, int numTracks) :
        QEvent(QEvent::Type(QEvent::User + 1))
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_indices.append(tracks[i]);
        }
    }

    QList<qint32> indices() const { return m_indices; }

private:

    QList<qint32> m_indices;

};

class TracksMovedEvent : public QEvent
{

public:

    TracksMovedEvent(const int* tracks, int numTracks, int newPosition) :
        QEvent(QEvent::Type(QEvent::User + 2)),
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

class TrackCreatedChangedEvent : public QEvent
{

public:

    TrackCreatedChangedEvent(int position, sp_user* user, int when) :
        QEvent(QEvent::Type(QEvent::User + 7)),
        m_position(position),
        m_user(user)
    {
        m_when = QDateTime::fromTime_t(when);
    }

    qint32 position() const { return m_position; }
    QDateTime when() const { return m_when; }
    sp_user* user() const { return m_user; }

private:

    qint32 m_position;
    QDateTime m_when;
    sp_user* m_user;

};

class TrackSeenChangedEvent : public QEvent
{

public:

    TrackSeenChangedEvent(int position, bool seen) :
        QEvent(QEvent::Type(QEvent::User + 8)),
        m_position(position),
        m_seen(seen)
    { }

    qint32 position() const { return m_position; }
    bool seen() const { return m_seen; }

private:

    qint32 m_position;
    bool m_seen;

};

class DescriptionChangedEvent : public QEvent
{

public:

    DescriptionChangedEvent(const char* description) :
        QEvent(QEvent::Type(QEvent::User + 9)),
        m_description(description)
    { }

    QString description() const { return m_description; }

private:

    QString m_description;

};

static void SP_CALLCONV tracksAddedCallback(sp_playlist* playlist, sp_track* const* tracks, int numTracks, int position, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new TracksAddedEvent(tracks, numTracks, position));
}

static void SP_CALLCONV tracksRemovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new TracksRemovedEvent(tracks, numTracks));
}

static void SP_CALLCONV tracksMovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, int newPosition, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new TracksMovedEvent(tracks, numTracks, newPosition));
}

static void SP_CALLCONV playlistRenamedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new QEvent(QEvent::Type(QEvent::User + 3)));
}

static void SP_CALLCONV playlistStateChangedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new QEvent(QEvent::Type(QEvent::User + 4)));
}

static void SP_CALLCONV playlistUpdateInProgressCallback(sp_playlist* playlist, bool done, void* userData)
{
    Q_UNUSED(userData)
    Q_UNUSED(playlist)
    Q_UNUSED(done)
}

static void SP_CALLCONV playlistMetadataUpdatedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new QEvent(QEvent::Type(QEvent::User + 6)));
}

static void SP_CALLCONV trackCreatedChangedCallback(sp_playlist* playlist, int position, sp_user* user, int when, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new TrackCreatedChangedEvent(position, user, when));
}

static void SP_CALLCONV trackSeenChangedCallback(sp_playlist* playlist, int position, bool seen, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new TrackSeenChangedEvent(position, seen));
}

static void SP_CALLCONV descriptionChangedCallback(sp_playlist* playlist, const char* description, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new DescriptionChangedEvent(description));
}

static void SP_CALLCONV imageChangedCallback(sp_playlist* playlist, const byte* image, void* userData)
{
    Q_UNUSED(userData)
    Q_UNUSED(playlist)
    Q_UNUSED(image)
}

static void SP_CALLCONV trackMessageChangedCallback(sp_playlist* playlist, int position, const char* message, void* userData)
{
    Q_UNUSED(userData)
    Q_UNUSED(playlist)
    Q_UNUSED(position)
    Q_UNUSED(message)
}

static void SP_CALLCONV subscribersChangedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Playlist::playlistObjects.value(playlist), new QEvent(QEvent::Type(QEvent::User + 12)));
}

#endif
