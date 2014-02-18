#ifndef QTSPOTIFY_PLAYLISTCONTAINER_H_
#define QTSPOTIFY_PLAYLISTCONTAINER_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QEvent>
#include <libspotify/api.h>

struct sp_playlistcontainer;
struct sp_playlistcontainer_callbacks;
class Playlist;

/*!
 * \brief The PlaylistType enum indicated if a 'playlist' is really a playlist or a folder
 */
enum class PlaylistType {
    Playlist = SP_PLAYLIST_TYPE_PLAYLIST,
    StartFolder = SP_PLAYLIST_TYPE_START_FOLDER,
    EndFolder = SP_PLAYLIST_TYPE_END_FOLDER
};

class QTS_EXPORT PlaylistContainer : public QObject
{

    Q_OBJECT

public:

    static QHash<sp_playlistcontainer*, PlaylistContainer*> containerObjects;

    /*!
     * \brief Constructs a playlist container and loads all metadata
     * \param pc The playlist container
     */
    PlaylistContainer(sp_playlistcontainer* pc);
    virtual ~PlaylistContainer();

    /*!
     * \brief Get the playlists in the playlist container
     * \return A list of playlists
     */
    QList<Playlist*> playlists() const;

    /*!
     * \brief native
     * \return
     */
    sp_playlistcontainer* native() const;

    /*!
     * \brief This function adds the specified playlist to the playlist container
     * \param playlist The playlist to be added
     *
     * This function reports to libspotify that the playlist has been added, and then immediately adds it to the playlist container.
     * When libspotify in turn reports that the playlist has been added, a verification routine checks that it's truly there. This gives more responsiveness.
     */
    void add(Playlist* playlist);

    /*!
     * \brief This function removed the specified playlist from the playlist container
     * \param playlist The playlist to be removed
     *
     * This function reports to libspotify that the playlist has been removed, and then immediately removed it from the playlist container.
     * When libspotify in turn reports that the playlist has been removed, a verification routine checks that it's truly removed. This gives more responsiveness.
     */
    void remove(Playlist* playlist);

    /*!
     * \brief This function moved the specified playlist to the given position
     * \param playlist The playlist to be moved
     * \param newPosition The new position in the playlist containerObjects
     *
     * This function reports to libspotify that the playlist has been moved, and then immediately moves it to the specified location.
     * When libspotify reports that the playlist was moved, a verification routine checks that it's truly moved. This gives more responsiveness.
     */
    void move(Playlist* playlist, qint32 newPosition);

    /*!
     * \brief This function creates a new playlist with the specified name
     * \param name The name of the playlist to be created
     *
     * This function reports to libspotify that the playlist was created, and the immediately adds it to the playlist container.
     * When libspotify reports that the playlist was created, a verification routine checks that it's truly created. This gives more responsiveness.
     */
    void create(const QString& name);

protected:

    bool event(QEvent*);

private slots:

    void onMetadataUpdated();

private:

    QList<Playlist*> m_playlists;

    sp_playlistcontainer* m_spContainer;
    sp_playlistcontainer_callbacks* m_callbacks;

};
#endif
