#ifndef QTSPOTIFY_DELETERS_H_
#define QTSPOTIFY_DELETERS_H_

#include <libspotify/api.h>

extern void deleteTrack(sp_track* track);

extern void deleteArtist(sp_artist* artist);

extern void deleteAlbum(sp_album* album);

extern void deleteArtistBrowse(sp_artistbrowse* artistBrowse);

extern void deleteAlbumBrowse(sp_albumbrowse* albumBrowse);

extern void deletePlaylist(sp_playlist* playlist);

extern void deletePlaylistContainer(sp_playlistcontainer* playlistContainer);

extern void deleteSearch(sp_search* search);

extern void deleteUser(sp_user* user);

#endif
