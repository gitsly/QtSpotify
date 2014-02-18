#ifndef QTSPOTIFY_SEARCHCALLBACKS_H_
#define QTSPOTIFY_SEARCHCALLBACKS_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Search>
#include <libspotify/api.h>

#include <QtCore/QCoreApplication>

static void SP_CALLCONV searchCompletedCallback(sp_search* result, void* userData)
{
    Q_UNUSED(userData)
    QCoreApplication::postEvent(Search::searchObjects.value(result), new QEvent(QEvent::User));
}

#endif
