#ifndef QTSPOTIFY_SPOTIFYPLAYLISTCONTAINER_H_
#define QTSPOTIFY_SPOTIFYPLAYLISTCONTAINER_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QList>
#include <libspotify/api.h>

class SpotifyPlaylist;
class SpotifyUser;

class QTSPOTIFYSHARED_EXPORT SpotifyPlaylistContainer : public QObject
{

    Q_OBJECT

public:

    SpotifyPlaylistContainer(sp_playlistcontainer* playlistContainer);
    virtual ~SpotifyPlaylistContainer();

    /*!
     * \brief loaded
     * \return
     */
    bool loaded() const;

    QList<SpotifyPlaylist*> playlists() const;

    SpotifyUser* owner() const;



    void addPlaylist(SpotifyPlaylist* playlist);
    void createPlaylist(const QString& name);
    void removePlaylist(SpotifyPlaylist* playlist);
    void movePlaylist(SpotifyPlaylist* playlist, qint32 newPosition);
    void addFolder(qint32 position, const QString& name);

private:

    QList<SpotifyPlaylist*> m_playlists;

};

#endif
