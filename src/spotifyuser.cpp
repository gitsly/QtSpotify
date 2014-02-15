#include <QtSpotify/spotifyuser.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifyplaylistcontainer.h>
#include <QtSpotify/spotifyplaylist.h>

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

SpotifyPlaylistContainer* SpotifyUser::playlistContainer()
{
    if(m_playlistContainer == nullptr) {
        sp_playlistcontainer* pc;
        if(SpotifySession::instance()->user() == this) {
            pc = sp_session_playlistcontainer(SpotifySession::instance()->native());
            sp_playlistcontainer_add_ref(pc);
        }
        else {
            pc = sp_session_publishedcontainer_for_user_create(SpotifySession::instance()->native(), m_canonicalName.toUtf8().constData());
        }

        m_playlistContainer = new SpotifyPlaylistContainer(pc);
    }

    return m_playlistContainer;
}

SpotifyPlaylist* SpotifyUser::starredList()
{
    if(m_starredList == nullptr) {
        sp_playlist* playlist;
        if(SpotifySession::instance()->user() == this) {
            playlist = sp_session_starred_create(SpotifySession::instance()->native());
        }
        else {
            playlist = sp_session_starred_for_user_create(SpotifySession::instance()->native(), m_canonicalName.toUtf8().constData());
        }

        m_starredList = new SpotifyPlaylist(playlist);
    }

    return m_starredList;
}

SpotifyPlaylist* SpotifyUser::inbox()
{
    if(SpotifySession::instance()->user() != this) {
        return nullptr;
    }

    if(m_inbox == nullptr) {
        sp_playlist* inbox = sp_session_inbox_create(SpotifySession::instance()->native());
        m_inbox = new SpotifyPlaylist(inbox);
    }

    return m_inbox;
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
