/*! \file */

#ifndef QTSPOTIFY_SPOTIFYALBUM_H_
#define QTSPOTIFY_SPOTIFYALBUM_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <libspotify/api.h>

class SpotifyArtist;

/*!
 * \brief The AlbumType enum represents a type of an album
 */
enum class AlbumType {
    Album = SP_ALBUMTYPE_ALBUM,             /*!< A normal album */
    Single = SP_ALBUMTYPE_SINGLE,           /*!< A single */
    Compilation = SP_ALBUMTYPE_COMPILATION, /*!< A compilation album */
    Unknown = SP_ALBUMTYPE_UNKNOWN          /*!< An unknown album type */
};

/*!
 * \brief The SpotifyAlbum class wraps the Libspotify native representation
 */
class QTSPOTIFYSHARED_EXPORT SpotifyAlbum : public QObject
{

    Q_OBJECT

public:

    /*!
     * \brief SpotifyAlbum
     * \param album
     */
    SpotifyAlbum(sp_album* album);
    ~SpotifyAlbum();

    /*!
     * \brief Returns true if the album is currently available for browsing
     * \return True if the album is available, false otherwise
     */
    bool available() const;

    /*!
     * \brief Returns true if the album is loaded by Libspotify
     * \return True if the album is loaded, false otherwise
     */
    bool loaded() const;

    /*!
     * \brief Returns the artist associated with the album
     * \return The artist of the album
     */
    SpotifyArtist* artist() const;

    /*!
     * \brief Returns the string representation of the name of the album
     * \return The album name
     */
    QString name() const;

    /*!
     * \brief The ID string of the cover art for the album
     * \return The cover ID
     */
    QString coverId() const;

    /*!
     * \brief Returns the year the albums was released
     * \return An integer representing the release year of the album
     */
    qint32 year() const;

    /*!
     * \brief Returns the type of the album
     * \return The album type
     */
    AlbumType type() const;

    /*!
     * \brief Returns the Libspotify native representation of the album
     * \return The native representation
     */
    sp_album* native() const;

private slots:

    void updateData();

signals:

    /*!
     * \brief Emitted whenever the album's availability changes
     * \param available True if the album is available, false otherwise
     */
    void availabilityChanged(bool available);

    /*!
     * \brief Emitted whenever the loaded status of the album changes
     * \param loaded True if the album is loaded, false otherwise
     */
    void loadedChanged(bool loaded);

    /*!
     * \brief Emitted whenever the metadata of the album is updated
     */
    void albumDataChanged();

private:

    SpotifyArtist* m_artist;
    QString m_name;
    QString m_coverId;

    qint32 m_year;
    AlbumType m_type;

    bool m_available;
    bool m_loaded;

    sp_album* m_spAlbum;

};


#endif
