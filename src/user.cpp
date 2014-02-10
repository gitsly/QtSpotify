#include <QtSpotify/user.h>

SpotifyUser::SpotifyUser(sp_user* user);
SpotifyUser::~SpotifyUser();
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
