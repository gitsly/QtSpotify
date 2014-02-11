#ifndef QTSPOTIFY_PLAYLISTCONTAINER_H_
#define QTSPOTIFY_PLAYLISTCONTAINER_H_

#include <QtSpotify/qtspotify_global.h>
#include <QObject>

#include "playlistcontainercallbacks.h"

class SpotifyPlaylist;
struct sp_playlistcontainer;
struct sp_playlist;
struct sp_playlistcontainer_callbacks;

class QTSPOTIFYSHARED_EXPORT SpotifyPlaylistContainer : public QObject
{

    Q_OBJECT

public:

    SpotifyPlaylistContainer(sp_playlistcontainer* playlistContainer);
    ~SpotifyPlaylistContainer();

    bool loaded();

    QList<SpotifyPlaylist*> playlists() const;

    sp_playlistcontainer* spContainer() const;

private slots:

    void updateData();

signals:

    void playlistContainerDataChanged();
    void playlistsNameChanged();

private:

    sp_playlistcontainer* m_spContainer;
    sp_playlistcontainer_callbacks* m_containerCallbacks;

    QList<SpotifyPlaylist*> m_playlists;

};

#endif
