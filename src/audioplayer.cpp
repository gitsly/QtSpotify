#include <QtSpotify/Audio/audioplayer.h>
#include <QtSpotify/Audio/audioevents.h>

#include <QtSpotify/Session>
#include <QtCore/QCoreApplication>

QMutex AudioPlayer::audioMutex;
QBuffer AudioPlayer::audioBuffer;

qint32 AudioPlayer::readPos = 0;
qint32 AudioPlayer::writePos = 0;


void AudioThread::run()
{
    Session::audioPlayer = new AudioPlayer();
    exec();
    delete Session::audioPlayer;
}

AudioPlayer::AudioPlayer() :
    QObject(nullptr),
    m_audioOutput(nullptr),
    m_ioDevice(nullptr),
    m_audioTimerId(0),
    m_timeCounter(0),
    m_endOfTrack(false),
    m_previousElapsedTime(0)
{
}

AudioPlayer::~AudioPlayer()
{

}

bool AudioPlayer::event(QEvent* e)
{
    if(e->type() == QEvent::User + 3) {
        //Streaming started event
        StreamingStartedEvent* ev = static_cast<StreamingStartedEvent*>(e);
        startStreaming(ev->channels(), ev->sampleRate(), ev->volume());
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 4) {
        //yada
        m_endOfTrack = true;
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 6) {
        //Resume event
        if(m_audioOutput != nullptr) {
            m_audioTimerId = startTimer(AUDIOSTREAM_UPDATE_INTERVAL);
            m_audioOutput->resume();
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 7) {
        //Suspend event
        if(m_audioOutput != nullptr) {
            killTimer(m_audioTimerId);
            m_audioOutput->suspend();
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 8) {
        //Stop event
        QMutexLocker lock(&audioMutex);
        killTimer(m_audioTimerId);
        audioBuffer.close();
        audioBuffer.setData(QByteArray());
        readPos = 0;
        writePos = 0;
        if(m_audioOutput != nullptr) {
            m_audioOutput->suspend();
            m_audioOutput->stop();
            delete m_audioOutput;
            m_audioOutput = nullptr;
            m_ioDevice = nullptr;
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 9) {
        //Reset buffers event
        if(m_audioOutput != nullptr) {
            QMutexLocker lock(&audioMutex);
            killTimer(m_audioTimerId);
            m_audioOutput->suspend();
            m_audioOutput->stop();
            audioBuffer.close();
            audioBuffer.setData(QByteArray());
            audioBuffer.open(QIODevice::ReadWrite);
            readPos = 0;
            writePos = 0;
            m_audioOutput->reset();
            m_ioDevice = m_audioOutput->start();
            m_audioOutput->suspend();
            m_audioTimerId = startTimer(AUDIOSTREAM_UPDATE_INTERVAL);
            m_timeCounter = 0;
            m_previousElapsedTime = 0;
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 20) {
        //Volume event
        if(m_audioOutput != nullptr) {
            VolumeEvent* ev = static_cast<VolumeEvent*>(e);
            m_audioOutput->setVolume(ev->volume() / 100.f);
        }
    }
    else if(e->type() == QEvent::Timer) {
        QTimerEvent* ev = static_cast<QTimerEvent*>(e);
        if(ev->timerId() == m_audioTimerId) {
            updateAudioBuffers();
            e->accept();
            return true;
        }
    }

    return QObject::event(e);
}

void AudioPlayer::startStreaming(qint32 channels, qint32 sampleRate, qint32 volume)
{
    if(m_audioOutput == nullptr) {
        QAudioFormat format;
        format.setChannelCount(channels);
        format.setSampleRate(sampleRate);
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        format.setCodec("audio/pcm");

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
        if(!info.isFormatSupported(format)) {
            //Error
        }

        m_audioOutput = new QAudioOutput(format);
        m_audioOutput->setVolume(volume / 100.f);
        m_audioOutput->setBufferSize(BUFFER_SIZE);
        m_ioDevice = m_audioOutput->start();
        m_audioOutput->suspend();
        m_audioTimerId = startTimer(AUDIOSTREAM_UPDATE_INTERVAL);
        m_endOfTrack = false;
        m_timeCounter = 0;
        m_previousElapsedTime = 0;
    }
}

void AudioPlayer::updateAudioBuffers()
{
    if(m_audioOutput == nullptr) {
        return;
    }

    if(m_audioOutput->state() == QAudio::SuspendedState) {
        m_audioOutput->resume();
    }

    if(m_endOfTrack && m_audioOutput->state() == QAudio::IdleState) {
        killTimer(m_audioTimerId);
        qint32 elapsedTime = qint32(m_audioOutput->processedUSecs() / 1000);
        QCoreApplication::postEvent(Session::instance(), new TrackProgressedEvent(elapsedTime - m_previousElapsedTime));
        QCoreApplication::postEvent(Session::instance(), new QEvent(QEvent::Type(QEvent::User + 4)));
        m_previousElapsedTime = elapsedTime;
        return;
    }
    else {
        audioMutex.lock();
        qint32 toRead = qMin(writePos - readPos, m_audioOutput->bytesFree());
        audioBuffer.seek(readPos);
        char* data = new char[toRead];
        qint32 read = audioBuffer.read(data, toRead);
        readPos += read;
        audioMutex.unlock();

        m_ioDevice->write(data, read);

        delete[] data;
    }

    m_timeCounter += AUDIOSTREAM_UPDATE_INTERVAL;
    if(m_timeCounter >= 1000) {
        m_timeCounter = 0;
        qint32 elapsedTime = qint32(m_audioOutput->processedUSecs() / 10000);
        QCoreApplication::postEvent(Session::instance(), new TrackProgressedEvent(elapsedTime - m_previousElapsedTime));
        m_previousElapsedTime = elapsedTime;
    }
}
