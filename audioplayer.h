#ifndef QTSPOTIFY_AUDIOPLAYER_H
#define QTSPOTIFY_AUDIOPLAYER_H

#include <QtCore/QObject>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QIODevice>
#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QBuffer>
#include <QtCore/QThread>

#include "sessionevents.h"

#define BUFFER_SIZE 51200
#define AUDIOSTREAM_UPDATE_INTERVAL 20

class SpotifyAudioThreadWorker;

static QMutex g_audioMutex;
static QBuffer g_audioBuffer;

static qint32 g_readPos = 0;
static qint32 g_writePos = 0;

static SpotifyAudioThreadWorker* g_audioWorker;

class SpotifyAudioThreadWorker : public QObject
{

public:

    SpotifyAudioThreadWorker();
    bool event(QEvent*);

private:

    void startStreaming(qint32 channels, qint32 sampleRate, qint32 volume);
    void updateAudioBuffer();

    QAudioOutput* m_audioOutput;
    QIODevice* m_ioDevice;
    qint32 m_audioTimerId;
    qint32 m_timeCounter;
    bool m_endOfTrack;
    qint32 m_previousElapsedTime;

};

class SpotifyAudioThread : public QThread
{
public:
    void run();
};

#endif // AUDIOPLAYER_H
