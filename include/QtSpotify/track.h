#ifndef QTSPOTIFY_TRACK_H_
#define QTSPOTIFY_TRACK_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <libspotify/api.h>

struct sp_track;
class SpotifyAlbum;
class SpotifyArtist;

class QTSPOTIFYSHARED_EXPORT SpotifyTrack : public QObject
{

    Q_OBJECT

public:

    /*!
     * \brief The OfflineStatus enum gives an indication of the current offline status for tracks
     */
    enum OfflineStatus {
        No = SP_TRACK_OFFLINE_NO,                       /**< The track is not marked for offline */
        Waiting = SP_TRACK_OFFLINE_WAITING,             /**< The track is waiting for download */
        Downloading = SP_TRACK_OFFLINE_DOWNLOADING,     /**< The track is currently downloading */
        Done = SP_TRACK_OFFLINE_DONE,                   /**< The track is downloaded and can be played */
        Error = SP_TRACK_OFFLINE_ERROR,                 /**< An error was encountered while downloading */
        DoneExpired = SP_TRACK_OFFLINE_DONE_EXPIRED,    /**< The track is downloaded but not playable due to expiration */
        LimitExceeded = SP_TRACK_OFFLINE_LIMIT_EXCEEDED,/**< Maximum number of offline tracks has been reached */
        DoneResync = SP_TRACK_OFFLINE_DONE_RESYNC       /**< The track is downloaded and playable but is scheduled for re-download */
    };

    /*!
     * \brief The TrackError enum gives an indicaton of the current track status
     */
    enum TrackError {
        Ok = SP_ERROR_OK,                           /**< No error */
        Loading = SP_ERROR_IS_LOADING,              /**< The track is currently loading */
        OtherPermanent = SP_ERROR_OTHER_PERMANENT   /**< Other, permanent error */
    };

    /*!
     * \brief Constructs a new track from the specified native type
     * \param track The libspotify native track representation
     */
    explicit SpotifyTrack(sp_track* track);
    ~SpotifyTrack();

    /*!
     * \brief Returns the name of the track
     * \return Track name
     */
    const QString& name() const;

    /*!
     * \brief Returns the album on which the track appears on
     * \return The album
     */
    SpotifyAlbum* album() const;

    /*!
     * \brief Returns a list of artists that appears in the track
     * \return A list of artists
     */
    QList<SpotifyArtist*> artists() const;

    /*!
     * \brief Returns the popularity of the track
     * \return A value between 0 and 100
     */
    quint8 popularity() const;

    /*!
     * \brief Returns the star status of the track
     * \return True if the track is starred, false otherwise
     * \see setStarred()
     */
    bool starred() const;

    /*!
     * \brief Sets the current star status of the track
     * \param star Bool representing the desired star status of the track
     * \see starred()
     */
    void setStarred(bool star);

    /*!
     * \brief Returns the index number of the disc the track appears on
     * \return The disc index; a value between 1 and the total number of discs on the album
     * \note This function is only valid in an artist browse or album browse context
     * \see discIndex()
     */
    qint32 disc() const;

    /*!
     * \brief Returns the position of the track on its disc
     * \return Track position; a value between 1 and the total number of tracks on the disc
     * \note This function is only valid in an artist rowse or album browse context
     * \see disc()
     */
    qint32 discIndex() const;

    /*!
     * \brief The duration in milliseconds of the track
     * \return The duration of the track in milliseconds
     */
    qint32 duration() const;

    /*!
     * \brief Returns the availability status of the track
     * \return True if the track is available, false otherwise
     */
    bool available() const;

};

#endif
