#include <QtSpotify/spotifytrack.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifyalbum.h>
#include <QtSpotify/spotifyartist.h>

SpotifyTrack::SpotifyTrack(sp_track *track) :
    QObject(nullptr),
    m_offlineStatus(TrackOfflineStatus::No),
    m_starred(false),
    m_album(nullptr),
    m_available(false),
    m_loaded(false),
    m_name(""),
    m_duration(0),
    m_popularity(0),
    m_disc(-1),
    m_discIndex(-1)
{
    sp_track_add_ref(track);
    m_spTrack = track;

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyTrack::updateData);
    updateData();
}

SpotifyTrack::~SpotifyTrack()
{
    sp_track_release(m_spTrack);
    qDeleteAll(m_artists);
    delete m_album;
}

bool SpotifyTrack::available() const
{
    return m_available;
}

bool SpotifyTrack::loaded() const
{
    return sp_track_is_loaded(m_spTrack);
}

TrackOfflineStatus SpotifyTrack::offlineStatus() const
{
    return m_offlineStatus;
}

bool SpotifyTrack::starred() const
{
    return m_starred;
}

void SpotifyTrack::setStarred(bool star)
{
    sp_track_set_starred(SpotifySession::instance()->native(), const_cast<sp_track* const*>(&m_spTrack), 1, star);
}

QList<SpotifyArtist*> SpotifyTrack::artists() const
{
    return m_artists;
}

SpotifyAlbum* SpotifyTrack::album() const
{
    return m_album;
}

QString SpotifyTrack::name() const
{
    return m_name;
}

qint32 SpotifyTrack::duration() const
{
    return m_duration;
}

qint32 SpotifyTrack::popularity() const
{
    return m_popularity;
}

qint32 SpotifyTrack::disc() const
{
    return m_disc;
}

qint32 SpotifyTrack::discIndex() const
{
    return m_discIndex;
}

sp_track* SpotifyTrack::native() const
{
    return m_spTrack;
}

void SpotifyTrack::updateData()
{
    bool updated = false;

    bool loaded = sp_track_is_loaded(m_spTrack);
    if(m_loaded != loaded) {
        m_loaded = loaded;
        emit loadedChanged(m_loaded);
        updated = true;
    }

    bool available = sp_track_get_availability(SpotifySession::instance()->native(), m_spTrack) == SP_TRACK_AVAILABILITY_AVAILABLE;
    if(m_available != available) {
        m_available = available;
        emit availabilityChanged(m_available);
        updated = true;
    }

    QString name = QString::fromUtf8(sp_track_name(m_spTrack));
    qint32 popularity = sp_track_popularity(m_spTrack);
    qint32 duration = sp_track_duration(m_spTrack);
    qint32 disc = sp_track_disc(m_spTrack);
    qint32 discIndex = sp_track_index(m_spTrack);

    if(m_artists.isEmpty()) {
        qint32 numArtists = sp_track_num_artists(m_spTrack);
        for(qint32 i=0 ; i<numArtists ; ++i) {
            m_artists.append(new SpotifyArtist(sp_track_artist(m_spTrack, i)));
        }
        updated = true;
    }

    if(m_name != name) {
        m_name = name;
        updated = true;
    }

    if(m_popularity != popularity) {
        m_popularity = popularity;
        updated = true;
    }

    if(m_duration != duration) {
        m_duration = duration;
        updated = true;
    }

    if(m_disc != disc) {
        m_disc = disc;
        updated = true;
    }

    if(m_discIndex = discIndex) {
        m_discIndex = discIndex;
        updated = true;
    }

    if(m_album == nullptr){
        SpotifyAlbum* album = new SpotifyAlbum(sp_track_album(m_spTrack));
        m_album = album;
        updated = true;
    }

    if(updated) {
        emit trackDataChanged();
    }
}
