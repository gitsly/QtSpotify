#ifndef QTSPOTIFY_SESSION_H_
#define QTSPOTIFY_SESSION_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtSpotify/Audio/audioplayer.h>

#include <libspotify/api.h>

class User;

enum class ConnectionStatus {
    LoggedOut = SP_CONNECTION_STATE_LOGGED_OUT,
    LoggedIn = SP_CONNECTION_STATE_LOGGED_IN,
    Disconnected = SP_CONNECTION_STATE_DISCONNECTED,
    Undefined = SP_CONNECTION_STATE_UNDEFINED,
    Offline = SP_CONNECTION_STATE_OFFLINE
};

enum class ConnectionError {
    Ok = SP_ERROR_OK,
    ClientTooOld = SP_ERROR_CLIENT_TOO_OLD,
    UnableToContactServer = SP_ERROR_UNABLE_TO_CONTACT_SERVER,
    BadUsernameOrPassword = SP_ERROR_BAD_USERNAME_OR_PASSWORD,
    UserBanned = SP_ERROR_USER_BANNED,
    UserNeedsPremium = SP_ERROR_USER_NEEDS_PREMIUM,
    OtherTransient = SP_ERROR_OTHER_TRANSIENT,
    OtherPermanent = SP_ERROR_OTHER_PERMANENT
};

enum class OfflineError {
    TooManyTracks = SP_ERROR_OFFLINE_TOO_MANY_TRACKS,
    DiskCache = SP_ERROR_OFFLINE_DISK_CACHE,
    Expired = SP_ERROR_OFFLINE_EXPIRED,
    NotAllowed = SP_ERROR_OFFLINE_NOT_ALLOWED,
    LicenseLost = SP_ERROR_OFFLINE_LICENSE_LOST,
    LicenseError = SP_ERROR_OFFLINE_LICENSE_ERROR
};

enum class StreamingQuality {
    Unknown = -1,
    LowQuality = SP_BITRATE_96k,
    HighQuality = SP_BITRATE_160k,
    UltraQuality = SP_BITRATE_320k
};

enum ConnectionRule {
    AllowNetwork = SP_CONNECTION_RULE_NETWORK,
    AllowNetworkIfRoaming = SP_CONNECTION_RULE_NETWORK_IF_ROAMING,
    AllowSyncOverMobile = SP_CONNECTION_RULE_ALLOW_SYNC_OVER_MOBILE,
    AllowSyncOverWifi = SP_CONNECTION_RULE_ALLOW_SYNC_OVER_WIFI
};

class QTS_EXPORT Session : public QObject
{

    Q_OBJECT

    Q_PROPERTY(ConnectionStatus connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(ConnectionError connectionError READ connectionError NOTIFY connectionErrorChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(bool repeatOne READ repeatOne WRITE setRepeatOne NOTIFY repeatOneChanged)
    Q_PROPERTY(qint32 volume READ volume WRITE volumeChanged)

    Q_ENUMS(ConnectionError OfflineError StreamingQuality ConnectionRule)

public:

    Q_DECLARE_FLAGS(ConnectionRules, ConnectionRule)

    static AudioPlayer* audioPlayer;
    static Session* instance();

    Q_INVOKABLE bool isValid() const;
    Q_INVOKABLE QString formatDuration(qint64 duration);
    Q_INVOKABLE void setOfflineMode(bool on, bool forced = false);
    Q_INVOKABLE void setSyncOverMobileAllowed(bool syncAllowed);

    ConnectionStatus connectionStatus() const;

    bool loggedIn() const;
    bool offlineMode() const;

    ConnectionError connectionError() const;

    User* user() const;

    StreamingQuality streamingQuality() const;
    void setStreamingQuality(StreamingQuality quality);

    StreamingQuality syncQuality() const;
    void setSyncQuality(StreamingQuality quality);

    bool syncOverMobileAllowed() const;

    bool shuffle() const;
    void setShuffle(bool shuffle);

    bool repeat() const;
    void setRepeat(bool repeat);

    bool repeatOne() const;
    void setRepeatOne(bool repeatOne);

    qint32 volume() const;

    sp_session* native() const;

public slots:

    void login(const QString& username, const QString& password = QString(), bool rememberMe = false);
    void logout(bool keepLoginInfo);

    void pause();
    void resume();
    void stop();
    void seek(qint32 offset);
    void setVolume(qint32 volume);

private slots:

    void cleanUp();

signals:

    void metadataUpdated();

    void loggingIn(const QString& username);
    void loggingOut();

    void playTokenLost();

    void shuffleChanged(bool shuffle);
    void repeatChanged(bool repeat);
    void repeatOneChanged(bool repeatOne);
    void volumeChanged(qreal volume);

    void connectionStatusChanged(ConnectionStatus status);
    void connectionErrorChanged(ConnectionError error);
    void connectionRulesChanged(ConnectionRules rules);

    void streamingQualityChanged(StreamingQuality quality);
    void syncQualityChanged(StreamingQuality quality);

    void syncOverMobileAllowedChanged(bool allowed);

private:

    Session();
    Q_DISABLE_COPY(Session)

    void init();

    QString getStoredLoginInformation() const;

    void setConnectionRules(ConnectionRules rules);
    void setConnectionRule(ConnectionRule rule, bool on = true);

    sp_session* m_spSession;
    sp_session_callbacks m_callbacks;
    sp_session_config m_spConfig;

    ConnectionStatus m_connectionStatus;
    ConnectionError m_connectionError;
    ConnectionRules m_connectionRules;

    StreamingQuality m_streamingQuality;
    StreamingQuality m_syncQuality;
    bool m_syncOverMobileAllowed;

    bool m_loggedIn;
    bool m_offlineMode;

    bool m_shuffle;
    bool m_repeat;
    bool m_repeatOne;

    qint32 m_volume;

    AudioThread* m_audioThread;

    User* m_user;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Session::ConnectionRules)

#endif
