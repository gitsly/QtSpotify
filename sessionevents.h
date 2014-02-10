#ifndef QTSPOTIFY_SESSIONEVENTS_H_
#define QTSPOTIFY_SESSIONEVENTS_H_

#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <libspotify/api.h>

enum EventOffset {
    ConnectionError = 1,
    MetadataUpdated = 2,
    StreamingStarted = 3,
    EndOfTrack = 4,
    StopPlayback = 5,
    ResumePlayback = 6,
    SuspendAudio = 7,
    StopAudio = 8,
    ResetBuffers = 9,
    TrackProgressed = 10,
    RequestImage = 11,
    ReceiveImage = 12,
    PlayTokenLost = 13,
    LoggedIn = 14,
    LoggedOut = 15,
    OfflineError = 16,
    Volume = 20,
};

class SpotifyConnectionErrorEvent : public QEvent
{

public:

    SpotifyConnectionErrorEvent(sp_error error, const QString& message) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::ConnectionError)),
        m_error(error),
        m_message(message)
    { }

    sp_error error() const { return m_error; }
    QString message() const { return m_message; }

private:

    sp_error m_error;
    QString m_message;

};

class SpotifyStreamingStartedEvent : public QEvent
{

public:

    SpotifyStreamingStartedEvent(qint32 channels, qint32 sampleRate, qint32 volume) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::StreamingStarted)),
        m_channels(channels),
        m_sampleRate(sampleRate),
        m_volume(volume)
    { }

    qint32 channels() const { return m_channels; }
    qint32 samepleRate() const { return m_sampleRate; }
    qint32 volume() const { return m_volume; }

private:

    qint32 m_channels;
    qint32 m_sampleRate;
    qint32 m_volume;

};

class SpotifyVolumeEvent : public QEvent
{

public:

    SpotifyVolumeEvent(qint32 volume) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::Volume)),
        m_volume(volume)
    { }

    qint32 volume() const { return m_volume; }

private:

    qint32 m_volume;

};

class SpotifyTrackProgressEvent : public QEvent
{

public:

    SpotifyTrackProgressEvent(qint32 delta) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::TrackProgressed)),
        m_delta(delta)
    { }

    qint32 delta() const { return m_delta; }

private:

    qint32 m_delta;

};

class SpotifyRequestImageEvent : public QEvent
{

public:

    SpotifyRequestImageEvent(const QString& id) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::RequestImage)),
        m_id(id)
    { }

    const QString& imageId() const { return m_id; }

private:

    QString m_id;

};

class SpotifyReceiveImageEvent : public QEvent
{

public:

    SpotifyReceiveImageEvent(sp_image* image) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::ReceiveImage)),
        m_image(image)
    { }

    sp_image* image() const { return m_image; }

private:

    sp_image* m_image;

};

class SpotifyOfflineErrorEvent : public QEvent
{

public:

    SpotifyOfflineErrorEvent(sp_error error, const QString& message) :
        QEvent(QEvent::Type(QEvent::User + EventOffset::OfflineError)),
        m_error(error),
        m_message(message)
    { }

    sp_error error() const { return m_error; }
    QString message() const { return m_message; }

private:

    sp_error m_error;
    QString m_message;

};

#endif
