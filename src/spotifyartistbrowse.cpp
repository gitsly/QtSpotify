#include <QtSpotify/spotifyartistbrowse.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/artistbrowsecallbacks.h>

#include <QtSpotify/spotifyalbum.h>
#include <QtSpotify/spotifytrack.h>
#include <QtSpotify/spotifyartist.h>

QHash<sp_artistbrowse*, SpotifyArtistBrowse*> SpotifyArtistBrowse::artistBrowseObjects = QHash<sp_artistbrowse*, SpotifyArtistBrowse*>();

SpotifyArtistBrowse::SpotifyArtistBrowse(sp_artist* artist) :
    QObject(nullptr)
{
    m_artist = new SpotifyArtist(artist);

    m_spArtistBrowse = sp_artistbrowse_create(SpotifySession::instance()->native(), m_artist->native(), ArtistBrowseType::Full, artistBrowseCompleteCallback, nullptr);
    artistBrowseObjects.insert(m_spArtistBrowse, this);

    sp_artistbrowse_add_ref(m_spArtistBrowse);
}

SpotifyArtistBrowse::~SpotifyArtistBrowse()
{
    delete m_artist;
    sp_artistbrowse_release(m_spArtistBrowse);
}

SpotifyArtist* SpotifyArtistBrowse::artist() const
{
    return m_artist;
}

QList<SpotifyTrack*> SpotifyArtistBrowse::tracks() const
{
    return m_tracks;
}

QList<SpotifyTrack*> SpotifyArtistBrowse::tophits() const
{
    return m_tophitTracks;
}

QList<SpotifyAlbum*> SpotifyArtistBrowse::albums() const
{
    return m_albums;
}

QList<SpotifyArtist*> SpotifyArtistBrowse::similarArtists() const
{
    return m_similarArtists;
}

QString SpotifyArtistBrowse::biography() const
{
    return m_biography;
}

qint32 SpotifyArtistBrowse::serveTime() const
{
    return m_serveTime;
}

bool SpotifyArtistBrowse::event(QEvent* e)
{
    if(event->type() == QEvent::User) {
        //Artistbrowse complete
        updateData();
        emit completed();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void SpotifyArtistBrowse::updateData()
{
    bool updated = false;

    QString biography = sp_artistbrowse_biography(m_spArtistBrowse);
    qint32 serveTime = sp_artistbrowse_backend_request_duration(m_spArtistBrowse);

    updated |= exchange(m_biography, biography);
    updated |= exchange(m_serveTime, serveTime);

    if(m_tracks.isEmpty()) {

        qint32 numTracks = sp_artistbrowse_num_tracks(m_spArtistBrowse);

        for(qint32 i=0 ; i<numTracks ; ++i) {
            sp_track* track = sp_artistbrowse_track(m_spArtistBrowse, i);
            if(track != nullptr) {
                m_tracks.append(new SpotifyTrack(track));
            }
        }

        updated = true;
    }

    if(m_tophitTracks.isEmpty()) {

        qint32 numTophits = sp_artistbrowse_num_tophit_tracks(m_spArtistBrowse);

        for(qint32 i=0 ; i<numTophits ; ++i) {
            sp_track* track = sp_artistbrowse_tophit_track(m_spArtistBrowse, i);
            if(track != nullptr) {
                m_tophitTracks.append(new SpotifyTrack(track));
            }
        }

        updated = true;
    }

    if(m_albums.isEmpty()) {

        qint32 numAlbums = sp_artistbrowse_num_albums(m_spArtistBrowse);

        for(qint32 i=0 ; i<numAlbums ; ++i) {
            sp_album* album = sp_artistbrowse_album(m_spArtistBrowse, i);
            if(album != nullptr) {
                m_albums.append(new SpotifyAlbum(album));
            }
        }

        updated = true;
    }

    if(m_similarArtists.isEmpty()) {

        qint32 numSimilar = sp_artistbrowse_num_similar_artists(m_spArtistBrowse);

        for(qint32 i=0 ; i<numSimilar ; ++i) {
            sp_artist* artist = sp_artistbrowse_num_similar_artists(m_spArtistBrowse);
            if(artist != nullptr) {
                m_similarArtists.append(new SpotifyArtist(artist));
            }
        }

        updated = true;
    }

    return updated;

}
