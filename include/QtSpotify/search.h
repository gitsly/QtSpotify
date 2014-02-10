#ifndef QTSPOTIFY_SEARCH_H_
#define QTSPOTIFY_SEARCH_H_

#include <QtSpotify/qtspotify_global.h>
#include <QString>
#include <QList>

struct sp_link;
struct sp_search;
class SpotifyArtist;
class SpotifyAlbum;

class QTSPOTIFYSHARED_EXPORT SpotifySearch : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QList<SpotifyArtist*> artists READ artists NOTIFY resultsChanged)
    Q_PROPERTY(QList<SpotifyAlbum*> albums READ albums NOTIFY resultsChanged)

public:

    SpotifySearch();
    ~SpotifySearch();

    /*!
     * \brief The query for the search
     * \return The query string
     * \see setQuery()
     */
    QString query() const;

    /*!
     * \brief Set the query for the search
     * \param query The desired query
     */
    void setQuery(const QString& query);

    /*!
     * \brief The artists result of the search
     * \return List of artists
     */
    QList<SpotifyArtist*> artists() const;

    /*!
     * \brief The albums result of the search
     * \return List of albums
     */
    QList<SpotifyAlbum*> albums() const;

    /*!
     * \brief Set the upper limit of tracks returned by the search
     * \param limit The limit
     */
    void setTracksLimit(qint32 limit);

    /*!
     * \brief Set the upper limit of artists returned by the search
     * \param limit The limit
     */
    void setArtistsLimit(qint32 limit);

    /*!
     * \brief Set the upper limit of albums returned by the search
     * \param limit The limit
     */
    void setAlbumsLimit(qint32 limit);

    /*!
     * \brief Indicates whether or not a search is in progress
     * \return True if a search is in progress, false otherwise
     */
    bool busy() const;

    /*!
     * \brief Starts the search
     * \see busy(), resultsChanged()
     */
    void search();

    /*!
     * \brief Continue searching for tracks with the same query
     * \note If the query has changed between search() and more(), a new search will be created. Otherwise the list of tracks will be populated with more tracks.
     */
    void more();

signals:

    void resultsChanged();

private:

    sp_search* m_spSearch;

    QString m_query;

    qint32 m_tracksLimit;
    qint32 m_artistsLimit;
    qint32 m_albumsLimit;

    qint32 m_tracksOffset;
    qint32 m_artistsOffset;
    qint32 m_albumsOffset;

    QList<SpotifyArtist*> m_artists;
    QList<SpotifyAlbum*> m_albums;

};

#endif
