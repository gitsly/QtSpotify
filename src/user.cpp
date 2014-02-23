#include <QtSpotify/Core/user.h>
#include <QtSpotify/Core/deleters.h>

#include <libspotify/api.h>

namespace QtSpotify {

User::User(sp_user* user)
{
    sp_user_add_ref(user);
    m_spUser = std::shared_ptr<sp_user>(user, deleteUser);
}

User::~User()
{

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

}
