#include <QtSpotify/user.h>
#include <libspotify/api.h>
#include <QtSpotify/session.h>

SpotifyUser::SpotifyUser(sp_user* user)
{
    sp_user_add_ref(user);
    m_spUser = user;

    m_canonicalName = QString::fromUtf8(sp_user_canonical_name(m_spUser));

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyUser::updateData);
}

SpotifyUser::~SpotifyUser()
{
    delete m_playlistContainer;
    sp_user_release(m_spUser);
}

bool SpotifyUser::loaded()
{
    return sp_user_is_loaded(m_spUser);
}

QString SpotifyUser::canonicalName() const
{
    return m_canonicalName;
}

QString SpotifyUser::displayName() const
{
    return m_displayName;
}

SpotifyPlaylistContainer* SpotifyUser::playlistContainer() const
{
    if(m_playlistContainer == nullptr) {

        sp_playlistcontainer* pc;

        if(SpotifySession::instance()->user() == this) {
            //Get the Playlist Container for the logged in user
            pc = sp_session_playlistcontainer(SpotifySession::instance()->spSession());
            sp_playlistcontainer_add_ref(pc);
        }
        else {
            //Get the Playlist Container for another user
            pc = sp_session_publishedcontainer_for_user_create(SpotifySession::instance()->spSession(), m_canonicalName.toUtf8().constData());
        }

        m_playlistContainer = new SpotifyPlaylistContainer(pc);
        connect(m_playlistContainer, &SpotifyPlaylistContainer::playlistContainerDataChanged, this, &SpotifyUser::playlistsChanged);
        connect(m_playlistContainer, &SpotifyPlaylistContainer::playlistsNameChanged, this, &SpotifyUser::playlistsNameChanged);
    }

    return m_playlistContainer;
}

SpotifyPlaylist* SpotifyUser::starredList() const
{
    if(m_starredList == nullptr) {

        sp_playlist* playlist = nullptr;

        if(SpotifySession::instance()->user() == this) {
            //Get the starred list for the logged in user
            playlist = sp_session_starred_create(SpotifySession::instance()->spSession());
        }
        else {
            //Get the starred list for another user
            playlist = sp_session_starred_for_user_create(SpotifySession::instance()->spSession(), m_canonicalName.toUtf8().constData());
        }

        m_starredList = new SpotifyPlaylist(SpotifyPlaylist::Starred, playlist, false);
    }

    return m_starredList;
}

SpotifyPlaylist* SpotifyUser::inbox() const
{
    if(SpotifySession::instance()->user() != this) {
        return nullptr;
    }

    if(m_inbox == nullptr) {
        sp_playlist* inbox = sp_session_inbox_create(SpotifySession::instance()->spSession());
        m_inbox = new SpotifyPlaylist(SpotifyPlaylist::Inbox, inbox, false);
    }

    return m_inbox;
}

QList<SpotifyPlaylist*> SpotifyUser::playlists() const
{
    return m_playlistContainer->playlists();
}

bool SpotifyUser::createPlaylist(const QString& name)
{
    if(name.trimmed().isEmpty()) {
        return false;
    }

    QString n = name.trimmed();
    if(n.size() > 255) {
        n.resize(255);
    }

    sp_playlist* playlist = sp_playlistcontainer_add_new_playlist(m_playlistContainer->spContainer(), n.toUtf8().constData());
    return playlist != nullptr;
}

bool SpotifyUser::createPlaylistInFolder(const QString& name, SpotifyPlaylist* folder)
{
    if(folder == nullptr || folder->type() != SpotifyPlaylist::Folder) {
        return createPlaylist(name);
    }

    //Names can't be empty
    if(name.trimmed().isEmpty()) {
        return false;
    }

    QString trimmed = name.trimmed();
    if(trimmed.size() > 255) {
        trimmed.resize(255);
    }

    sp_playlist* playlist = sp_playlistcontainer_add_new_playlist(m_playlistContainer->spContainer(), trimmed.toUtf8().constData());
    if(playlist == nullptr) {
        return false;
    }

    qint32 folderIndex = m_playlistContainer->playlists().indexOf(folder);
    if(folderIndex == -1) {
        return false;
    }

    sp_uint64 folderId = sp_playlistcontainer_playlist_folder_id(m_playlistContainer->spContainer(), folderIndex);
    int count = sp_playlistcontainer_num_playlists(m_playlistContainer->spContainer());

    for(qint32 j = folderIndex + 1 ; j < count ; ++j) {
        if(folderId == sp_playlistcontainer_playlist_folder_id(m_playlistContainer->spContainer(), j)) {
            folderIndex = j;
        }
    }

    sp_playlistcontainer_move_playlist(m_playlistContainer->spContainer(), count - 1, folderIndex, false);
    return true;
}

bool SpotifyUser::createPlaylistFromTrack(SpotifyTrack* track)
{
    if(track == nullptr) {
        return false;
    }

    sp_playlist* playlist = sp_playlistcontainer_add_new_playlist(m_playlistContainer->spContainer(), track->name().toUtf8().constData());
    if(playlist == nullptr) {
        return false;
    }

    sp_playlist_add_tracks(playlist, const_cast<sp_track* const*>(&track->spTrack()), 1, 0, SpotifySession::instance()->spSession());
    return true;
}

bool SpotifyUser::createPlaylistFromAlbum(SpotifyAlbumBrowse* albumBrowse)
{
    if(albumBrowse == nullptr || albumBrowse->tracks() == nullptr || albumBrowse->tracks().count() == 0) {
        return false;
    }

    QString name = QString("%1 - %2").arg(albumBrowse->album()->artist()->name()).arg(albumBrowse->album()->name());
    sp_playlist* playlist = sp_playlistcontainer_add_new_playlist(m_playlistContainer->spContainer(), name.toUtf8().constData());

    if(playlist == nullptr) {
        return false;
    }

    QVector<sp_track*> tracks;
    for(int i=0 ; i<albumBrowse->tracks().count() ; ++i) {
        tracks.append(albumBrowse->tracks().at(i)->spTrack());
    }

    sp_playlist_add_tracks(playlist, const_cast<sp_track* const*>(tracks.data()), tracks.count(), 0, SpotifySession::instance()->spSession());
    return true;
}

void SpotifyUser::removePlaylist(SpotifyPlaylist* playlist)
{
    if(playlist == nullptr) {
        return;
    }

    qint32 playlistIndex = m_playlistContainer->playlists().indexOf(playlist);
    if(i == -1) {
        return;
    }

    if(playlist->type() == SpotifyPlaylist::Folder) {
        sp_uint64 folderId = sp_playlistcontainer_playlist_folder_id(m_playlistContainer->spContainer(), playlistIndex);
        for(qint32 j = playlistIndex + 1 ; j < sp_playlistcontainer_num_playlists(m_playlistContainer->spContainer()) ; ++j) {
            if(sp_playlistcontainer_playlist_folder_id(m_playlistContainer->spContainer(), j) == folderId) {
                sp_playlistcontainer_remove_playlist(m_playlistContainer->spContainer(), j);
                break;
            }
        }
    }

    sp_playlistcontainer_remove_playlist(m_playlistContainer->spContainer(), playlistIndex);
}

bool SpotifyUser::ownsPlaylist(SpotifyPlaylist* playlist)
{
    return (playlist != nullptr && playlist->owner() == m_canonicalName);
}

bool SpotifyUser::canModifyPlaylist(SpotifyPlaylist* playlist)
{
    return (playlist != nullptr && (playlist->collaborative() || ownsPlaylist(playlist)));
}

void SpotifyUser::updateData()
{
    bool updated = false;

    QString canonicalName = QString::fromUtf8(sp_user_canonical_name(m_spUser));
    QString displayName = QString::fromUtf8(sp_user_display_name(m_spUser));

    if(m_canonicalName != canonicalName) {
        m_canonicalName = canonicalName;
        updated = true;
    }

    if(m_displayName != displayName) {
        m_displayName = displayName;
        updated = true;
    }

    emit userDataChanged();
}
