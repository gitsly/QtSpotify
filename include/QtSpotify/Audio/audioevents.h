#ifndef AUDIOEVENTS_H
#define AUDIOEVENTS_H

#include <QtSpotify/Core/global.h>
#include <QtCore/QEvent>

class QTS_EXPORT StreamingStartedEvent : public QEvent
{

public:

    StreamingStartedEvent(qint32 channels, qint32 sampleRate, qint32 volume) :
        QEvent(QEvent::Type(QEvent::User + 3)),
        m_channels(channels),
        m_sampleRate(sampleRate),
        m_volume(volume)
    { }

    qint32 channels() const { return m_channels; }
    qint32 sampleRate() const { return m_sampleRate; }
    qint32 volume() const { return m_volume; }

private:

    qint32 m_channels;
    qint32 m_sampleRate;
    qint32 m_volume;

};

class QTS_EXPORT VolumeEvent : public QEvent
{

public:

    VolumeEvent(qint32 volume) :
        QEvent(QEvent::Type(QEvent::User + 20)),
        m_volume(volume)
    { }

    qint32 volume() const { return m_volume; }

private:

    qint32 m_volume;

};

class QTS_EXPORT TrackProgressedEvent : public QEvent
{

public:

    TrackProgressedEvent(qint32 delta) :
        QEvent(QEvent::Type(QEvent::User + 10)),
        m_delta(delta)
    { }

    qint32 delta() const { return m_delta; }

private:

    qint32 m_delta;
};

#endif // AUDIOEVENTS_H
