#ifndef QTSPOTIFY_SPOTIFY_H_
#define QTSPOTIFY_SPOTIFY_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Core/deleters.h>

#include <libspotify/api.h>
#include <memory>

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtNetwork/QNetworkConfigurationManager>

namespace QtSpotify {

class User;

class QTS_EXPORT Spotify : public QObject
{

    Q_OBJECT

public:

    virtual ~Spotify();

    static Spotify& instance();

    QtSpotify::User* user() const;
    std::shared_ptr<sp_session> session() const;

protected:

    bool event(QEvent*);

public slots:

    void login(const QString& username, const QString& password = QString(), bool rememberMe = false);
    void logout(bool keepLoginInformation = true);

signals:

    void metadataUpdated();

private:

    Spotify();
    Q_DISABLE_COPY(Spotify)

    void init();
    void checkNetwork();
    void processSpotifyEvents();

    static std::shared_ptr<Spotify> m_instance;

    std::shared_ptr<QtSpotify::User> m_user;

    std::shared_ptr<sp_session> m_spSession;
    std::shared_ptr<sp_session_callbacks> m_callbacks;
    std::shared_ptr<sp_session_config> m_config;

    qint32 m_processTimerId;

    std::shared_ptr<QNetworkConfigurationManager> m_networkManager;

    static void SP_CALLCONV loggedInCallback(sp_session*, sp_error error);
    static void SP_CALLCONV loggedOutCallback(sp_session*);
    static void SP_CALLCONV metadataUpdatedCallback(sp_session*);
    static void SP_CALLCONV connectionErrorCallback(sp_session*, sp_error error);

    static void SP_CALLCONV notifyMainThreadCallback(sp_session*);

    static void SP_CALLCONV logMessageCallback(sp_session*, const char* message);

    static void SP_CALLCONV offlineErrorCallback(sp_session*, sp_error error);

};

}

#endif
