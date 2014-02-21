#include <QtSpotify/Core/deleters.h>

void deleteTrack(sp_track* track) {
    sp_track_release(track);
}

void deleteArtist(sp_artist* artist) {
    sp_artist_release(artist);
}

void deleteAlbum(sp_album* album) {
    sp_album_release(album);
}

void deleteArtistBrowse(sp_artistbrowse* artistBrowse) {
    sp_artistbrowse_release(artistBrowse);
}

void deleteAlbumBrowse(sp_albumbrowse* albumBrowse) {
    sp_albumbrowse_release(albumBrowse);
}

void deletePlaylist(sp_playlist* playlist) {
    sp_playlist_release(playlist);
}

void deletePlaylistContainer(sp_playlistcontainer* playlistContainer) {
    sp_playlistcontainer_release(playlistContainer);
}

void deleteSearch(sp_search* search) {
    sp_search_release(search);
}

void deleteUser(sp_user* user) {
    sp_user_release(user);
}
