#include <QtSpotify/Core/playlistcontainer.h>
#include <QtSpotify/Core/spotify.h>
#include <QtSpotify/Core/deleters.h>
#include <QtSpotify/Core/playlistcontainerevents.h>

#include <QtSpotify/Core/user.h>
#include <QtSpotify/Core/playlist.h>


namespace QtSpotify {

QHash<sp_playlistcontainer*, PlaylistContainer*> PlaylistContainer::containerObjects = QHash<sp_playlistcontainer*, PlaylistContainer*>();

PlaylistContainer::PlaylistContainer(sp_playlistcontainer* container) :
    QObject(nullptr),
    m_spContainer(nullptr),
    m_callbacks(nullptr)
{
    sp_playlistcontainer_add_ref(container);
    m_spContainer = std::shared_ptr<sp_playlistcontainer>(container, deletePlaylistContainer);
    containerObjects.insert(m_spContainer.get(), this);

    m_callbacks = std::shared_ptr<sp_playlistcontainer_callbacks>(new sp_playlistcontainer_callbacks());
    m_callbacks->playlist_added = &PlaylistContainer::playlistAddedCallback;
    m_callbacks->playlist_removed = &PlaylistContainer::playlistRemovedCallback;
    m_callbacks->playlist_moved = &PlaylistContainer::playlistMovedCallback;
    m_callbacks->container_loaded = &PlaylistContainer::containerLoadedCallback;

    sp_playlistcontainer_add_callbacks(m_spContainer.get(), m_callbacks.get(), nullptr);

    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &PlaylistContainer::onMetadataUpdated);
}

PlaylistContainer::~PlaylistContainer()
{

}

QQmlListProperty<Playlist> PlaylistContainer::playlists()
{
    return QQmlListProperty<Playlist>(this, nullptr, &PlaylistContainer::playlistsCountFunction, &PlaylistContainer::playlistsAtFunction);
}

bool PlaylistContainer::event(QEvent* e)
{
    switch(e->type()) {
        case QEvent::User:
        {
            //Playlist added event
            PlaylistAddedEvent* event = static_cast<PlaylistAddedEvent*>(e);
            onPlaylistAdded(event);
            e->accept();
            return true;
        }
        case QEvent::User + 1:
        {
            //Playlist removed event
            PlaylistRemovedEvent* event = static_cast<PlaylistRemovedEvent*>(e);
            onPlaylistRemoved(event);
            e->accept();
            return true;
        }
        case QEvent::User + 2:
        {
            //Playlist moved event
            PlaylistMovedEvent* event = static_cast<PlaylistMovedEvent*>(e);
            onPlaylistMoved(event);
            e->accept();
            return true;
        }
        case QEvent::User + 3:
            //PlaylistContainer loaded event
            onMetadataUpdated();
            e->accept();
            return true;
        default:
            return QObject::event(e);
    }
}

void PlaylistContainer::onMetadataUpdated()
{
    if(m_playlists.isEmpty()) {
        qint32 numPlaylists = sp_playlistcontainer_num_playlists(m_spContainer.get());

        for(qint32 i=0 ; i<numPlaylists ; ++i) {
            m_playlists.append(std::make_shared<Playlist>(sp_playlistcontainer_playlist(m_spContainer.get(), i)));
        }
    }

    emit playlistContainerDataChanged();
}

void PlaylistContainer::onPlaylistAdded(PlaylistAddedEvent* event)
{
    m_playlists.insert(event->position(), event->playlist());
    emit playlistsChanged();
}

void PlaylistContainer::onPlaylistRemoved(PlaylistRemovedEvent* event)
{
    if(event->position() < 0 || event->position() > m_playlists.count() - 1 || m_playlists.isEmpty()) {
        return;
    }

    auto playlist = m_playlists.takeAt(event->position());
    playlist.reset();

    emit playlistsChanged();
}

void PlaylistContainer::onPlaylistMoved(PlaylistMovedEvent* event)
{
    if(event->position() < 0 || event->position() > m_playlists.count() - 1 || m_playlists.isEmpty()) {
        return;
    }

    m_playlists.move(event->position(), event->newPosition());
    emit playlistsChanged();
}

void PlaylistContainer::playlistAddedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, void*)
{
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(container), new PlaylistAddedEvent(playlist, position));
}

void PlaylistContainer::playlistRemovedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, void*)
{
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(container), new PlaylistRemovedEvent(playlist, position));
}

void PlaylistContainer::playlistMovedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, int newPosition, void*)
{
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(container), new PlaylistMovedEvent(playlist, position, newPosition));
}

void PlaylistContainer::containerLoadedCallback(sp_playlistcontainer* container, void*)
{
    QCoreApplication::postEvent(PlaylistContainer::containerObjects.value(container), new QEvent(QEvent::Type(QEvent::User + 3)));
}

qint32 PlaylistContainer::playlistsCountFunction(QQmlListProperty<Playlist>* list)
{
    auto containerObject = qobject_cast<PlaylistContainer*>(list->object);

    if(containerObject != nullptr) {
        return containerObject->m_playlists.count();
    }

    return 0;
}

Playlist* PlaylistContainer::playlistsAtFunction(QQmlListProperty<Playlist>* list, qint32 index)
{
    auto containerObject = qobject_cast<PlaylistContainer*>(list->object);

    if(containerObject != nullptr) {
        return containerObject->m_playlists.at(index).get();
    }

    return nullptr;
}

}
