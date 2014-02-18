#include <QtSpotify/Search>
#include <libspotify/api.h>

#include <QtSpotify/Track>
#include <QtSpotify/Artist>
#include <QtSpotify/Album>
#include <QtSpotify/Session>

#include <QtSpotify/Core/searchcallbacks.h>

QHash<sp_search*, Search*> Search::searchObjects = QHash<sp_search*, Search*>();

Search::Search() :
    QObject(nullptr),
    m_query(""),
    m_albumsLimit(10),
    m_artistsLimit(10),
    m_tracksLimit(50),
    m_tracksOffset(0),
    m_artistsOffset(0),
    m_albumsOffset(0)
{

}

Search::~Search()
{
    qDeleteAll(m_tracksResult);
    qDeleteAll(m_artistsResult);
    qDeleteAll(m_albumsResult);

    m_tracksResult.clear();
}

QString Search::query() const
{
    return m_query;
}

void Search::setQuery(const QString& query)
{
    if(exchange(m_query, query)) {
        emit queryChanged(m_query);
    }
}

QList<Track*> Search::trackResults() const
{
    return m_tracksResult;
}

QList<Artist*> Search::artistsResult() const
{
    return m_artistsResult;
}

QList<Album*> Search::albumsResult() const
{
    return m_albumsResult;
}

void Search::search()
{
    m_spSearch = sp_search_create(Session::instance()->native(), m_query.toUtf8().constData(),
                     m_tracksOffset, m_tracksLimit,
                     m_albumsOffset, m_albumsLimit,
                     m_artistsOffset, m_artistsLimit,
                     0, 0, SP_SEARCH_STANDARD, searchCompletedCallback, nullptr);

    searchObjects.insert(m_spSearch, this);
}

bool Search::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        loadResults();
        searchObjects.remove(m_spSearch);
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void Search::loadResults()
{
    qint32 numTracks = sp_search_num_tracks(m_spSearch);
    qint32 numArtists = sp_search_num_artists(m_spSearch);
    qint32 numAlbums = sp_search_num_albums(m_spSearch);

    for(qint32 i=0 ; i<numTracks ; ++i) {
        Track* track = new Track(sp_search_track(m_spSearch, i));
        if(track != nullptr) {
            m_tracksResult.append(track);
        }
    }

    for(qint32 i=0 ; i<numArtists ; ++i) {
        Artist* artist = new Artist(sp_search_artist(m_spSearch, i));
        if(artist != nullptr) {
            m_artistsResult.append(artist);
        }
    }

    for(qint32 i=0 ; i<numAlbums ; ++i) {
        Album* album = new Album(sp_search_album(m_spSearch, i));
        if(album != nullptr) {
            m_albumsResult.append(album);
        }
    }
}
