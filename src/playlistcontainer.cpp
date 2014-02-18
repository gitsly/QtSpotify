#include <QtSpotify/PlaylistContainer>
#include <QtSpotify/Playlist>
#include <QtSpotify/Session>

#include <QtSpotify/Core/playlistcontainercallbacks.h>

QHash<sp_playlistcontainer*, PlaylistContainer*> PlaylistContainer::containerObjects = QHash<sp_playlistcontainer*, PlaylistContainer*>();

PlaylistContainer::PlaylistContainer(sp_playlistcontainer* pc) :
    QObject(nullptr)
{
    sp_playlistcontainer_add_ref(pc);
    m_spContainer = pc;

    containerObjects.insert(m_spContainer, this);

    m_callbacks = new sp_playlistcontainer_callbacks();
    m_callbacks->container_loaded = containerLoadedCallback;
    m_callbacks->playlist_added = playlistAddedCallback;
    m_callbacks->playlist_moved = playlistMovedCallback;
    m_callbacks->playlist_removed = playlistRemovedCallback;
    sp_playlistcontainer_add_callbacks(m_spContainer, m_callbacks, nullptr);

    connect(Session::instance(), &Session::metadataUpdated, this, &PlaylistContainer::onMetadataUpdated);
    onMetadataUpdated();
}

PlaylistContainer::~PlaylistContainer()
{
    sp_playlistcontainer_release(m_spContainer);
}

QList<Playlist*> PlaylistContainer::playlists() const
{
    return m_playlists;
}

sp_playlistcontainer* PlaylistContainer::native() const
{
    return m_spContainer;
}

void PlaylistContainer::add(Playlist* playlist)
{
    sp_link* link = sp_link_create_from_playlist(playlist->native());

    m_playlists.append(playlist);
    sp_playlistcontainer_add_playlist(m_spContainer, link);

    sp_link_release(link);
}

void PlaylistContainer::remove(Playlist* playlist)
{
    qint32 playlistIndex = m_playlists.indexOf(playlist);

    if(playlistIndex > -1) {
        m_playlists.removeAt(playlistIndex);
        sp_playlistcontainer_remove_playlist(m_spContainer, playlistIndex);
    }
}

void PlaylistContainer::move(Playlist* playlist, qint32 newPosition)
{
    qint32 playlistIndex = m_playlists.indexOf(playlist);

    if(playlistIndex > -1) {
        m_playlists.move(playlistIndex, newPosition);
        sp_playlistcontainer_move_playlist(m_spContainer, playlistIndex, newPosition, false);
    }
}

void PlaylistContainer::create(const QString& name)
{
    m_playlists.append(new Playlist(sp_playlistcontainer_add_new_playlist(m_spContainer, name.toUtf8().constData())));
}

bool PlaylistContainer::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        //Playlist added event
        PlaylistAddedEvent* ev = static_cast<PlaylistAddedEvent*>(e);
        qint32 plIndex = m_playlists.indexOf(ev->playlist());
        if(plIndex == ev->position()) {
            //Playlist was added successfully
        }
        else {
            //Playlist wasn't added correctly
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 1) {
        //Playlist removed event
        PlaylistRemovedEvent* ev = static_cast<PlaylistRemovedEvent*>(e);
        qint32 plIndex = m_playlists.indexOf(ev->playlist());
        if(plIndex == -1) {
            //Playlist was removed successfully
        }
        else {
            //Playlist is still in the container
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 2) {
        //Playlist moved event
        PlaylistMovedEvent* ev = static_cast<PlaylistMovedEvent*>(e);
        qint32 plIndex = m_playlists.indexOf(ev->playlist());
        if(plIndex == ev->newPosition()) {
            //Playlist was moved correctly
        }
        else {
            //Playlist was moved incorrectly
        }
        e->accept();
        return true;
    }
    else if(e->type() == QEvent::User + 3) {
        //Container loaded
        onMetadataUpdated();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void PlaylistContainer::onMetadataUpdated()
{
    bool updated = false;

    if(m_playlists.isEmpty()) {

        qint32 numPlaylists = sp_playlistcontainer_num_playlists(m_spContainer);

        for(qint32 i=0 ; i<numPlaylists ; ++i) {
            sp_playlist* pl = sp_playlistcontainer_playlist(m_spContainer, i);
            if(pl != nullptr) {
                m_playlists.append(new Playlist(pl));
            }
        }
    }
}
