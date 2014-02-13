#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifytrack.h>
#include <QtSpotify/spotifyuser.h>

#include <QtCore/QVector>

#include "../playlistevents.h"

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

bool SpotifyPlaylist::event(QEvent* e)
{
    if(e->type() == QEvent::User + 1) {
        //Tracks added event
        PlaylistTracksAddedEvent* ev = static_cast<PlaylistTracksAddedEvent*>(e);
        onTracksAdded(ev->tracks(), ev->position());
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 2) {
        //Tracks removed event
        PlaylistTracksRemovedEvent* ev = static_cast<PlaylistTracksRemovedEvent*>(e);
        onTracksRemoved(ev->indices());
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 3) {
        //Playlist renamed event
        m_name = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 4) {
        //Playlist state changed event
    }
    else if(e->type() == QEvent::User + 5) {
        //Playlist updated in progress event
    }
    else if(e->type() == QEvent::User + 6) {
        //Plylist metadata changed event
        updateData();
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 7) {
        //Track created changed event
    }
    else if(e->type() == QEvent::User + 8) {
        //Track seen changed event
        PlaylistTrackSeenChangedEvent* ev = static_cast<PlaylistTrackSeenChangedEvent*>(e);
        //m_tracks.at(ev->position())->setSeen(ev->seen());
    }
    else if(e->type() == QEvent::User + 9) {
        //Playlist description changed event
        PlaylistDescriptionChangedEvent* ev = static_cast<PlaylistDescriptionChangedEvent*>(e);
        m_description = ev->description();
    }
    else if(e->type() == QEvent::User + 10) {
        //Playlist image changed event
    }
    else if(e->type() == QEvent::User + 11) {
        //Track message changed event
    }
    else if(e->type() == QEvent::User + 12) {
        //Playlist subscribers changed
        PlaylistSubscribersChangedEvent* ev = static_cast<PlaylistSubscribersChangedEvent*>(e);
        m_subscribers = ev->subscribers();
    }

    QObject::event(e);
    return false;
}

void SpotifyPlaylist::updateData()
{

}

void SpotifyPlaylist::onTracksAdded(QList<SpotifyTrack*> tracks, qint32 position)
{

}

void SpotifyPlaylist::onTracksRemoved(QList<qint32> indices)
{

}
