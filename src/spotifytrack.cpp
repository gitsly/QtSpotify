#include <QtSpotify/spotifytrack.h>
#include <QtSpotify/spotifysession.h>

SpotifyTrack::SpotifyTrack(sp_track *track) :
    QObject(nullptr)
{
    sp_track_add_ref(track);
    m_spTrack = track;

    updateData();
}

SpotifyTrack::~SpotifyTrack()
{
    sp_track_release(track);
}

bool SpotifyTrack::available() const
{
    return (sp_track_get_availability(SpotifySession::instance()->native(), m_spTrack) == SP_TRACK_AVAILABILITY_AVAILABLE);
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
    sp_track_set_starred(SpotifySession::instance()->native(), const_cast<sp_track* cont*>(&m_spTrack), 1, star);
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

}
