#ifndef ALBUMBROWSECALLBACKS_H
#define ALBUMBROWSECALLBACKS_H

#include <libspotify/api.h>
#include <QtSpotify/AlbumBrowse>
#include <QtCore/QCoreApplication>

static void SP_CALLCONV albumBrowseCompleteCallback(sp_albumbrowse* result, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(AlbumBrowse::albumBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
}

#endif // ALBUMBROWSECALLBACKS_H
