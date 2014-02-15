#ifndef ALBUMBROWSECALLBACKS_H
#define ALBUMBROWSECALLBACKS_H

#include <QtCore/qglobal.h>
#include <QtCore/QCoreApplication>
#include <QtSpotify/spotifyalbumbrowse.h>
#include <libspotify/api.h>

static void SP_CALLCONV albumBrowseCompleteCallback(sp_albumbrowse* result, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(SpotifyAlbumBrowse::albumBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
}

#endif // ALBUMBROWSECALLBACKS_H
