#ifndef QTSPOTIFY_SPOTIFYPLAYLIST_H_
#define QTSPOTIFY_SPOTIFYPLAYLIST_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <libspotify/api.h>

struct sp_playlist;
class SpotifyTrack;
class SpotifyAlbumBrowse;

enum class PlaylistType {
    Playlist = SP_PLAYLIST_TYPE_PLAYLIST,
    StartFolder = SP_PLAYLIST_TYPE_START_FOLDER,
    EndFolder = SP_PLAYLIST_TYPE_END_FOLDER,
    Placeholder = SP_PLAYLIST_TYPE_PLACEHOLDER
};

enum class PlaylistOfflineStatus {
    No = SP_PLAYLIST_OFFLINE_STATUS_NO,
    Yes = SP_PLAYLIST_OFFLINE_STATUS_YES,
    Downloading = SP_PLAYLIST_OFFLINE_STATUS_DOWNLOADING,
    Waiting = SP_PLAYLIST_OFFLINE_STATUS_WAITING
};

class QTSPOTIFYSHARED_EXPORT SpotifyPlaylist : public QObject
{

    Q_OBJECT

    Q_ENUMS(PlaylistType PlaylistOfflineStatus)

public:

    static QHash<sp_playlist*, SpotifyPlaylist*> playlistObjects;

    SpotifyPlaylist(sp_playlist* playlist);
    ~SpotifyPlaylist();

    /*!
     * \brief Returns true if the playlist is loaded by Libspotify
     * \return True if the playlist is loaded, false otherwise
     */
    bool loaded() const;

    /*!
     * \brief Returns true if the playlist is available in offline mode
     * \return True if the playlist is available offline, otherwise false
     */
    bool availableOffline() const;

    /*!
     * \brief Returns a string representation of the name of the playlist
     * \return The playlist name
     */
    QString name() const;

    /*!
     * \brief Returns a description of the playlist
     * \return The playlist description
     */
    QString description() const;

    /*!
     * \brief Returns true if the playlist is marked as collaborative
     * \return True if the playlist is marked as collaborative, false otherwise
     */
    bool collaborative() const;

    /*!
     * \brief Returns the subscribers of the playlist
     * \return Returns a list of canonical names of the users who have subscribed to the playlist
     */
    QStringList subscribers() const;

    /*!
     * \brief Returns the current offline status of the playlist
     * \return An indication of the current offline status
     */
    PlaylistOfflineStatus offlineStatus() const;

    /*!
     * \brief Returns the type of the playlist
     * \return The playlist type
     */
    PlaylistType type() const;

    /*!
     * \brief Returns the tracks in the playlist
     * \return A list of tracks currently in the playlist
     */
    QList<SpotifyTrack*> tracks() const;

    /*!
     * \brief Returns the Libspotify native representation of the playlist
     * \return The native representation
     */
    sp_playlist* native() const;

    /*!
     * \brief Sets the name of the playlist
     * \param name The new name of the playlist
     */
    void setName(const QString& name);

    /*!
     * \brief Sets the collaborative status of the playlist, if the user has rights to do so
     * \param collaborative True if the playlist is to be collaborative, otherwise false
     */
    void setCollaborative(bool collaborative);

    /*!
     * \brief Sets the offline status of the playlist
     * \param download True if the playlist should be downloaded, otherwise false
     */
    void setAvailableOffline(bool download);

    /*!
     * \brief Adds a single track to the playlist
     * \param track The track to add
     */
    void add(SpotifyTrack* track);

    /*!
     * \brief Adds multiple tracks to the playlist
     * \param tracks The track list
     */
    void add(QList<SpotifyTrack*> tracks);

    /*!
     * \brief Removes the specified track from the playlist
     * \param track The track to remove
     */
    void remove(SpotifyTrack* track);

    /*!
     * \brief Removes multiple tracks from the playlist
     * \param tracks List of tracks to remove
     */
    void remove(QList<SpotifyTrack*> tracks);

    /*!
     * \brief Moves the specified track to the position specified
     * \param track The track to move
     * \param newPosition The position to which the track is to be moved
     */
    void move(SpotifyTrack* track, qint32 newPosition);

    /*!
     * \brief Moves multiple tracks to the position specified
     * \param tracks The tracks to move
     * \param newPosition The position to which the tracks are to be moved
     */
    void move(QList<SpotifyTrack*> tracks, qint32 newPosition);

    /*!
     * \brief Creates a new playlist and adds the track to it. The playlist will be named "ArtistName - TrackName"
     * \param track The track of which to create a playlist
     */
    static void createFromTrack(SpotifyTrack* track);

    /*!
     * \brief Creates a new playlist and adds all the album's tracks to it. The playlist will be named "ArtistName - AlbumName"
     * \param album The album of which to create a playlist
     */
    static void createFromAlbum(SpotifyAlbumBrowse* album);

private slots:

    void updateData();

private:

    QString m_name;
    QString m_description;
    QList<SpotifyTrack*> m_tracks;

    bool m_collaborative;
    QStringList m_subscribers;

    PlaylistOfflineStatus m_offlineStatus;
    PlaylistType m_type;

    sp_playlist* m_spPlaylist;
};

#endif
