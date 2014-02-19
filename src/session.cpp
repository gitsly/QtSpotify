#include <QtSpotify/Session>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtSpotify/Core/sessioncallbacks.h>
#include "../apikey.h"

AudioPlayer* Session::audioPlayer = nullptr;

Session::Session()
{
    connect(qApp, &QCoreApplication::aboutToQuit, this, &Session::cleanUp);

    m_audioThread = new AudioThread();
    m_audioThread->start(QThread::HighestPriority);

    init();
}

Session* Session::instance()
{
    static Session instance;
    return &instance;
}

void Session::init()
{
    m_callbacks.connectionstate_updated = connectionStateUpdatedCallback;
    m_callbacks.connection_error = connectionErrorCallback;
    m_callbacks.credentials_blob_updated = credentialsBlogUpdatedCallback;
    m_callbacks.end_of_track = endOfTrackCallback;
    m_callbacks.get_audio_buffer_stats = getAudioBufferStatsCallback;
    m_callbacks.logged_in = loggedInCallback;
    m_callbacks.logged_out = loggedOutCallback;
    m_callbacks.log_message = logMessageCallback;
    m_callbacks.message_to_user = messageToUserCallback;
    m_callbacks.metadata_updated = metadataUpdatedCallback;
    m_callbacks.music_delivery = musicDeliveryCallback;
    m_callbacks.notify_main_thread = notifyMainThreadCallback;
    m_callbacks.offline_error = offlineErrorCallback;
    m_callbacks.offline_status_updated = offlineStatusUpdatedCallback;
    m_callbacks.play_token_lost = playTokenLostCallback;
    m_callbacks.private_session_mode_changed = privateSessionModeChangedCallback;
    m_callbacks.scrobble_error = scrobbleErrorCallback;
    m_callbacks.start_playback = startPlaybackCallback;
    m_callbacks.stop_playback = stopPlaybackCallback;
    m_callbacks.streaming_error = streamingErrorCallback;
    m_callbacks.userinfo_updated = userInfoUpdatedCallback;

    QString cacheLocation = QCoreApplication::applicationDirPath() + QString("/cache");

    m_spConfig.api_version = SPOTIFY_API_VERSION;
    m_spConfig.cache_location = cacheLocation.toLatin1().constData();
    m_spConfig.settings_location = cacheLocation.toLatin1().constData();
    m_spConfig.application_key = g_appkey;
    m_spConfig.application_key_size = g_appkey_size;
    m_spConfig.callbacks = &m_callbacks;
    m_spConfig.user_agent = "QtSpotify/0.1";
    m_spConfig.tracefile = nullptr;

    sp_error error = sp_session_create(&m_spConfig, &m_spSession);

    if(error != SP_ERROR_OK) {
        //Error creating session
    }
    else {
        QSettings settings;

        if(settings.contains("username")) {
            settings.remove("username");
            settings.remove("password");
        }

        m_offlineMode = settings.value("offlineMode", false).toBool();

        //TODO
        //checkNetworkAccess();

        setStreamingQuality(StreamingQuality::UltraQuality);

        StreamingQuality syncQuality = StreamingQuality(settings.value("syncQuality", int(StreamingQuality::UltraQuality)).toInt());
        setSyncQuality(syncQuality);

        bool syncMobile = settings.value("syncOverMobile", false).toBool();
        setSyncOverMobileAllowed(syncMobile);

        QString storedLogin = getStoredLoginInformation();
        if(!storedLogin.isEmpty()) {
            login(storedLogin);
        }

        bool shuffle = settings.value("shuffle", false).toBool();
        bool repeat = settings.value("repeat", false).toBool();
        bool repeatOne = settings.value("repeatOne", false).toBool();
        qint32 volume = settings.value("volume", 100).toInt();

        setShuffle(shuffle);
        setRepeat(repeat);
        setRepeatOne(repeatOne);
        setVolume(volume);

    }
}

/********************** INVOKABLE FUNCTIONS **********************/
/*****************************************************************/

bool Session::isValid() const
{
    return m_spSession != nullptr;
}

QString Session::formatDuration(qint64 duration)
{
    //Convert to seconds
    duration /= 1000;
    qint32 seconds = duration % 60;

    duration /= 60;
    qint32 minutes = duration % 60;

    qint32 hours = duration / 60;

    QString formatted;
    if(hours > 0) {
       formatted += QString::number(hours) + QLatin1String(":");
    }

    formatted += QLatin1String(minutes > 9 || hours == 0 ? "" : "0") + QString::number(minutes) + QLatin1String(":");
    formatted += QLatin1String(seconds > 9 ? "" : "0") + QString::number(seconds);

    return formatted;
}

void Session::setOfflineMode(bool on, bool forced)
{
    if(exchange(m_offlineMode, on)) {

    }
}

void Session::setSyncOverMobileAllowed(bool syncAllowed)
{
    if(exchange(m_syncOverMobileAllowed, syncAllowed)) {
        QSettings settings;
        settings.setValue("syncOverMobileAllowed", m_syncOverMobileAllowed);

        setConnectionRule(ConnectionRule::AllowSyncOverMobile, m_syncOverMobileAllowed);
        emit syncOverMobileAllowedChanged(m_syncOverMobileAllowed);
    }
}

/********************** PUBLIC FUNCTIONS **********************/
/**************************************************************/

ConnectionStatus Session::connectionStatus() const
{
    return m_connectionStatus;
}

bool Session::loggedIn() const
{
    return m_loggedIn;
}

bool Session::offlineMode() const
{
    return m_offlineMode;
}

ConnectionError Session::connectionError() const
{
    return m_connectionError;
}

User* Session::user() const
{
    return m_user;
}

StreamingQuality Session::streamingQuality() const
{
    return m_streamingQuality;
}

void Session::setStreamingQuality(StreamingQuality quality)
{
    if(exchange(m_streamingQuality, quality)) {
        QSettings settings;
        settings.setValue("streamingQuality", qint32(quality));

        emit streamingQualityChanged(m_streamingQuality);
    }
}

StreamingQuality Session::syncQuality() const
{
    return m_syncQuality;
}

void Session::setSyncQuality(StreamingQuality quality)
{
    if(exchange(m_syncQuality, quality)) {
        QSettings settings;
        settings.setValue("syncQuality", qint32(quality));

        emit syncQualityChanged(m_syncQuality);
    }
}

bool Session::syncOverMobileAllowed() const
{
    return m_syncOverMobileAllowed;
}

bool Session::shuffle() const
{
    return m_shuffle;
}

void Session::setShuffle(bool shuffle)
{

}

bool Session::repeat() const
{
    return m_repeat;
}

void Session::setRepeat(bool repeat)
{

}

bool Session::repeatOne() const
{
    return m_repeatOne;
}

void Session::setRepeatOne(bool repeatOne)
{

}

qint32 Session::volume() const
{
    return m_volume;
}

sp_session* Session::native() const
{
    return m_spSession;
}

/********************** PUBLIC SLOTS **********************/
/**********************************************************/

void Session::login(const QString& username, const QString& password, bool rememberMe)
{

}

void Session::logout(bool keepLoginInfo)
{

}

void Session::pause()
{

}

void Session::resume()
{

}

void Session::stop()
{

}

void Session::seek(qint32 offset)
{

}

void Session::setVolume(qint32 volume)
{

}


/********************** PRIVATE SLOTS **********************/
/***********************************************************/

void Session::cleanUp()
{

}

/********************** PRIVATE FUNCTIONS **********************/
/***************************************************************/

QString Session::getStoredLoginInformation() const
{
    QString username;

    char buffer[255];
    qint32 size = sp_session_remembered_user(m_spSession, &buffer[0], 255);
    if(size > 0) {
        username = QString::fromUtf8(&buffer[0], size);
    }

    return username;
}

void Session::setConnectionRules(ConnectionRules rules)
{
    if(m_connectionRules == rules) {
        return;
    }

    m_connectionRules = rules;
    sp_session_set_connection_rules(m_spSession, sp_connection_rules(int(m_connectionRules)));
    emit connectionRulesChanged(m_connectionRules);
}

void Session::setConnectionRule(ConnectionRule rule, bool on)
{
    ConnectionRules oldRules = m_connectionRules;

    if(on) {
        m_connectionRules |= rule;
    }
    else {
        m_connectionRules &= ~rule;
    }

    if(m_connectionRules != oldRules) {
        sp_session_set_connection_rules(m_spSession, sp_connection_rules(int(m_connectionRules)));
        emit connectionRulesChanged(m_connectionRules);
    }
}
