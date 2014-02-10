#ifndef QTSPOTIFY_SESSION_H_
#define QTSPOTIFY_SESSION_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QHash>
#include <QMutex>
#include <libspotify/api.h>

class QTSPOTIFYSHARED_EXPORT SpotifySession : public QObject
{

    Q_OBJECT


    Q_ENUMS(ConnectionStatus ConnectionError OfflineError StreamingQuality)

public:

    enum ConnectionStatus {
        LoggedOut = SP_CONNECTION_STATE_LOGGED_OUT,
        LoggedIn = SP_CONNECTION_STATE_LOGGED_IN,
        Disconnected = SP_CONNECTION_STATE_DISCONNECTED,
        Undefined = SP_CONNECTION_STATE_UNDEFINED,
        Offline = SP_CONNECTION_STATE_OFFLINE
    };

    enum ConnectionError {
        Ok = SP_ERROR_OK,
        ClientTooOld = SP_ERROR_CLIENT_TOO_OLD,
        UnableToContactServer = SP_ERROR_UNABLE_TO_CONTACT_SERVER,
        BadUserNameOrPassword = SP_ERROR_BAD_USERNAME_OR_PASSWORD,
        UserBanned = SP_ERROR_USER_BANNED,
        UserNeedsPremium = SP_ERROR_USER_NEEDS_PREMIUM,
        OtherTransient = SP_ERROR_OTHER_TRANSIENT,
        OtherPermanent = SP_ERROR_OTHER_PERMANENT
    };

    enum OfflineError {
        TooManyTracks = SP_ERROR_OFFLINE_TOO_MANY_TRACKS,
        DiskCache = SP_ERROR_OFFLINE_DISK_CACHE,
        Expired = SP_ERROR_OFFLINE_EXPIRED,
        NotAllowed = SP_ERROR_OFFLINE_NOT_ALLOWED,
        LicenseLost = SP_ERROR_OFFLINE_LICENSE_LOST,
        LicenseError = SP_ERROR_OFFLINE_LICENSE_ERROR
    };

    enum StreamingQuality {
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

    Q_DECLARE_FLAGS(ConnectionRules, ConnectionRule)

    static SpotifySession* instance();

private:

    SpotifySession();
    Q_DISABLE_COPY(SpotifySession)
    static SpotifySession* m_instance;

    void init();

    sp_session* m_spSession;
    sp_session_callbacks m_spCallbacks;


};

#endif
