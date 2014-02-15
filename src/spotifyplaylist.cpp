#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifytrack.h>
#include <QtSpotify/spotifyuser.h>
#include <QtSpotify/spotifyplaylistcontainer.h>

#include <QtSpotify/playlistcallbacks.h>
#include <QtSpotify/playlistevents.h>

#include <QtCore/QVector>

QHash<sp_playlist*, SpotifyPlaylist*> SpotifyPlaylist::playlistObjects = QHash<sp_playlist*, SpotifyPlaylist*>();

SpotifyPlaylist::SpotifyPlaylist(sp_playlist* playlist)
{
    sp_playlist_add_ref(playlist);
    m_spPlaylist = playlist;

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyPlaylist::updateData);

    m_callbacks = new sp_playlist_callbacks();
    m_callbacks->description_changed = descriptionChangedCallback;
    m_callbacks->image_changed = imageChangedCallback;
    m_callbacks->playlist_metadata_updated = playlistMetadataChangedCallback;
    m_callbacks->playlist_renamed = playlistRenamedCallback;
    m_callbacks->playlist_state_changed = playlistStateChangedCallback;
    m_callbacks->playlist_update_in_progress = playlistUpdateInProgressCallback;
    m_callbacks->subscribers_changed = subscribersChangedCallback;
    m_callbacks->tracks_added = tracksAddedCallback;
    m_callbacks->tracks_moved = tracksMovedCallback;
    m_callbacks->tracks_removed = tracksRemovedCallback;
    m_callbacks->track_created_changed = trackCreatedChangedCallback;
    m_callbacks->track_message_changed = trackMessageChangedCallback;
    m_callbacks->track_seen_changed = trackSeenChangedCallback;
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

SpotifyUser* SpotifyPlaylist::owner() const
{
    return m_owner;
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
        //Tracks moved event
        PlaylistTracksMovedEvent* ev = static_cast<PlaylistTracksMovedEvent*>(e);
        onTracksMoved(ev->indices(), ev->position());
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 4) {
        //Playlist renamed event
        m_name = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 5) {
        //Playlist state changed event
    }
    else if(e->type() == QEvent::User + 6) {
        //Playlist updated in progress event
    }
    else if(e->type() == QEvent::User + 7) {
        //Plylist metadata changed event
        updateData();
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 8) {
        //Track created changed event
    }
    else if(e->type() == QEvent::User + 9) {
        //Track seen changed event
        PlaylistTrackSeenChangedEvent* ev = static_cast<PlaylistTrackSeenChangedEvent*>(e);
        //m_tracks.at(ev->position())->setSeen(ev->seen());
    }
    else if(e->type() == QEvent::User + 10) {
        //Playlist description changed event
        PlaylistDescriptionChangedEvent* ev = static_cast<PlaylistDescriptionChangedEvent*>(e);
        m_description = ev->description();
    }
    else if(e->type() == QEvent::User + 11) {
        //Playlist image changed event
    }
    else if(e->type() == QEvent::User + 12) {
        //Track message changed event
    }
    else if(e->type() == QEvent::User + 13) {
        //Playlist subscribers changed
        PlaylistSubscribersChangedEvent* ev = static_cast<PlaylistSubscribersChangedEvent*>(e);
        m_subscribers = ev->subscribers();
    }

    QObject::event(e);
    return false;
}

void SpotifyPlaylist::updateData()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
    QString description = QString::fromUtf8(sp_playlist_get_description(m_spPlaylist));
    bool collaborative = sp_playlist_is_collaborative(m_spPlaylist);
    PlaylistOfflineStatus offlineStatus = PlaylistOfflineStatus(sp_playlist_get_offline_status(SpotifySession::instance()->native(), m_spPlaylist));

    if(m_subscribers.isEmpty()) {

        if(m_spSubscribers != nullptr) {
            sp_playlist_subscribers_free(m_spSubscribers);
        }
        m_spSubscribers = sp_playlist_subscribers(m_spPlaylist);

        for(qint32 i=0 ; i<m_spSubscribers->count ; ++i) {
            m_subscribers.append(QString::fromUtf8(m_spSubscribers->subscribers[i]));
        }

        updated = true;
    }

    if(m_tracks.isEmpty()) {
        qint32 numTracks = sp_playlist_num_tracks(m_spPlaylist);
        for(qint32 i=0 ; i<numTracks ; ++i) {
            SpotifyTrack* track = new SpotifyTrack(sp_playlist_track(m_spPlaylist, i));
            if(track != nullptr) {
                m_tracks.append(track);
            }
        }
        updated = true;
    }

    auto updateType = [=]() {

        qint32 index = m_owner->playlistContainer()->playlists().indexOf(this);
        if(index > -1) {
            sp_playlistcontainer_playlist_type(m_owner->playlistContainer()->native(), index);
        }
    };

    if(m_owner == nullptr) {
        sp_user* owner = sp_playlist_owner(m_spPlaylist);
        if(owner != nullptr) {
            m_owner = new SpotifyUser(owner);
        }

        updateType();

        updated = true;
    }
    else {
        updateType();
    }

    updated |= exchange(m_name, name);
    updated |= exchange(m_description, description);
    updated |= exchange(m_collaborative, collaborative);
    updated |= exchange(m_offlineStatus, offlineStatus);

}

void SpotifyPlaylist::onTracksAdded(QList<SpotifyTrack*> tracks, qint32 position)
{

}

void SpotifyPlaylist::onTracksRemoved(QList<qint32> indices)
{

}

void SpotifyPlaylist::onTracksMoved(QList<qint32> indices, qint32 position)
{

}
