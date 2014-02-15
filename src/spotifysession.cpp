#include <QtSpotify/spotifysession.h>

SpotifySession* SpotifySession::m_instance = nullptr;

SpotifySession::SpotifySession() :
    QObject(nullptr),
    m_spSession(nullptr),
    m_user(nullptr)
{

}

SpotifySession* SpotifySession::instance()
{
    if(m_instance == nullptr) {
        static QMutex mutex;
        mutex.lock();

        if(m_instance == nullptr) {
            m_instance = new SpotifySession();
            m_instance->init();
        }
    }

    return m_instance;
}

void SpotifySession::init()
{

}

sp_session* SpotifySession::native() const
{
    return m_spSession;
}

SpotifyUser* SpotifySession::user() const
{
    return m_user;
}
