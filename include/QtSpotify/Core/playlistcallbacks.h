#ifndef QTSPOTIFY_PLAYLISTCALLBACKS_H_
#define QTSPOTIFY_PLAYLISTCALLBACKS_H_

#include <libspotify/api.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QDateTime>

#include <memory>

#include <QtSpotify/Core/deleters.h>
#include <QtSpotify/Core/playlist.h>
#include <QtSpotify/Core/track.h>

using namespace QtSpotify;

class TracksAddedEvent : public QEvent
{

public:

    TracksAddedEvent(sp_track* const* tracks, int numTracks, int position) :
        QEvent(QEvent::Type(QEvent::User)),
        m_position(position)
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            //m_tracks.append(std::make_shared<Track>(tracks[i]));
            m_tracks.append(std::shared_ptr<Track>(new Track(tracks[i])));
        }
    }

    QList< std::shared_ptr<Track> > tracks() const { return m_tracks; }
    qint32 position() const { return m_position; }

private:

    QList<std::shared_ptr<Track> > m_tracks;
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
        m_newPosition(newPosition)
    {
        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_indices.append(tracks[i]);
        }
    }

    QList<qint32> indices() const { return m_indices; }
    qint32 newPosition() const { return m_newPosition; }

private:

    QList<qint32> m_indices;
    qint32 m_newPosition;

};

class TrackCreatedChangedEvent : public QEvent
{

public:

    TrackCreatedChangedEvent(int position, sp_user* user, int when) :
        QEvent(QEvent::Type(QEvent::User + 7)),
        m_position(position)
    {
        m_creationDate = QDateTime::fromTime_t(when);
        m_creator = std::make_shared<QtSpotify::User>(user);
    }

    qint32 position() const { return m_position; }
    std::shared_ptr<QtSpotify::User> creator() const { return m_creator; }
    QDateTime creationDate() const { return m_creationDate; }

private:

    qint32 m_position;
    std::shared_ptr<QtSpotify::User> m_creator;
    QDateTime m_creationDate;

};

class TrackSeenChangedEvent : public QEvent
{

public:

    TrackSeenChangedEvent(int position, bool seen) :
        QEvent(QEvent::Type(QEvent::User + 8)),
        m_position(position),
        m_seen(seen)
    { }

    bool seen() const { return m_seen; }
    qint32 position() const { return m_position; }

private:

    bool m_seen;
    qint32 m_position;

};

class DescriptionChangedEvent : public QEvent
{

public:

    DescriptionChangedEvent(const char* description) :
        QEvent(QEvent::Type(QEvent::User + 9))
    {
        m_description = QString::fromUtf8(description);
    }

    QString description() const { return m_description; }

private:

    QString m_description;

};

#endif
