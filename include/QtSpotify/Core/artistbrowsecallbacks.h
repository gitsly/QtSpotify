#ifndef ARTISTBROWSECALLBACKS_H
#define ARTISTBROWSECALLBACKS_H

#include <libspotify/api.h>
#include <QtCore/QHash>
#include <QtSpotify/ArtistBrowse>
#include <QtCore/QCoreApplication>

static void SP_CALLCONV artistBrowseCompleteCallback(sp_artistbrowse* result, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(ArtistBrowse::artistBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
}

#endif // ARTISTBROWSECALLBACKS_H
