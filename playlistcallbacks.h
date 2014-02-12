#ifndef PLAYLISTCALLBACKS_H
#define PLAYLISTCALLBACKS_H

#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifytrack.h>
#include <libspotify/api.h>

/*!
 * \brief Called when one or more tracks have been added to a playlist
 * \param playlist The affected playlist
 * \param tracks Array of pointers to track objects
 * \param numTracks Number of entries in tracks
 * \param position Position in the playlist for the first track
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV tracksAddedCallback(sp_playlist* playlist, sp_track* const* tracks, int numTracks, int position, void* userData)
{

}

/*!
 * \brief Called when one or more tracks have been removed from a playlist
 * \param playlist The affected playlist
 * \param tracks Array of positions representing the tracks that were removed
 * \param numTracks Number of entries in tracks
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV tracksRemovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, void* userData)
{

}

static void SP_CALLCONV playlistRenamedCallback(sp_playlist* playlist, void* userData)
{

}

static void SP_CALLCONV playlistStateChangedCallback(sp_playlist* playlist, void* userData)
{

}

static void SP_CALLCONV playlistUpdateInProgressCallback(sp_playlist* playlist, bool done, void* userData)
{

}

static void SP_CALLCONV playlistMetadataChangedCallback(sp_playlist* playlist, void* userData)
{

}

static void SP_CALLCONV trackCreatedChangedCallback(sp_playlist* playlist, int position, sp_user* user, int when, void* userData)
{

}

static void SP_CALLCONV trackSeenChanged(sp_playlist* playlist, int position, bool seen, void* userData)
{

}

static void SP_CALLCONV descriptionChangedCallback(sp_playlist* playlist, const char* description, void* userData)
{

}

static void SP_CALLCONV imageChangedCallback(sp_playlist* playlist, const byte* image, void* userData)
{

}

static void SP_CALLCONV trackMessageChangedCallback(sp_playlist* playlist, int position, const char* message, void* userData)
{

}

static void SP_CALLCONV subscribersChangedCallback(sp_playlist* playlist, void* userData)
{

}

#endif // PLAYLISTCALLBACKS_H
