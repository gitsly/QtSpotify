#ifndef QTSPOTIFY_AUDIOPLAYER_H_
#define QTSPOTIFY_AUDIOPLAYER_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QtCore/QBuffer>
#include <QtCore/QThread>
#include <QtCore/QEvent>

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioFormat>

#define AUDIOSTREAM_UPDATE_INTERVAL 20
#define BUFFER_SIZE 51200

class AudioThread : public QThread
{

public:

    void run();

};

class QTS_EXPORT AudioPlayer : public QObject
{

    Q_OBJECT

public:

    AudioPlayer();
    virtual ~AudioPlayer();

    static QMutex audioMutex;
    static QBuffer audioBuffer;

    static qint32 readPos;
    static qint32 writePos;

protected:

    bool event(QEvent*);

private:

    void startStreaming(qint32 channels, qint32 sampleRate, qint32 volume);
    void updateAudioBuffers();

    QAudioOutput* m_audioOutput;
    QIODevice* m_ioDevice;

    qint32 m_audioTimerId;
    qint32 m_timeCounter;
    qint32 m_previousElapsedTime;

    bool m_endOfTrack;

};

#endif
