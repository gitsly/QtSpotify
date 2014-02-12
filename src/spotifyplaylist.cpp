#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifysession.h>

QHash<sp_playlist*, SpotifyPlaylist*> SpotifyPlaylist::playlistObjects = QHash<sp_playlist*, SpotifyPlaylist*>;

SpotifyPlaylist::SpotifyPlaylist()
{
    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyPlaylist::updateData);
}

SpotifyPlaylist::~SpotifyPlaylist()
{

}

bool SpotifyPlaylist::loaded() const
{

}

bool SpotifyPlaylist::availableOffline() const
{

}

QString SpotifyPlaylist::name() const
{

}

QString SpotifyPlaylist::description() const
{

}

bool SpotifyPlaylist::collaborative() const
{

}

QStringList SpotifyPlaylist::subscribers() const
{

}

PlaylistOfflineStatus SpotifyPlaylist::offlineStatus() const
{

}

PlaylistType SpotifyPlaylist::type() const
{

}

QList<SpotifyTrack*> SpotifyPlaylist::tracks() const
{

}

void SpotifyPlaylist::setName(const QString& name)
{

}

void SpotifyPlaylist::setCollaborative(bool collaborative)
{

}

void SpotifyPlaylist::setAvailableOffline(bool download)
{

}

void SpotifyPlaylist::add(SpotifyTrack* track)
{

}

void SpotifyPlaylist::add(QList<SpotifyTrack*> tracks)
{

}

void SpotifyPlaylist::remove(SpotifyTrack* track)
{

}

void SpotifyPlaylist::remove(QList<SpotifyTrack*> tracks)
{

}

void SpotifyPlaylist::move(SpotifyTrack* track, qint32 newPosition)
{

}

void SpotifyPlaylist::move(QList<SpotifyTrack*> tracks, qint32 newPosition)
{

}

void SpotifyPlaylist::createFromTrack(SpotifyTrack* track)
{

}

void SpotifyPlaylist::createFromAlbum(SpotifyAlbumBrowse* album)
{

}
