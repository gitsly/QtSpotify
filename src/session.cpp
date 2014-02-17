#include <QtSpotify/Session>

Session::Session()
{

}

Session* Session::instance()
{
    static Session instance;
    return &instance;
}

sp_session* Session::native() const
{
    return m_spSession;
}

bool Session::offlineMode() const
{
    return m_offlineMode;
}

User* Session::user() const
{
    return m_user;
}
