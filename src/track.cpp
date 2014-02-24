#include <QtSpotify/Core/track.h>
#include <QtSpotify/Spotify>
#include <QtSpotify/Artist>
#include <QtSpotify/Album>
#include <QtSpotify/Core/playlist.h>
#include <QtSpotify/Core/deleters.h>
#include <QtCore/QDebug>

namespace QtSpotify {

Track::Track(sp_track* track, std::shared_ptr<Playlist> playlist) :
    QObject(nullptr),
    m_name(""),
    m_album(nullptr),
    m_creator(nullptr),
    m_starred(false),
    m_seen(true),
    m_duration(0),
    m_popularity(0),
    m_disc(0),
    m_discIndex(0),
    m_offlineStatus(TrackOfflineStatus::No)
    //m_spTrack(nullptr)
{
    if(playlist != nullptr) {
        m_playlist = std::weak_ptr<Playlist>(playlist);
    }

    sp_track_add_ref(track);
    m_spTrack = std::shared_ptr<sp_track>(track, deleteTrack);

    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Track::onMetadataUpdated);
}

Track::~Track()
{

}

bool Track::loaded() const
{
    return sp_track_is_loaded(m_spTrack.get());
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

TrackOfflineStatus Track::offlineStatus() const
{
    return m_offlineStatus;
}

void Track::onMetadataUpdated()
{
    bool updated = false;

    qDebug() << "Track loaded status: " << loaded();

    bool starred = sp_track_is_starred(Spotify::instance().session().get(), m_spTrack.get());
    QString name = sp_track_name(m_spTrack.get());
    qint32 duration = sp_track_duration(m_spTrack.get());
    qint32 popularity = sp_track_popularity(m_spTrack.get());
    qint32 disc = sp_track_disc(m_spTrack.get());
    qint32 discIndex = sp_track_index(m_spTrack.get());

    qDebug() << "Track name: " << name;

    if(m_artists.isEmpty()) {
        qint32 numArtists = sp_track_num_artists(m_spTrack.get());
        for(qint32 i=0 ; i<numArtists ; ++i) {
            m_artists.append(std::make_shared<Artist>(sp_track_artist(m_spTrack.get(), i)));
        }

        updated = true;
    }

    if(m_album == nullptr) {
        m_album = std::make_shared<Album>(sp_track_album(m_spTrack.get()));
        updated = true;
    }

    updated |= exchange(m_starred, starred);
    updated |= exchange(m_name, name);
    updated |= exchange(m_duration, duration);
    updated |= exchange(m_popularity, popularity);
    updated |= exchange(m_disc, disc);
    updated |= exchange(m_discIndex, discIndex);

    if(updated) {
        emit trackDataChanged();
    }
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
