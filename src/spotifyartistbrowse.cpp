#include <QtSpotify/spotifyartistbrowse.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/artistbrowsecallbacks.h>

QHash<sp_artistbrowse*, SpotifyArtistBrowse*> SpotifyArtistBrowse::artistBrowseObjects = QHash<sp_artistbrowse*, SpotifyArtistBrowse*>();

SpotifyArtistBrowse::SpotifyArtistBrowse(sp_artist* artist) :
    QObject(nullptr)
{
    sp_artist_add_ref(artist);
    m_artist = artist;

    m_spArtistBrowse = sp_artistbrowse_create(SpotifySession::instance()->native(), m_artist, ArtistBrowseType::Full, artistBrowseCompleteCallback, nullptr);
    sp_artistbrowse_add_ref(m_spArtistBrowse);
}

SpotifyArtistBrowse::~SpotifyArtistBrowse()
{
    sp_artist_release(m_artist);
    sp_artistbrowse_release(m_spArtistBrowse);
}

QList<SpotifyTrack*> SpotifyArtistBrowse::tracks() const
{

}

QList<SpotifyTrack*> SpotifyArtistBrowse::tophits() const
{

}

QList<SpotifyAlbum*> SpotifyArtistBrowse::albums() const
{

}

QStringList SpotifyArtistBrowse::similarArtists() const
{

}

QString SpotifyArtistBrowse::biography() const
{

}

qint32 SpotifyArtistBrowse::serveTime() const
{

}

bool SpotifyArtistBrowse::event(QEvent* e)
{
    if(event->type() == QEvent::User) {
        //Artistbrowse complete
        emit completed();
        updateData();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void SpotifyArtistBrowse::updateData()
{

}
