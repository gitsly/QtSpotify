#include <QtSpotify/Core/user.h>
#include <QtSpotify/Core/deleters.h>
#include <QtSpotify/Core/spotify.h>

#include <QtSpotify/Core/playlist.h>
#include <QtSpotify/Core/playlistcontainer.h>

#include <QtCore/QDebug>

#include <libspotify/api.h>

namespace QtSpotify {

User::User(sp_user* user)
{
    sp_user_add_ref(user);
    m_spUser = std::shared_ptr<sp_user>(user, deleteUser);

    loadMetaData();
}

User::~User()
{

}

bool User::loaded() const
{
    return sp_user_is_loaded(m_spUser.get());
}

QString User::displayName() const
{
    return m_displayName;
}

QString User::canonicalName() const
{
    return m_canonicalName;
}

Playlist* User::starredList() const
{
    return m_starredList.get();
}

PlaylistContainer* User::playlistContainer() const
{
    return m_playlistContainer.get();
}

void User::loadMetaData()
{
    qDebug() << "User loaded status: " << loaded();

    m_displayName = QString::fromUtf8(sp_user_display_name(m_spUser.get()));
    m_canonicalName = QString::fromUtf8(sp_user_canonical_name(m_spUser.get()));
    m_starredList = std::make_shared<Playlist>(sp_session_starred_for_user_create(
                                                   Spotify::instance().session().get(), m_canonicalName.toUtf8().constData()
                                                   ));
    m_playlistContainer = std::make_shared<PlaylistContainer>(sp_session_publishedcontainer_for_user_create(
                                                                  Spotify::instance().session().get(), m_canonicalName.toUtf8().constData()
                                                                  ));
}

}
