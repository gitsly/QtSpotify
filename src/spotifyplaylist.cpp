#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifytrack.h>

#include <QtCore/QVector>

QHash<sp_playlist*, SpotifyPlaylist*> SpotifyPlaylist::playlistObjects = QHash<sp_playlist*, SpotifyPlaylist*>();

SpotifyPlaylist::SpotifyPlaylist(sp_playlist* playlist)
{
    sp_playlist_add_ref(playlist);
    m_spPlaylist = playlist;

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyPlaylist::updateData);
}

SpotifyPlaylist::~SpotifyPlaylist()
{

}

bool SpotifyPlaylist::loaded() const
{
    return sp_playlist_is_loaded(m_spPlaylist);
}

bool SpotifyPlaylist::availableOffline() const
{
    return false;
}

QString SpotifyPlaylist::name() const
{
    return m_name;
}

QString SpotifyPlaylist::description() const
{
    return m_description;
}

bool SpotifyPlaylist::collaborative() const
{
    return m_collaborative;
}

QStringList SpotifyPlaylist::subscribers() const
{
    return m_subscribers;
}

PlaylistOfflineStatus SpotifyPlaylist::offlineStatus() const
{
    return m_offlineStatus;
}

PlaylistType SpotifyPlaylist::type() const
{
    return m_type;
}

QList<SpotifyTrack*> SpotifyPlaylist::tracks() const
{
    return m_tracks;
}

sp_playlist* SpotifyPlaylist::native() const
{
    return m_spPlaylist;
}

void SpotifyPlaylist::setName(const QString& name)
{
    sp_playlist_rename(m_spPlaylist, name.toUtf8().constData());
}

void SpotifyPlaylist::setCollaborative(bool collaborative)
{
    sp_playlist_set_collaborative(m_spPlaylist, collaborative);
}

void SpotifyPlaylist::setAvailableOffline(bool download)
{
    sp_playlist_set_offline_mode(SpotifySession::instance()->native(), m_spPlaylist, download);
}

void SpotifyPlaylist::add(SpotifyTrack* track)
{
    sp_track* t = track->native();
    sp_playlist_add_tracks(m_spPlaylist, const_cast<sp_track* const*>(&t), 1, m_tracks.count(), SpotifySession::instance()->native());
}

void SpotifyPlaylist::add(QList<SpotifyTrack*> tracks)
{
    QVector<sp_track*> t;
    foreach(SpotifyTrack* track , tracks) {
        t.append(track->native());
    }

    sp_playlist_add_tracks(m_spPlaylist, const_cast<sp_track* const*>(t.data()), t.count(), m_tracks.count(), SpotifySession::instance()->native());
}

void SpotifyPlaylist::remove(SpotifyTrack* track)
{
    if(track == nullptr) {
        return;
    }

    qint32 i = m_tracks.indexOf(track);

    if(i > -1) {
        sp_playlist_remove_tracks(m_spPlaylist, &i, 1);
    }
}

void SpotifyPlaylist::remove(QList<SpotifyTrack*> tracks)
{
    QVector<qint32> indices;

    for(qint32 i=0 ; i<tracks.count() ; ++i) {

        if(tracks.at(i) == nullptr) {
            continue;
        }

        qint32 index = m_tracks.indexOf(tracks.at(i));
        if(index > -1) {
            indices.append(index);
        }
    }

    if(!indices.isEmpty()) {
        sp_playlist_remove_tracks(m_spPlaylist, indices.data(), indices.count());
    }
}

void SpotifyPlaylist::move(SpotifyTrack* track, qint32 newPosition)
{
    qint32 index = m_tracks.indexOf(track);

    if(index > -1) {
        sp_playlist_reorder_tracks(m_spPlaylist, &index, 1, newPosition);
    }
}

void SpotifyPlaylist::move(QList<SpotifyTrack*> tracks, qint32 newPosition)
{
    QVector<qint32> indices;

    for(qint32 i=0 ; i<tracks.count() ; ++i) {

        if(tracks.at(i) == nullptr) {
            continue;
        }

        qint32 index = m_tracks.indexOf(tracks.at(i));
        if(index > -1) {
            indices.append(index);
        }
    }

    if(!indices.isEmpty()) {
        sp_playlist_reorder_tracks(m_spPlaylist, indices.data(), indices.count(), newPosition);
    }
}

void SpotifyPlaylist::createFromTrack(SpotifyTrack* track)
{

}

void SpotifyPlaylist::createFromAlbum(SpotifyAlbumBrowse* album)
{

}

void SpotifyPlaylist::updateData()
{

}
