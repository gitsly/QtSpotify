#ifndef PLAYLISTCONTAINERCALLBACKS_H
#define PLAYLISTCONTAINERCALLBACKS_H

#include <libspotify/api.h>

static void SP_CALLCONV playlistAddedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, void* userData)
{

}

static void SP_CALLCONV playlistRemovedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, void* userData)
{

}

static void SP_CALLCONV playlistMovedCallback(sp_playlistcontainer* pc, sp_playlist* playlist, int position, int newPosition, void* userData)
{

}

static void SP_CALLCONV containerLoadedCallback(sp_playlistcontainer* pc, void* userData)
{

}

#endif // PLAYLISTCONTAINERCALLBACKS_H
