#ifndef QTSPOTIFY_SPOTIFYPLAYLISTCONTAINER_H_
#define QTSPOTIFY_SPOTIFYPLAYLISTCONTAINER_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <libspotify/api.h>

class SpotifyPlaylist;
class SpotifyUser;

class QTSPOTIFYSHARED_EXPORT SpotifyPlaylistContainer : public QObject
{

    Q_OBJECT

public:

    SpotifyPlaylistContainer(sp_playlistcontainer* playlistContainer);
    virtual ~SpotifyPlaylistContainer();

    static QHash<sp_playlistcontainer*, SpotifyPlaylistContainer*> containerObjects;

    /*!
     * \brief loaded
     * \return
     */
    bool loaded() const;

    /*!
     * \brief playlists
     * \return
     */
    QList<SpotifyPlaylist*> playlists() const;

    /*!
     * \brief owner
     * \return
     */
    //SpotifyUser* owner() const;

    /*!
     * \brief native
     * \return
     */
    sp_playlistcontainer* native() const;



    void addPlaylist(SpotifyPlaylist* playlist);
    void createPlaylist(const QString& name);
    void removePlaylist(SpotifyPlaylist* playlist);
    void movePlaylist(SpotifyPlaylist* playlist, qint32 newPosition);
    void addFolder(qint32 position, const QString& name);

private:


    QList<SpotifyPlaylist*> m_playlists;

    sp_playlistcontainer* m_spContainer;
    sp_playlistcontainer_callbacks* m_callbacks;

};

#endif
