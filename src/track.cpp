#include <QtSpotify/Track>
#include <QtSpotify/Session>
#include <QtSpotify/Playlist>
#include <QtSpotify/User>
#include <QtSpotify/Album>
#include <QtSpotify/Artist>

Track::Track(sp_track *track, Playlist* playlist) :
    QObject(nullptr),
    m_playlist(playlist),
    m_name(""),
    m_starred(false),
    m_album(nullptr),
    m_duration(0),
    m_popularity(0),
    m_disc(0),
    m_discIndex(0),
    m_offlineStatus(TrackOfflineStatus::No),
    m_spTrack(track)
{
    sp_track_add_ref(track);

    if(playlist != nullptr) {
        sp_playlist_add_ref(playlist->native());
    }

    connect(Session::instance(), &Session::metadataUpdated, this, &Track::onMetadataUpdated);
    onMetadataUpdated();
}

Track::~Track()
{
    qDeleteAll(m_artists);
    m_artists.clear();

    delete m_album;
    delete m_creator;
    delete m_playlist;

    sp_playlist_release(m_playlist->native());
    sp_track_release(m_spTrack);
}

bool Track::loaded() const
{
    return sp_track_is_loaded(m_spTrack);
}

bool Track::available() const
{
    return m_available && (!Session::instance()->offlineMode() || availableOffline());
}

bool Track::availableOffline() const
{
    return m_offlineStatus == TrackOfflineStatus::Done || m_offlineStatus == TrackOfflineStatus::DoneResync;
}

QString Track::name() const
{
    return m_name;
}

bool Track::starred() const
{
    return m_starred;
}

void Track::setStarred(bool starred)
{
    sp_track_set_starred(Session::instance()->native(), const_cast<sp_track* const*>(&m_spTrack), 1, starred);
    emit starredChanged(starred);
}

QList<Artist*> Track::artists() const
{
    return m_artists;
}

Album* Track::album() const
{
    return m_album;
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

User* Track::creator() const
{
    return m_creator;
}

QDateTime Track::creationTime() const
{
    return m_creationTime;
}

sp_track* Track::native() const
{
    return m_spTrack;
}

void Track::onMetadataUpdated()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_track_name(m_spTrack));
    bool starred = sp_track_is_starred(Session::instance()->native(), m_spTrack);
    bool available = sp_track_get_availability(Session::instance()->native(), m_spTrack) == SP_TRACK_AVAILABILITY_AVAILABLE;
    qint32 duration = sp_track_duration(m_spTrack);
    qint32 popularity = sp_track_popularity(m_spTrack);
    qint32 disc = sp_track_disc(m_spTrack);
    qint32 discIndex = sp_track_index(m_spTrack);
    TrackOfflineStatus offlineStatus = TrackOfflineStatus(sp_track_offline_get_status(m_spTrack));

    //If the track belongs to a playlist, it has a creator and a creation time
    if(m_playlist != nullptr && m_creator == nullptr) {
        qint32 trackIndex = m_playlist->tracks().indexOf(this);
        qint32 creationTime = sp_playlist_track_create_time(m_playlist->native(), trackIndex);

        User* creator = new User(sp_playlist_track_creator(m_playlist->native(), trackIndex));
        if(creator != nullptr) {
            m_creator = creator;
            updated = true;
        }

        QDateTime time = QDateTime::fromTime_t(creationTime);
        updated |= exchange(m_creationTime, time);
    }

    updated |= exchange(m_name, name);
    updated |= exchange(m_starred, starred);
    updated |= exchange(m_available, available);
    updated |= exchange(m_duration, duration);
    updated |= exchange(m_popularity, popularity);
    updated |= exchange(m_disc, disc);
    updated |= exchange(m_discIndex, discIndex);

    if(m_offlineStatus != offlineStatus) {
        m_offlineStatus = offlineStatus;
        emit offlineStatusChanged(m_offlineStatus);
        updated = true;
    }

    if(updated) {
        emit metadataChanged();
    }

}
