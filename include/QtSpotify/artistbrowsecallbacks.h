#ifndef ARTISTBROWSECALLBACKS_H
#define ARTISTBROWSECALLBACKS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QHash>
#include <QtSpotify/spotifyartistbrowse.h>
#include <libspotify/api.h>

static void SP_CALLCONV artistBrowseCompleteCallback(sp_artistbrowse* result, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyArtistBrowse::artistBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
}

#endif // ARTISTBROWSECALLBACKS_H
