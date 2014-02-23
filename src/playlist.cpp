#include <QtSpotify/Core/playlist.h>
#include <QtSpotify/Core/track.h>
#include <QtSpotify/Spotify>
#include <QtSpotify/Core/deleters.h>

namespace QtSpotify {

QHash<sp_playlist*, Playlist*> Playlist::playlistObjects = QHash<sp_playlist*, Playlist*>();

Playlist::Playlist(sp_playlist* playlist)
{
    sp_playlist_add_ref(playlist);
    m_spPlaylist = std::shared_ptr<sp_playlist>(playlist, deletePlaylist);

    //Add this playlist to playlistObjects so the callbacks knows which class to post events to
    playlistObjects.insert(m_spPlaylist.get(), this);

    //Set up callbacks for this playlist
    m_callbacks = new sp_playlist_callbacks();
    m_callbacks->description_changed = &Playlist::descriptionChangedCallback;
    m_callbacks->image_changed = &Playlist::imageChangedCallback;
    m_callbacks->playlist_metadata_updated = &Playlist::playlistMetadataUpdatedCallback;
    m_callbacks->playlist_renamed = &Playlist::playlistRenamedCallback;
    m_callbacks->playlist_state_changed = &Playlist::playlistStateChangedCallback;
    m_callbacks->playlist_update_in_progress = &Playlist::playlistUpdateInProgressCallback;
    m_callbacks->subscribers_changed = &Playlist::subscribersChangedCallback;
    m_callbacks->tracks_added = &Playlist::tracksAddedCallback;
    m_callbacks->tracks_moved = &Playlist::tracksMovedCallback;
    m_callbacks->tracks_removed = &Playlist::tracksRemovedCallback;
    m_callbacks->track_created_changed = &Playlist::trackCreatedChangedCallback;
    m_callbacks->track_message_changed = &Playlist::trackMessageChangedCallback;
    m_callbacks->track_seen_changed = &Playlist::trackSeenChangedCallback;
    sp_playlist_add_callbacks(m_spPlaylist.get(), m_callbacks, nullptr);

    //Spotify says something, somewhere has been updated... Better check the playlist
    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Playlist::onMetadataUpdated);

    //If any property on the playlist changes, send out a signal that the playlist as a whole has changed
    connect(this, &Playlist::tracksChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::nameChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::descriptionChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::collaborativeChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::totalDurationChanged, this, &Playlist::playlistDataChanged);
}

Playlist::~Playlist()
{
    //These members go out of scope and are deleted automatically
    //m_tracks
    //m_owner
    //m_spPlaylist (Also sp_playlist_release() is called via the deleter func)

    //TODO: See if it's possible to use a deleter func
    sp_playlist_remove_callbacks(m_spPlaylist.get(), m_callbacks, nullptr);
    delete m_callbacks;
}

QString Playlist::name() const
{
    return m_name;
}

//TODO: Check in the callback if it was successful
void Playlist::setName(const QString &name)
{
    /*if( owner() != Spotify::instance().user() ) {
        return;
    }*/

    QString trimmedName = name.trimmed();
    if( trimmedName.size() <= 0 ) {
        return;
    }

    if(trimmedName != m_name) {
        sp_playlist_rename(m_spPlaylist.get(), trimmedName.toUtf8().constData());
    }
}

QString Playlist::description() const
{
    return m_description;
}

quint32 Playlist::totalDuration() const
{
    quint32 total = 0;

    for( auto it = m_tracks.constBegin() ; it != m_tracks.constEnd() ; ++it ) {
        total += (*it)->duration();
    }

    return total;
}

/*User* Playlist::owner() const
{
    return m_owner.get();
}*/

bool Playlist::collaborative() const
{
    return m_collaborative;
}


//TODO: Check in callback if the setting was successful
void Playlist::setCollaborative(bool collaborative)
{
    /*if( owner() != Spotify::instance().user() ) {
        return;
    }*/

    if(collaborative != m_collaborative) {
        sp_playlist_set_collaborative(m_spPlaylist.get(), collaborative);
    }
}

PlaylistOfflineStatus Playlist::offlineStatus() const
{
    return m_offlineStatus;
}

QQmlListProperty<Track> Playlist::tracks()
{
    return QQmlListProperty<Track>(this, nullptr, &Playlist::tracksAppendFunction, &Playlist::tracksCountFunction,
                                   &Playlist::tracksAtFunction, &Playlist::tracksClearFunction);
}

void Playlist::onMetadataUpdated()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_playlist_name(m_spPlaylist.get()));
    //std::shared_ptr<User> owner = std::make_shared<User>(sp_playlist_owner(m_spPlaylist.get()));
    bool collaborative = sp_playlist_is_collaborative(m_spPlaylist.get());
    QString description = sp_playlist_get_description(m_spPlaylist.get());
    PlaylistOfflineStatus offlineStatus = PlaylistOfflineStatus(sp_playlist_get_offline_status(Spotify::instance().session().get(), m_spPlaylist.get()));

    if(m_tracks.isEmpty()) {
        qint32 trackCount = sp_playlist_num_tracks(m_spPlaylist.get());

        for(qint32 i=0 ; i<trackCount ; ++i) {
            m_tracks.append(std::make_shared<Track>(sp_playlist_track(m_spPlaylist.get(), i), std::shared_ptr<Playlist>(this)));
        }

        updated = true;
    }

    updated |= exchange(m_name, name);
    //updated |= exchange(m_owner, owner);
    updated |= exchange(m_collaborative, collaborative);
    updated |= exchange(m_description, description);
    updated |= exchange(m_offlineStatus, offlineStatus);

    if(updated) {
        emit playlistDataChanged();
    }
}

bool Playlist::event(QEvent* e)
{
    switch( e->type() )
    {
        case QEvent::User:
        {
            //Tracks added event
            TracksAddedEvent* event = static_cast<TracksAddedEvent*>(e);
            onTracksAdded(event);
            e->accept();
            return true;
        }
        case QEvent::User + 1:
        {
            //Tracks removed event
            TracksRemovedEvent* event = static_cast<TracksRemovedEvent*>(e);
            onTracksRemoved(event);
            e->accept();
            return true;
        }
        case QEvent::User + 2:
        {
            //Tracks moved event
            TracksMovedEvent* event = static_cast<TracksMovedEvent*>(e);
            onTracksMoved(event);
            e->accept();
            return true;
        }
        case QEvent::User + 3:
        {
            //Playlist renamed event
            onNameChanged(QString::fromUtf8(sp_playlist_name(m_spPlaylist.get())));
            e->accept();
            return true;
        }
        case QEvent::User + 4:
            //Playlist state changed event
            break;
        case QEvent::User + 5:
            //Playlist update in progress event
            break;
        case QEvent::User + 6:
            //Playlist metadata updated event
            onMetadataUpdated();
            e->accept();
            return true;
        case QEvent::User + 7:
        {
            //Track created changed event
            TrackCreatedChangedEvent* event = static_cast<TrackCreatedChangedEvent*>(e);
            onTrackCreatedChanged(event);
            e->accept();
            return true;
        }
        case QEvent::User + 8:
        {
            //Track seen changed event
            TrackSeenChangedEvent* event = static_cast<TrackSeenChangedEvent*>(e);
            onTrackSeenChanged(event);
            e->accept();
            return true;
        }
        case QEvent::User + 9:
        {
            //Description changed event
            DescriptionChangedEvent* event = static_cast<DescriptionChangedEvent*>(e);
            onDescriptionChanged(event->description());
            e->accept();
            return true;
        }
        case QEvent::User + 12:
            //Subscribers changed event
            onSubscribersChanged();
            e->accept();
            return true;
    }

    return QObject::event(e);
}

void Playlist::onNameChanged(const QString& newName)
{
    if(exchange(m_name, newName)) {
        emit nameChanged(m_name);
    }
}

void Playlist::onCollaborativeChanged(bool collaborative)
{
    if(exchange(m_collaborative, collaborative)) {
        emit collaborativeChanged(m_collaborative);
    }
}

void Playlist::onDescriptionChanged(const QString& description)
{
    if(exchange(m_description, description)) {
        emit descriptionChanged(m_description);
    }
}

void Playlist::onTracksAdded(TracksAddedEvent* event)
{
    for(qint32 i=0 ; i<event->tracks().count() ; ++i) {
        m_tracks.insert(event->position() + i, event->tracks().at(i));
    }

    emit tracksChanged();
}

void Playlist::onTracksMoved(TracksMovedEvent* event)
{
    qint32 index = event->newPosition();

    for(qint32 i=0 ; i<event->indices().count() ; ++i) {

    }
}

void Playlist::onTracksRemoved(TracksRemovedEvent* event)
{

}

void Playlist::onTrackCreatedChanged(TrackCreatedChangedEvent* event)
{

}

void Playlist::onTrackSeenChanged(TrackSeenChangedEvent* event)
{

}

void Playlist::onSubscribersChanged()
{

}


void Playlist::tracksAppendFunction(QQmlListProperty<Track>* list, Track* track)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if( playlist != nullptr ) {
        playlist->m_tracks.append(std::shared_ptr<Track>(track));
    }
}

Track* Playlist::tracksAtFunction(QQmlListProperty<Track>* list, int index)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if( playlist != nullptr ) {
        return playlist->m_tracks.at(index).get();
    }

    return nullptr;
}

void Playlist::tracksClearFunction(QQmlListProperty<Track>* list)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if( playlist != nullptr ) {
        foreach( auto track, playlist->m_tracks ) {
            track.reset();
        }
        playlist->m_tracks.clear();
    }
}

qint32 Playlist::tracksCountFunction(QQmlListProperty<Track>* list)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if( playlist != nullptr ) {
        return playlist->m_tracks.count();
    }

    return 0;
}

void SP_CALLCONV Playlist::tracksAddedCallback(sp_playlist* playlist, sp_track* const* tracks, int numTracks, int position, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new TracksAddedEvent(tracks, numTracks, position));
}

void SP_CALLCONV Playlist::tracksRemovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new TracksRemovedEvent(tracks, numTracks));
}

void SP_CALLCONV Playlist::tracksMovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, int newPosition, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new TracksMovedEvent(tracks, numTracks, newPosition));
}

void SP_CALLCONV Playlist::playlistRenamedCallback(sp_playlist* playlist, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new QEvent(QEvent::Type(QEvent::User + 3)));
}

void SP_CALLCONV Playlist::playlistStateChangedCallback(sp_playlist* playlist, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new QEvent(QEvent::Type(QEvent::User + 4)));
}

void SP_CALLCONV Playlist::playlistUpdateInProgressCallback(sp_playlist* playlist, bool done, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new QEvent(QEvent::Type(QEvent::User + 5)));
}

void SP_CALLCONV Playlist::playlistMetadataUpdatedCallback(sp_playlist* playlist, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new QEvent(QEvent::Type(QEvent::User + 6)));
}

void SP_CALLCONV Playlist::trackCreatedChangedCallback(sp_playlist* playlist, int position, sp_user* user, int when, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new TrackCreatedChangedEvent(position, user, when));
}

void SP_CALLCONV Playlist::trackSeenChangedCallback(sp_playlist* playlist, int position, bool seen, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new TrackSeenChangedEvent(position, seen));
}

void SP_CALLCONV Playlist::descriptionChangedCallback(sp_playlist* playlist, const char* description, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new DescriptionChangedEvent(description));
}

//TODO: Get ID from byte yada
void SP_CALLCONV Playlist::imageChangedCallback(sp_playlist* playlist, const byte* image, void*)
{
    Q_UNUSED(playlist)
    Q_UNUSED(image)
}

void SP_CALLCONV Playlist::trackMessageChangedCallback(sp_playlist* playlist, int position, const char* message, void*)
{
    Q_UNUSED(playlist)
    Q_UNUSED(position)
    Q_UNUSED(message)
}

void SP_CALLCONV Playlist::subscribersChangedCallback(sp_playlist* playlist, void*)
{
    QCoreApplication::postEvent(QtSpotify::Playlist::playlistObjects.value(playlist),
                                new QEvent(QEvent::Type(QEvent::User + 12)));
}

}
