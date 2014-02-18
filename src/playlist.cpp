#include <QtSpotify/Playlist>
#include <QtSpotify/Session>
#include <QtSpotify/Track>
#include <QtSpotify/User>

#include <QtSpotify/Core/playlistcallbacks.h>

QHash<sp_playlist*, Playlist*> Playlist::playlistObjects = QHash<sp_playlist*, Playlist*>();

Playlist::Playlist(sp_playlist* playlist) :
    QObject(nullptr),
    m_name(""),
    m_owner(nullptr),
    m_collaborative(false),
    m_description(""),
    m_offlineStatus(PlaylistOfflineStatus::No)
{
    sp_playlist_add_ref(playlist);
    m_spPlaylist = playlist;

    m_callbacks = new sp_playlist_callbacks();
    m_callbacks->description_changed = descriptionChangedCallback;
    m_callbacks->image_changed = imageChangedCallback;
    m_callbacks->playlist_metadata_updated = playlistMetadataUpdatedCallback;
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
    sp_playlist_add_callbacks(m_spPlaylist, m_callbacks, nullptr);

    connect(this, &Playlist::nameChanged, this, &Playlist::metadataChanged);
    connect(this, &Playlist::descriptionChanged, this, &Playlist::metadataChanged);

    connect(Session::instance(), &Session::metadataUpdated, this, &Playlist::onMetadataUpdated);
    onMetadataUpdated();
}

Playlist::~Playlist()
{
    qDeleteAll(m_tracks);
    m_tracks.clear();

    delete m_owner;

    qDeleteAll(m_subscribers);
    m_subscribers.clear();

    sp_playlist_remove_callbacks(m_spPlaylist, m_callbacks, nullptr);
    sp_playlist_release(m_spPlaylist);
}

bool Playlist::loaded() const
{
    return sp_playlist_is_loaded(m_spPlaylist);
}

QList<Track*> Playlist::tracks() const
{
    return m_tracks;
}

QString Playlist::name() const
{
    return m_name;
}

User* Playlist::owner() const
{
    return m_owner;
}

bool Playlist::collaborative() const
{
    return m_collaborative;
}

void Playlist::setCollaborative(bool collaborative)
{
    if(exchange(m_collaborative, collaborative)) {
        sp_playlist_set_collaborative(m_spPlaylist, collaborative);
    }
}

QString Playlist::description() const
{
    return m_description;
}

QList<User*> Playlist::subscribers() const
{
    return m_subscribers;
}

PlaylistOfflineStatus Playlist::offlineStatus() const
{
    return m_offlineStatus;
}

sp_playlist* Playlist::native() const
{
    return m_spPlaylist;
}

void Playlist::add(Track* track, qint32 position)
{
    m_tracks.insert(position, track);

    sp_track* t = track->native();
    sp_playlist_add_tracks(m_spPlaylist, const_cast<sp_track* const*>(&t), 1, position, Session::instance()->native());
}

void Playlist::remove(Track* track)
{
    qint32 trackPosition = m_tracks.indexOf(track);

    sp_playlist_remove_tracks(m_spPlaylist, &trackPosition, 1);
}

void Playlist::move(Track* track, qint32 newPosition)
{
    qint32 trackPosition = m_tracks.indexOf(track);

    sp_playlist_reorder_tracks(m_spPlaylist, &trackPosition, 1, newPosition);
}

bool Playlist::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        //Tracks added event
        TracksAddedEvent* ev = static_cast<TracksAddedEvent*>(e);
        for(qint32 i=0 ; i<ev->tracks().count() ; ++i) {
            qint32 trackIndex = getTrackIndex(ev->tracks().at(i));
            if(trackIndex == -1) {
                //Something went wrong adding the tracks
            }
            else if(trackIndex == ev->position() + i) {
                //Tracks are where they should be :)
            }
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 1) {
        //Tracks removed event
        TracksRemovedEvent* ev = static_cast<TracksRemovedEvent*>(e);
        for(qint32 i=0 ; i<ev->indices().count() ; ++i) {
            m_tracks.removeAt(ev->indices().at(i));
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 2) {
        //Tracks moved event
        TracksMovedEvent* ev = static_cast<TracksMovedEvent*>(e);
        for(qint32 i=0 ; i<ev->indices().count() ; ++i) {
            m_tracks.move(ev->indices().at(i), ev->position() + i);
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 3) {
        //Playlist renamed event
        QString newName = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
        if(exchange(m_name, newName)) {
            emit nameChanged(m_name);
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 4) {
        //Playlist state changed event
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 5) {
        //Playlist update in progress event
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 6) {
        //Playlist metadata updated event
        onMetadataUpdated();
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 7) {
        //Track created changed event
        TrackCreatedChangedEvent* ev = static_cast<TrackCreatedChangedEvent*>(e);
        QMetaObject::invokeMethod(m_tracks.at(ev->position()), "onMetadataUpdated");
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 8) {
        //Track seen changed event
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 9) {
        //Description changed event
        DescriptionChangedEvent* ev = static_cast<DescriptionChangedEvent*>(e);
        if(exchange(m_description, ev->description())) {
            emit descriptionChanged(m_description);
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 10) {
        //Image changed event
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 11) {
        //Track message changed event
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 12) {
        //Subscribers changed event
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void Playlist::onMetadataUpdated()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
    bool collaborative = sp_playlist_is_collaborative(m_spPlaylist);
    QString description = sp_playlist_get_description(m_spPlaylist);
    PlaylistOfflineStatus offlineStatus = PlaylistOfflineStatus(sp_playlist_get_offline_status(Session::instance()->native(), m_spPlaylist));

    if(m_tracks.isEmpty()) {

        qint32 numTracks = sp_playlist_num_tracks(m_spPlaylist);
        for(qint32 i=0 ; i<numTracks ; ++i) {
            Track* track = new Track(sp_playlist_track(this->native(), i), this);
            if(track != nullptr) {
                m_tracks.append(track);
            }
        }

        updated = true;
    }

    if(m_subscribers.isEmpty()) {

        sp_subscribers* subscribers = sp_playlist_subscribers(m_spPlaylist);

        for(quint32 i=0 ; i<subscribers->count ; ++i) {
            QString userLink = QString("%1%2").arg("spotify:user:").arg(subscribers->subscribers[i]);
            sp_link* link = sp_link_create_from_string(userLink.toUtf8().constData());
            sp_user* user = sp_link_as_user(link);
            sp_user_add_ref(user);
            sp_link_release(link);

            m_subscribers.append(new User(user));
            sp_user_release(user);
        }

        updated = true;
    }

    if(m_owner == nullptr) {
        sp_user* user = sp_playlist_owner(m_spPlaylist);
        User* owner = new User(user);

        m_owner = owner;
        updated = true;
    }

    updated |= exchange(m_name, name);
    updated |= exchange(m_collaborative, collaborative);
    updated |= exchange(m_description, description);
    updated |= exchange(m_offlineStatus, offlineStatus);

    if(updated) {
        emit metadataChanged();
    }
}

qint32 Playlist::getTrackIndex(sp_track *track)
{
    for(qint32 i=0 ; i<m_tracks.count() ; ++i) {
        if(m_tracks.at(i)->native() == track) {
            return i;
        }
    }

    return -1;
}
