#include <QtSpotify/Core/track.h>
#include <QtSpotify/Spotify>
#include <QtSpotify/Core/deleters.h>

namespace QtSpotify {

Track::Track(sp_track* track, std::shared_ptr<Playlist> playlist)
{
    if(playlist != nullptr) {
        m_playlist = std::weak_ptr<Playlist>(playlist);
    }

    m_spTrack = std::shared_ptr<sp_track>(track, deleteTrack);

    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Track::onMetadataUpdated);
}

Track::~Track()
{

}

QQmlListProperty<Artist> Track::artists()
{
    return QQmlListProperty<Artist>(this, nullptr, &Track::artistsCountFunction, &Track::artistsAtFunction);
}

Album* Track::album() const
{
    return m_album.get();
}

User* Track::creator() const
{
    return m_creator.get();
}

QString Track::name() const
{
    return m_name;
}

bool Track::starred() const
{
    return m_starred;
}

bool Track::seen() const
{
    return m_seen;
}

qint32 Track::duration() const
{
    return m_duration;
}

qint32 Track::popularity() const
{
    return m_popularity;
}

qint32 Track::disc() const
{
    return m_disc;
}

qint32 Track::discIndex() const
{
    return m_discIndex;
}

void Track::onMetadataUpdated()
{

}

qint32 Track::artistsCountFunction(QQmlListProperty<Artist>* list)
{
    auto trackObject = qobject_cast<Track*>(list->object);

    if(trackObject != nullptr) {
        return trackObject->m_artists.count();
    }

    return 0;
}

Artist* Track::artistsAtFunction(QQmlListProperty<Artist>* list, qint32 index)
{
    auto trackObject = qobject_cast<Track*>(list->object);

    if(trackObject != nullptr) {
        return trackObject->m_artists.at(index).get();
    }

    return nullptr;
}

}
