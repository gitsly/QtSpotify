#include "audioplayer.h"

SpotifyAudioThreadWorker::SpotifyAudioThreadWorker() :
    QObject(nullptr),
    m_audioOutput(nullptr),
    m_ioDevice(nullptr),
    m_audioTimerId(0),
    m_timeCounter(0),
    m_endOfTrack(false),
    m_previousElapsedTime(0)
{
}

bool SpotifyAudioThreadWorker::event(QEvent* e)
{
    if(e->type() == QEvent::User + EventOffset::StreamingStarted) {
        SpotifyStreamingStartedEvent* ev = static_cast<SpotifyStreamingStartedEvent*>(e);
        startStreaming(ev->channels(), ev->samepleRate(), ev->volume());
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + EventOffset::EndOfTrack) {
        m_endOfTrack = true;
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + EventOffset::ResumePlayback) {
        if(m_audioOutput != nullptr) {
            killTimer(m_audioTimerId);
            m_audioOutput->suspend();
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + EventOffset::SuspendAudio) {
        if(m_audioOutput != nullptr) {
            killTimer(m_audioTimerId);
            m_audioOutput->suspend();
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + EventOffset::StopAudio) {
        QMutexLocker lock(&g_audioMutex);
        killTimer(m_audioTimerId);
        g_audioBuffer.close();
        g_audioBuffer.setData(QByteArray());
        g_readPos = 0;
        g_writePos = 0;
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
    else if(e->type() == QEvent::User + EventOffset::ResetBuffers) {
        if(m_audioOutput != nullptr) {
            QMutexLocker lock(&g_audioMutex);
            killTimer(m_audioTimerId);
            m_audioOutput->suspend();
            m_audioOutput->stop();
            g_audioBuffer.close();
            g_audioBuffer.setData(QByteArray());
            g_audioBuffer.open(QIODevice::ReadWrite);
            g_readPos = 0;
            g_writePos = 0;
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
    else if(e->type() == QEvent::User + EventOffset::Volume) {
        if(m_audioOutput != nullptr) {
            SpotifyVolumeEvent* ev = static_cast<SpotifyVolumeEvent*>(e);
            m_audioOutput->setVolume(ev->volume() / 100.f);
        }
    }
    else if(e->type() == QEvent::Timer) {
        QTimerEvent* te = static_cast<QTimerEvent*>(e);
        if(te->timerId() == m_audioTimerId) {
            updateAudioBuffer();
            e->accept();
            return true;
        }
    }

    return QObject::event(e);
}

void SpotifyAudioThreadWorker::startStreaming(qint32 channels, qint32 sampleRate, qint32 volume)
{
    if(m_audioOutput != nullptr) {
        QAudioFormat format;
        format.setChannelCount(channels);
        format.setCodec("audio/pcm");
        format.setSampleRate(sampleRate);
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);

        QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
        if(!info.isFormatSupported(format)) {
            //TODO: Log error
            QCoreApplication::postEvent(SpotifySession::instance(), new QEvent(QEvent::Type(QEvent::User + EventOffset::StopPlayback)));
            return;
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

void SpotifyAudioThreadWorker::updateAudioBuffer()
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
        QCoreApplication::postEvent(SpotifySession::instance(), new SpotifyTrackProgressEvent(elapsedTime - m_previousElapsedTime));
        QCoreApplication::postEvent(SpotifySession::instance(), new QEvent(QEvent::Type(QEvent::User + EventOffset::EndOfTrack)));
        m_previousElapsedTime = elapsedTime;
        return;
    }
    else {
        g_audioMutex.lock();
        qint32 toRead = qMin(g_writePos - g_readPos, m_audioOutput->bytesFree());
        g_audioBuffer.seek(g_readPos);
        char* data = new vhar[toRead];
        qint32 read = g_audioBuffer.read(data, toRead);
        g_readPos += read;
        g_audioMutex.unlock();

        m_ioDevice->write(data, read);
        delete[] data;
    }

    m_timeCounter += AUDIOSTREAM_UPDATE_INTERVAL;

    if(m_timeCounter >= 1000) {
        m_timeCounter = 0;
        qint32 elapsedTime = qint32(m_audioOutput->processedUSecs() / 1000);
        QCoreApplication::postEvent(SpotifySession::instance(), new SpotifyTrackProgressEvent(elapsedTime - m_previousElapsedTime));
        m_previousElapsedTime = elapsedTime;
    }
}

void SpotifyAudioThread::run()
{
    g_audioWorker = new SpotifyAudioThreadWorker();
    exec();
    delete g_audioWorker;
}
