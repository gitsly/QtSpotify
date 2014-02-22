#include <QtSpotify/Core/user.h>
#include <QtSpotify/Core/deleters.h>

namespace QtSpotify {

User::User(sp_user* user)
{
    sp_user_add_ref(user);
    m_spUser = std::shared_ptr<sp_user>(user, deleteUser);
}

User::~User()
{

}

}
