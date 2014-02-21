#include <QtSpotify/Core/playlist.h>
#include <QtSpotify/Track>
#include <QtSpotify/Spotify>
#include <QtSpotify/Core/deleters.h>

Playlist::Playlist(sp_playlist* playlist)
{
    m_spPlaylist = std::shared_ptr<sp_playlist>(playlist, deletePlaylist);

    //Spotify says something, somewhere has been updated... Better check the playlist
    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Playlist::onMetadataUpdated);

    //If any property on the playlist changes, send out a signal that the playlist as a whole has changed
    connect(this, &Playlist::tracksChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::nameChanged, this, &Playlist::playlistDataChanged);
    connect(this, &Playlist::collaborativeChanged, this, &Playlist::playlistDataChanged);
}

Playlist::~Playlist()
{

}

QString Playlist::name() const
{
    return m_name;
}

bool Playlist::collaborative() const
{
    return m_collaborative;
}

QQmlListProperty<Track> Playlist::tracks()
{
    return QQmlListProperty<Track>(this, nullptr, &Playlist::tracksAppendFunction, &Playlist::tracksCountFunction,
                                   &Playlist::tracksAtFunction, &Playlist::tracksClearFunction);
}

void Playlist::onMetadataUpdated()
{

}

void Playlist::tracksAppendFunction(QQmlListProperty<Track>* list, Track* track)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if(playlist != nullptr) {
        playlist->m_tracks.append(std::shared_ptr<Track>(track));
    }
}

Track* Playlist::tracksAtFunction(QQmlListProperty<Track>* list, int index)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if(playlist != nullptr) {
        return playlist->m_tracks.at(index).get();
    }

    return nullptr;
}

void Playlist::tracksClearFunction(QQmlListProperty<Track>* list)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if(playlist != nullptr) {
        foreach(auto track, playlist->m_tracks) {
            track.reset();
        }
        playlist->m_tracks.clear();
    }
}

qint32 Playlist::tracksCountFunction(QQmlListProperty<Track>* list)
{
    auto playlist = qobject_cast<Playlist*>(list->object);

    if(playlist != nullptr) {
        return playlist->m_tracks.count();
    }

    return 0;
}
