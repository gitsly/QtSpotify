#ifndef QTSPOTIFY_USER_H_
#define QTSPOTIFY_USER_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>

struct sp_user;
class SpotifyPlaylistContainer;
class SpotifyPlaylist;
class SpotifyTrack;
class SpotifyAlbumBrowse;

class QTSPOTIFYSHARED_EXPORT SpotifyUser : public QObject
{

    Q_OBJECT

public:

    SpotifyUser(sp_user* user);
    ~SpotifyUser();

    bool loaded();

    QString canonicalName() const;
    QString displayName() const;

    SpotifyPlaylistContainer* playlistContainer() const;
    SpotifyPlaylist* starredList() const;
    SpotifyPlaylist* inbox() const;

    Q_INVOKABLE bool createPlaylist(const QString& name);
    Q_INVOKABLE bool createPlaylistInFolder(const QString& name, SpotifyPlaylist* folder);
    Q_INVOKABLE bool createPlaylistFromTrack(SpotifyTrack* track);
    Q_INVOKABLE bool createPlaylistFromAlbum(SpotifyAlbumBrowse* album);
    Q_INVOKABLE void removePlaylist(SpotifyPlaylist* playlist);
    Q_INVOKABLE bool ownsPlaylist(SpotifyPlaylist* playlist);
    Q_INVOKABLE bool canModifyPlaylist(SpotifyPlaylist* playlist);

private slots:

    void updateData();

signals:

    void userDataChanged();
    void playlistsChanged();
    void playlistsNameChanged();

private:

    sp_user* m_spUser;

    QString m_canonicalName;
    QString m_displayName;

    SpotifyPlaylistContainer*  m_playlistContainer;
    SpotifyPlaylist* m_starredList;
    SpotifyPlaylist* m_inbox;

};


#endif
