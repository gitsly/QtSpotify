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

    /*!
     * \brief The ConnectionStatus enum represent different connection statuses
     */
    enum ConnectionStatus {
        LoggedOut = SP_CONNECTION_STATE_LOGGED_OUT,         /**< User not yet logged in */
        LoggedIn = SP_CONNECTION_STATE_LOGGED_IN,           /**< Logged in against a Spotify access point */
        Disconnected = SP_CONNECTION_STATE_DISCONNECTED,    /**< Was logged in, but has no been disconnected */
        Undefined = SP_CONNECTION_STATE_UNDEFINED,          /**< The connection state was undefined */
        Offline = SP_CONNECTION_STATE_OFFLINE               /**< Logged in in offline mode */
    };

    /*!
     * \brief The ConnectionError enum represents connection errors
     */
    enum ConnectionError {
        Ok = SP_ERROR_OK,                                           /**< No connection error */
        ClientTooOld = SP_ERROR_CLIENT_TOO_OLD,                     /**< Client is too old, library needs to be updated */
        UnableToContactServer = SP_ERROR_UNABLE_TO_CONTACT_SERVER,  /**< Cannot connect to the spotify backend system */
        BadUserNameOrPassword = SP_ERROR_BAD_USERNAME_OR_PASSWORD,  /**< Login failed because of bad username and/or password */
        UserBanned = SP_ERROR_USER_BANNED,                          /**< The specified username is banned */
        UserNeedsPremium = SP_ERROR_USER_NEEDS_PREMIUM,             /**< The specified user needs a premium account */
        OtherTransient = SP_ERROR_OTHER_TRANSIENT,                  /**< A transient error occurred */
        OtherPermanent = SP_ERROR_OTHER_PERMANENT                   /**< Some other error occurred, and it's permanent */
    };

    /*!
     * \brief The OfflineError enum represents error when trying to download stuff
     */
    enum OfflineError {
        TooManyTracks = SP_ERROR_OFFLINE_TOO_MANY_TRACKS,   /**< Reached the device limit for number of tracks to download */
        DiskCache = SP_ERROR_OFFLINE_DISK_CACHE,            /**< Disk cache is full, so no more tracks can be downloaded */
        Expired = SP_ERROR_OFFLINE_EXPIRED,                 /**< Offline key has expired, user needs to go online again */
        NotAllowed = SP_ERROR_OFFLINE_NOT_ALLOWED,          /**< This user is not allowed to use offline mode */
        LicenseLost = SP_ERROR_OFFLINE_LICENSE_LOST,        /**< The licese for this device has been lost. */
        LicenseError = SP_ERROR_OFFLINE_LICENSE_ERROR       /**< The Spotify license server does not respond correctly */
    };

    /*!
     * \brief The StreamingQuality enum represents different streaming qualities
     */
    enum StreamingQuality {
        Unknown = -1,
        LowQuality = SP_BITRATE_96k,    /**< Bitrate 96 kbps */
        HighQuality = SP_BITRATE_160k,  /**< Bitrate 160 kbps */
        UltraQuality = SP_BITRATE_320k  /**< Bitrate 320 kbps */
    };

    /*!
     * \brief The ConnectionRule enum represents different rules for the Spotify connection
     */
    enum ConnectionRule {
        AllowNetwork = SP_CONNECTION_RULE_NETWORK,                          /**< Allow network traffic. WHen not set, Libspotify will force itself into offline mode */
        AllowNetworkIfRoaming = SP_CONNECTION_RULE_NETWORK_IF_ROAMING,      /**< Allow network traffic even if roaming */
        AllowSyncOverMobile = SP_CONNECTION_RULE_ALLOW_SYNC_OVER_MOBILE,    /**< Set to allow syncing of offline content over mobile connections */
        AllowSyncOverWifi = SP_CONNECTION_RULE_ALLOW_SYNC_OVER_WIFI         /**< Set to allow syncing of foffline content over WiFi */
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
