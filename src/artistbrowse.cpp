#include <QtSpotify/ArtistBrowse>
#include <QtSpotify/Session>
#include <QtSpotify/Core/artistbrowsecallbacks.h>

QHash<sp_artistbrowse*, ArtistBrowse*> ArtistBrowse::artistBrowseObjects = QHash<sp_artistbrowse*, ArtistBrowse*>();

ArtistBrowse::ArtistBrowse(sp_artist* artist)
{
    m_spArtistBrowse = sp_artistbrowse_create(Session::instance()->native(), artist, SP_ARTISTBROWSE_FULL, artistBrowseCompleteCallback, nullptr);
    artistBrowseObjects.insert(m_spArtistBrowse, this);
}

ArtistBrowse::~ArtistBrowse()
{
    clearData();
    sp_artistbrowse_release(m_spArtistBrowse);
}

Artist* ArtistBrowse::artist() const
{
    return m_artist;
}

QList<Artist*> ArtistBrowse::similarArtists() const
{
    return m_similarArtists;
}

QList<Track*> ArtistBrowse::tracks() const
{
    return m_tracks;
}

QList<Album*> ArtistBrowse::albums() const
{
    return m_albums;
}

QString ArtistBrowse::biography() const
{
    return m_biography;
}

bool ArtistBrowse::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        loadResults();
        emit completed();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void ArtistBrowse::clearData()
{
    delete m_artist;

    qDeleteAll(m_similarArtists);
    m_similarArtists.clear();

    qDeleteAll(m_tracks);
    m_tracks.clear();

    qDeleteAll(m_albums);
    m_albums.clear();
}

void ArtistBrowse::loadResults()
{
    clearData();

    m_artist = new Artist(sp_artistbrowse_artist(m_spArtistBrowse));

    qint32 numSimilar = sp_artistbrowse_num_similar_artists(m_spArtistBrowse);
    for(qint32 i=0 ; i<numSimilar ; ++i) {
        m_similarArtists.append(new Artist(sp_artistbrowse_similar_artist(m_spArtistBrowse, i)));
    }

    qint32 numTracks = sp_artistbrowse_num_tracks(m_spArtistBrowse);
    for(qint32 i=0 ; i<numTracks ; ++i) {
        m_tracks.append(new Track(sp_artistbrowse_track(m_spArtistBrowse, i)));
    }

    qint32 numAlbums = sp_artistbrowse_num_albums(m_spArtistBrowse);
    for(qint32 i=0 ; i<numAlbums ; ++i) {
        m_albums.append(new Album(sp_artistbrowse_album(m_spArtistBrowse, i)));
    }

    m_biography = QString::fromUtf8(sp_artistbrowse_biography(m_spArtistBrowse));
}
