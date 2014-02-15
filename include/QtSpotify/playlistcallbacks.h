#ifndef PLAYLISTCALLBACKS_H
#define PLAYLISTCALLBACKS_H

#include <QtSpotify/spotifyplaylist.h>
#include <QtSpotify/spotifytrack.h>

#include <libspotify/api.h>
#include <QtCore/QCoreApplication>

#include "playlistevents.h"

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
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistTracksAddedEvent(tracks, numTracks, position));
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
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistTracksRemovedEvent(tracks, numTracks));
}

/*!
 * \brief Called when one or more tracks have been moved within a playlist
 * \param playlist The affected playlist
 * \param tracks Array of position representing the tracks that were moved
 * \param numTracks Number of entries in tracks
 * \param newPosition New position in the playlist for the first track
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV tracksMovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, int newPosition, void* userData)
{

}

/*!
 * \brief Called when a playlist has been renamed
 * \param playlist The affected playlist
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV playlistRenamedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistRenamedEvent(playlist));
}

/*!
 * \brief Playlist state changed is called when either a) Collaboration has changed, b) pending changes appeared or were commited or c) the playlist started or finished loading
 * \param playlist The affected playlist
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV playlistStateChangedCallback(sp_playlist* playlist, void* userData)
{

}

/*!
 * \brief Called when a playlist is updating or is done updating
 * \param playlist The affected playlist
 * \param done True iff the update is completed
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 *
 * This is called before and after a series of changes are applied to the playlist.
 */
static void SP_CALLCONV playlistUpdateInProgressCallback(sp_playlist* playlist, bool done, void* userData)
{

}

/*!
 * \brief Called when metadata for one or more tracks in a playlist has been updated
 * \param playlist The affected playlist
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV playlistMetadataChangedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new QEvent(QEvent::Type(QEvent::User + 7)));
}

/*!
 * \brief Called when create time and/or creator for a playlist entry changes
 * \param playlist The affected playlist
 * \param position Track position in playlist
 * \param user User object
 * \param when When entry was created, seconds since the unix epoch
 * \param userData userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV trackCreatedChangedCallback(sp_playlist* playlist, int position, sp_user* user, int when, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistTrackCreatedChangedEvent(position, user, when));
}

/*!
 * \brief Called when the seen attribute for a playlist entry changes
 * \param playlist The affected playlist
 * \param position The entry's position in the playlist
 * \param seen Set if entry is marked as seen
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV trackSeenChangedCallback(sp_playlist* playlist, int position, bool seen, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistTrackSeenChangedEvent(position, seen));
}

/*!
 * \brief Called when playlist description has changed
 * \param playlist The affected playlist
 * \param description New description
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV descriptionChangedCallback(sp_playlist* playlist, const char* description, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistDescriptionChangedEvent(description));
}

/*!
 * \brief Called when the playlist image has changed
 * \param playlist The affected playlist
 * \param image New image
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV imageChangedCallback(sp_playlist* playlist, const byte* image, void* userData)
{

}

/*!
 * \brief Called when the message attribute for a playlist entry changes
 * \param playlist The affected playlist
 * \param position The entry's position in the playlist
 * \param message UTF-8 encoded message
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV trackMessageChangedCallback(sp_playlist* playlist, int position, const char* message, void* userData)
{

}

/*!
 * \brief Called when playlist subscribers changes (count or list of names)
 * \param playlist The affected playlist
 * \param userData Userdata passed to sp_playlist_add_callbacks()
 */
static void SP_CALLCONV subscribersChangedCallback(sp_playlist* playlist, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyPlaylist::playlistObjects.value(playlist), new PlaylistSubscribersChangedEvent(playlist));
}

#endif // PLAYLISTCALLBACKS_H
