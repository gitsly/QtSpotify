#include <QtSpotify/Playlist>
#include <QtSpotify/Session>
#include <QtSpotify/Track>
#include <QtSpotify/User>

Playlist::Playlist(sp_playlist* playlist) :
    QObject(nullptr),
    m_name(""),
    m_owner(nullptr),
    m_collaborative(false),
    m_description(""),
    m_offlineStatus(PlaylistOfflineStatus::No)
{
    sp_playlist_add_ref(playlist);
    m_spPlaylist = playlist;

    connect(Session::instance(), &Session::metadataUpdated, this, &Playlist::onMetadataUpdated);
    onMetadataUpdated();
}

Playlist::~Playlist()
{
    qDeleteAll(m_tracks);
    m_tracks.clear();

    delete m_owner;

    qDeleteAll(m_subscribers);
    m_subscribers.clear();

    sp_playlist_release(m_spPlaylist);
}

bool Playlist::loaded() const
{
    return sp_playlist_is_loaded(m_spPlaylist);
}

QList<Track*> Playlist::tracks() const
{
    return m_tracks;
}

QString Playlist::name() const
{
    return m_name;
}

User* Playlist::owner() const
{
    return m_owner;
}

bool Playlist::collaborative() const
{
    return m_collaborative;
}

void Playlist::setCollaborative(bool collaborative)
{

}

QString Playlist::description() const
{
    return m_description;
}

QList<User*> Playlist::subscribers() const
{
    return m_subscribers;
}

PlaylistOfflineStatus Playlist::offlineStatus() const
{
    return m_offlineStatus;
}

sp_playlist* Playlist::native() const
{
    return m_spPlaylist;
}

void Playlist::add(Track* track, qint32 position)
{

}

void Playlist::remove(Track* track)
{

}

void Playlist::move(Track* track, qint32 newPosition)
{

}

void Playlist::onMetadataUpdated()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_playlist_name(m_spPlaylist));
    bool collaborative = sp_playlist_is_collaborative(m_spPlaylist);
    QString description = sp_playlist_get_description(m_spPlaylist);
    PlaylistOfflineStatus offlineStatus = PlaylistOfflineStatus(sp_playlist_get_offline_status(Session::instance()->native(), m_spPlaylist));

    if(m_tracks.isEmpty()) {

        qint32 numTracks = sp_playlist_num_tracks(m_spPlaylist);
        for(qint32 i=0 ; i<numTracks ; ++i) {
            Track* track = new Track(sp_playlist_track(this->native(), i), this);
            if(track != nullptr) {
                m_tracks.append(track);
            }
        }

        updated = true;
    }

    if(m_subscribers.isEmpty()) {

        sp_subscribers* subscribers = sp_playlist_subscribers(m_spPlaylist);

        for(quint32 i=0 ; i<subscribers->count ; ++i) {
            QString userLink = QString("%1%2").arg("spotify:user:").arg(subscribers->subscribers[i]);
            sp_link* link = sp_link_create_from_string(userLink.toUtf8().constData());
            sp_user* user = sp_link_as_user(link);
            sp_user_add_ref(user);
            sp_link_release(link);

            m_subscribers.append(new User(user));
            sp_user_release(user);
        }

        updated = true;
    }

    if(m_owner == nullptr) {
        sp_user* user = sp_playlist_owner(m_spPlaylist);
        User* owner = new User(user);

        m_owner = owner;
        updated = true;
    }

    if(updated) {
        emit metadataChanged();
    }
}
