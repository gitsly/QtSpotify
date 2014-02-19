#ifndef SESSIONCALLBACKS_H
#define SESSIONCALLBACKS_H

#include <QtSpotify/Core/global.h>
#include <libspotify/api.h>

static void SP_CALLCONV loggedInCallback(sp_session* session, sp_error error)
{

}

static void SP_CALLCONV loggedOutCallback(sp_session* session)
{

}

static void SP_CALLCONV metadataUpdatedCallback(sp_session* session)
{

}

static void SP_CALLCONV connectionErrorCallback(sp_session* session, sp_error error)
{

}

static void SP_CALLCONV messageToUserCallback(sp_session* session, const char* message)
{

}

static void SP_CALLCONV notifyMainThreadCallback(sp_session* session)
{

}

static int SP_CALLCONV musicDeliveryCallback(sp_session* session, const sp_audioformat* format, const void* frames, int numFrames)
{
    return -1;
}

static void SP_CALLCONV playTokenLostCallback(sp_session* session)
{

}

static void SP_CALLCONV logMessageCallback(sp_session* session, const char* data)
{

}

static void SP_CALLCONV endOfTrackCallback(sp_session* session)
{

}

static void SP_CALLCONV streamingErrorCallback(sp_session* session, sp_error error)
{

}

static void SP_CALLCONV userInfoUpdatedCallback(sp_session* session)
{

}

static void SP_CALLCONV startPlaybackCallback(sp_session* session)
{

}

static void SP_CALLCONV stopPlaybackCallback(sp_session* session)
{

}

static void SP_CALLCONV getAudioBufferStatsCallback(sp_session* session, sp_audio_buffer_stats* stats)
{

}

static void SP_CALLCONV offlineStatusUpdatedCallback(sp_session* session)
{

}

static void SP_CALLCONV offlineErrorCallback(sp_session* session, sp_error error)
{

}

static void SP_CALLCONV credentialsBlogUpdatedCallback(sp_session* session, const char* blob)
{

}

static void SP_CALLCONV connectionStateUpdatedCallback(sp_session* session)
{

}

static void SP_CALLCONV scrobbleErrorCallback(sp_session* session, sp_error error)
{

}

static void SP_CALLCONV privateSessionModeChangedCallback(sp_session* session, bool isPrivate)
{

}


#endif // SESSIONCALLBACKS_H
