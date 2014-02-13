#include <QtSpotify/spotifyuser.h>
#include <QtSpotify/spotifysession.h>

SpotifyUser::SpotifyUser(sp_user* user) :
    QObject(nullptr),
    m_canonicalName(""),
    m_displayName(""),
    m_loaded(false)
{
    sp_user_add_ref(user);
    m_spUser = user;

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyUser::updateData);
}

SpotifyUser::~SpotifyUser()
{
    sp_user_release(m_spUser);
}

QString SpotifyUser::displayName() const
{
    return m_displayName;
}

QString SpotifyUser::canonicalName() const
{
    return m_canonicalName;
}

bool SpotifyUser::loaded() const
{
    return sp_user_is_loaded(m_spUser);
}

void SpotifyUser::updateData()
{
    bool updated = false;

    QString canonicalName = QString::fromUtf8(sp_user_canonical_name(m_spUser));
    QString displayName = QString::fromUtf8(sp_user_display_name(m_spUser));
    bool loaded = sp_user_is_loaded(m_spUser);

    if(exchange(canonicalName, m_canonicalName)) {
        updated = true;
    }

    if(exchange(displayName, m_displayName)) {
        updated = true;
    }

    if(exchange(m_loaded, loaded)) {
        updated = true;
    }
}
