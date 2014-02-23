#include <QtSpotify/Core/spotify.h>
#include <QtSpotify/Core/apikey.h>
#include <QtSpotify/Core/deleters.h>

#include <QtSpotify/Core/user.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

namespace QtSpotify {

Spotify::Spotify() :
    QObject(nullptr),
    m_processTimerId(0),
    m_user(nullptr),
    m_callbacks(nullptr),
    m_config(nullptr),
    m_networkManager(nullptr)
{
    m_networkManager = std::make_shared<QNetworkConfigurationManager>();

    m_callbacks = std::shared_ptr<sp_session_callbacks>(new sp_session_callbacks());
    m_callbacks->logged_in = &Spotify::loggedInCallback;
    m_callbacks->logged_out = &Spotify::loggedOutCallback;
    m_callbacks->metadata_updated = &Spotify::metadataUpdatedCallback;
    m_callbacks->connection_error = &Spotify::connectionErrorCallback;
    m_callbacks->log_message = &Spotify::logMessageCallback;
    m_callbacks->notify_main_thread = &Spotify::notifyMainThreadCallback;
    m_callbacks->offline_error = &Spotify::offlineErrorCallback;

    m_config = std::shared_ptr<sp_session_config>(new sp_session_config());
    m_config->api_version = SPOTIFY_API_VERSION;
    m_config->application_key = g_appkey;
    m_config->application_key_size = g_appkey_size;
    m_config->user_agent = "QtSpotify-xarxer-0.1";
    m_config->device_id = "xarxer-mainpc-1.0";
    m_config->cache_location = "cache";
    m_config->settings_location = "settings";
    m_config->callbacks = m_callbacks.get();

    sp_session* tmpSession;

    sp_error error = sp_session_create(m_config.get(), &tmpSession);
    m_spSession = std::shared_ptr<sp_session>(tmpSession, deleteSession);

    checkNetwork();

    if(error != SP_ERROR_OK) {
        qDebug() << "Session creation error: " << QString::fromUtf8(sp_error_message(error));
    }
    else {

    }
}

Spotify::~Spotify()
{

}

Spotify& Spotify::instance()
{
    static Spotify inst;
    return inst;
}

User* Spotify::user() const
{
    return m_user.get();
}

std::shared_ptr<sp_session> Spotify::session() const
{
    return m_spSession;
}

bool Spotify::event(QEvent* e)
{
    switch(e->type()) {
        case QEvent::User:
        {
            //Logged in event
            qDebug() << "Spotify::event | Logged in event";
            m_user = std::make_shared<User>(sp_session_user(m_spSession.get()));
            checkNetwork();
            e->accept();
            return true;
        }
        case QEvent::User + 1:
            //Logged out event
            qDebug() << "Spotify::event | Logged out event";
            e->accept();
            return true;
        case QEvent::User + 2:
            //Metadata updated event
            qDebug() << "Spotify::event | Metadata updated event";
            emit metadataUpdated();
            e->accept();
            return true;
        case QEvent::User + 5:
            //Metadata updated event
            processSpotifyEvents();
            e->accept();
            return true;
        case QEvent::Timer:
        {
            QTimerEvent* te = static_cast<QTimerEvent*>(e);
            if(te->timerId() == m_processTimerId) {
                processSpotifyEvents();
                e->accept();
                return true;
            }
            else {
                return QObject::event(e);
            }
        }
        default:
            return QObject::event(e);
    }
}

void Spotify::login(const QString& username, const QString& password, bool rememberMe)
{
    sp_session_login(m_spSession.get(), username.toUtf8().constData(), password.toUtf8().constData(), rememberMe, nullptr);
}

void Spotify::logout(bool keepLoginInformation)
{
    sp_session_logout(m_spSession.get());
}

void Spotify::checkNetwork()
{
    if(!m_networkManager->isOnline()) {
        sp_session_set_connection_type(m_spSession.get(), SP_CONNECTION_TYPE_NONE);
        return;
    }

    QList<QNetworkConfiguration> activeConfigs = m_networkManager->allConfigurations(QNetworkConfiguration::Active);
    bool wired, wifi, mobile, roaming;
    wired = wifi = mobile = roaming = false;

    for(qint32 i=0 ; i<activeConfigs.count() ; ++i) {

        QNetworkConfiguration::BearerType bearerType = activeConfigs.at(i).bearerType();

        if(bearerType == QNetworkConfiguration::BearerEthernet) {
            wired = true;
            break;
        }
        else if(bearerType == QNetworkConfiguration::BearerWLAN) {
            wifi = true;
            break;
        }
        else if(bearerType == QNetworkConfiguration::Bearer2G ||
                bearerType == QNetworkConfiguration::Bearer3G ||
                bearerType == QNetworkConfiguration::Bearer4G ||
                bearerType == QNetworkConfiguration::BearerCDMA2000 ||
                bearerType == QNetworkConfiguration::BearerWCDMA ||
                bearerType == QNetworkConfiguration::BearerHSPA ||
                bearerType == QNetworkConfiguration::BearerWiMAX ||
                bearerType == QNetworkConfiguration::BearerEVDO ||
                bearerType == QNetworkConfiguration::BearerLTE) {
            mobile = true;
        }

        if(activeConfigs.at(i).isRoamingAvailable()) {
            roaming = true;
        }
    }

    sp_connection_type connectionType;
    if(wired) connectionType = SP_CONNECTION_TYPE_WIRED;
    else if(wifi) connectionType = SP_CONNECTION_TYPE_WIFI;
    else if(mobile) connectionType = SP_CONNECTION_TYPE_MOBILE;
    else if(roaming) connectionType = SP_CONNECTION_TYPE_MOBILE_ROAMING;
    else connectionType = SP_CONNECTION_TYPE_UNKNOWN;

    sp_session_set_connection_type(m_spSession.get(), connectionType);

    /*if(m_focedOfflineMode) {
        setOfflineMode(true, true);
    }
    else {
        setConnectionRule(ConnectionRule::AllowNetwork, !m_offlineMode);
    }*/
}

void Spotify::processSpotifyEvents()
{
    if(m_processTimerId != 0) {
        killTimer(m_processTimerId);
    }

    qint32 nextTimeout;

    do {
        sp_session_process_events(m_spSession.get(), &nextTimeout);
    } while(nextTimeout == 0);

    m_processTimerId = startTimer(nextTimeout);
}

void Spotify::loggedInCallback(sp_session*, sp_error error)
{
    if(error != SP_ERROR_OK) {
        QString message = QString::fromUtf8(sp_error_message(error));
        qDebug() << "loggedInCallback Error: " << message;
    }
    else {
        QCoreApplication::postEvent(&Spotify::instance(), new QEvent(QEvent::Type(QEvent::User)));
    }
}

void Spotify::loggedOutCallback(sp_session*)
{
    QCoreApplication::postEvent(&Spotify::instance(), new QEvent(QEvent::Type(QEvent::User + 1)));
}

void Spotify::metadataUpdatedCallback(sp_session*)
{
    QCoreApplication::postEvent(&Spotify::instance(), new QEvent(QEvent::Type(QEvent::User + 2)));
}

void Spotify::connectionErrorCallback(sp_session*, sp_error error)
{
    if(error != SP_ERROR_OK) {
        qDebug() << "Connection error callback: " << QString::fromUtf8(sp_error_message(error));
    }
}

void Spotify::notifyMainThreadCallback(sp_session*)
{
    QCoreApplication::postEvent(&Spotify::instance(), new QEvent(QEvent::Type(QEvent::User + 5)));
}

void Spotify::logMessageCallback(sp_session*, const char* message)
{
    qDebug() << QString::fromUtf8(message);
}

void Spotify::offlineErrorCallback(sp_session*, sp_error error)
{
    if(error != SP_ERROR_OK) {
        qDebug() << "Offline error callback: " << QString::fromUtf8(sp_error_message(error));
    }
}


}
