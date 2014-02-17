#include <QtSpotify/User>
#include <QtSpotify/Session>
#include <QtSpotify/Playlist>
#include <QtSpotify/PlaylistContainer>

#include <libspotify/api.h>

User::User(sp_user* user) :
    QObject(nullptr),
    m_playlistContainer(nullptr),
    m_starredList(nullptr),
    m_displayName(""),
    m_canonicalName("")
{
    sp_user_add_ref(user);
    m_spUser = user;

    connect(Session::instance(), &Session::metadataUpdated, this, &User::onMetadataUpdated);
    onMetadataUpdated();
}

User::~User()
{
    delete m_starredList;
    delete m_playlistContainer;

    sp_user_release(m_spUser);
}

bool User::loaded() const
{
    return sp_user_is_loaded(m_spUser);
}

Playlist* User::starredList() const
{
    return m_starredList;
}

PlaylistContainer* User::playlistContainer() const
{
    return m_playlistContainer;
}

QString User::canonicalName() const
{
    return m_canonicalName;
}

QString User::displayName() const
{
    return m_displayName;
}

sp_user* User::native() const
{
    return m_spUser;
}

void User::onMetadataUpdated()
{
    bool updated = false;

    QString canonicalName = QString::fromUtf8(sp_user_canonical_name(m_spUser));
    QString displayName = QString::fromUtf8(sp_user_display_name(m_spUser));

    updated |= exchange(m_canonicalName, canonicalName);
    updated |= exchange(m_displayName, displayName);

    if(m_playlistContainer == nullptr) {

        PlaylistContainer* container = nullptr;

        if(Session::instance()->user() == this) {
            container = new PlaylistContainer(sp_session_playlistcontainer(Session::instance()->native()));
        }
        else {
            container = new PlaylistContainer(sp_session_publishedcontainer_for_user_create(Session::instance()->native(), this->canonicalName().toUtf8().constData()));
        }

        m_playlistContainer = container;
        updated = true;
    }

    if(m_starredList == nullptr) {

        Playlist* starredList = nullptr;

        if(Session::instance()->user() == this) {
            starredList = new Playlist(sp_session_starred_create(Session::instance()->native()));
        }
        else {
            starredList = new Playlist(sp_session_starred_for_user_create(Session::instance()->native(), this->canonicalName().toUtf8().constData()));
        }

        m_starredList = starredList;
        updated = true;
    }

    if(updated) {
        emit metadataChanged();
    }
}
