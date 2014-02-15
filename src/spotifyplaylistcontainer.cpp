#include <QtSpotify/spotifyplaylistcontainer.h>
#include <QtSpotify/playlistcontainercallbacks.h>

QHash<sp_playlistcontainer*, SpotifyPlaylistContainer*> SpotifyPlaylistContainer::containerObjects = QHash<sp_playlistcontainer*, SpotifyPlaylistContainer*>();

SpotifyPlaylistContainer::SpotifyPlaylistContainer(sp_playlistcontainer* playlistContainer) :
    QObject(nullptr)
{
    sp_playlistcontainer_add_ref(playlistContainer);
    m_spContainer = playlistContainer;

    containerObjects.insert(m_spContainer, this);

    m_callbacks = new sp_playlistcontainer_callbacks();
    m_callbacks->container_loaded = containerLoadedCallback;
    m_callbacks->playlist_added = playlistAddedCallback;
    m_callbacks->playlist_moved = playlistMovedCallback;
    m_callbacks->playlist_removed = playlistRemovedCallback;
    sp_playlistcontainer_add_callbacks(m_spContainer, m_callbacks, nullptr);
}

SpotifyPlaylistContainer::~SpotifyPlaylistContainer()
{
    sp_playlistcontainer_release(m_spContainer);
}

bool SpotifyPlaylistContainer::loaded() const
{
    return true;
}

QList<SpotifyPlaylist*> SpotifyPlaylistContainer::playlists() const
{
    return m_playlists;
}

sp_playlistcontainer* SpotifyPlaylistContainer::native() const
{
    return m_spContainer;
}

void SpotifyPlaylistContainer::addPlaylist(SpotifyPlaylist* playlist)
{

}

void SpotifyPlaylistContainer::createPlaylist(const QString& name)
{

}

void SpotifyPlaylistContainer::removePlaylist(SpotifyPlaylist* playlist)
{

}

void SpotifyPlaylistContainer::movePlaylist(SpotifyPlaylist* playlist, qint32 newPosition)
{

}

void SpotifyPlaylistContainer::addFolder(qint32 position, const QString& name)
{

}
