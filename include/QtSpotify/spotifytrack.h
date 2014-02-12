/*! \file */

#ifndef SPOTIFYTRACK_H
#define SPOTIFYTRACK_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>
#include <libspotify/api.h>

class SpotifyArtist;
class SpotifyAlbum;

/*!
 * \enum The TrackOfflineStatus enum represents an offline status of a track
 */
enum class TrackOfflineStatus {
    No = SP_TRACK_OFFLINE_NO,                       /*!< The track is not marked for offline */
    Waiting = SP_TRACK_OFFLINE_WAITING,             /*!< The track is waiting for download */
    Downloading = SP_TRACK_OFFLINE_DOWNLOADING,     /*!< The track is currently downloading */
    Done = SP_TRACK_OFFLINE_DONE,                   /*!< The track is downloaded and can be played */
    Error = SP_TRACK_OFFLINE_ERROR,                 /*!< There was an error during download */
    DoneExpired = SP_TRACK_OFFLINE_DONE_EXPIRED,    /*!< The track is downloaded but not playable due to expiration */
    LimitExceeded = SP_TRACK_OFFLINE_LIMIT_EXCEEDED,/*!< Waiting for download because the device has reached max number of offline tracks */
    DoneResyn = SP_TRACK_OFFLINE_DONE_RESYNC        /*!< The track is downloaded and playable but is scheduled for re-download */
};

/*!
 * \brief The SpotifyTrack class is essentially a wrapper for the native Libspotify track format
 */
class QTSPOTIFYSHARED_EXPORT SpotifyTrack : public QObject
{

    Q_ENUMS(TrackOfflineStatus)

public:

    /*!
     * \brief Constructs a new track object and tries to load all metadata
     * \param track The Libspotify track representation
     * \see availabilityChanged()
     */
    SpotifyTrack(sp_track* track);
    ~SpotifyTrack();

    /*!
     * \brief Checks if the track is currently available for playback
     * \return True if the track is available, false otherwise
     */
    bool available() const;

    /*!
     * \brief Returns the current offline status for the track
     * \return The offline status
     * \note If no metadata has been loaded, this function returns TrackOfflineStatus::No
     */
    TrackOfflineStatus offlineStatus() const;

    /*!
     * \brief Returns the current star status for the track
     * \return True if the track is starred, otherwise false
     * \see setStarred()
     */
    bool starred() const;

    /*!
     * \brief Sets or unsets the star status for the track
     * \param star True if the track is to be starred, false otherwise
     * \see starred()
     */
    void setStarred(bool star);

    /*!
     * \brief Returns a list of artists that appears in the track
     * \return A list of artists
     */
    QList<SpotifyArtist*> artists() const;

    /*!
     * \brief Returns the album the track is on
     * \return An album
     */
    SpotifyAlbum* album() const;

    /*!
     * \brief Returns a string representation of the track
     * \return A string representation of the track
     */
    QString name() const;

    /*!
     * \brief Returns the duration in milliseconds for the track
     * \return The track duration
     */
    qint32 duration() const;

    /*!
     * \brief Returns a value betwee 0 and 100 representing the popularity of the track
     * \return The popularity of the track
     */
    qint32 popularity() const;

    /*!
     * \brief Returns the index of the disc on which this track appears
     * \return The disc index
     * \note This function is only useful in an artist- or album browsing context
     */
    qint32 disc() const;

    /*!
     * \brief Returns the index this track has on its disc
     * \return The index of the track on its disc
     * \note This function is only useful in an artist- or album browsing context
     */
    qint32 discIndex() const;

    /*!
     * \brief Returns the Libspotify native representation of the track
     * \return The native representation
     */
    sp_track* native() const;

private slots:

    void updateData();

signals:

    /*!
     * \brief Emitted whenever the tracks availability changes
     * \param available True if the track is available, false otherwise
     */
    void availabilityChanged(bool available);

    /*!
     * \brief Emitted whenever the metadata of the track is updated
     */
    void trackDataChanged();

private:

    TrackOfflineStatus m_offlineStatus;
    bool m_starred;

    QList<SpotifyArtist*> m_artists;
    SpotifyAlbum* m_album;
    QString m_name;

    qint32 m_duration;
    qint32 m_popularity;

    qint32 m_disc;
    qint32 m_discIndex;

    sp_track* m_spTrack;

};

#endif // SPOTIFYTRACK_H
