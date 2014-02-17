#include <QtSpotify/PlaylistContainer>

PlaylistContainer::PlaylistContainer(sp_playlistcontainer *pc) :
    QObject(nullptr)
{

}

PlaylistContainer::~PlaylistContainer()
{

}

sp_playlistcontainer* PlaylistContainer::native() const
{
    return m_spContainer;
}

void PlaylistContainer::add(Playlist* playlist)
{

}

void PlaylistContainer::add(QList<Playlist*> playlist)
{

}

void PlaylistContainer::remove(Playlist* playlist)
{

}

void PlaylistContainer::move(Playlist* playlist, qint32 newPosition)
{

}
