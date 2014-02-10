#include <QtSpotify/session.h>
#include "sessionevents.h"
#include "sessioncallbacks.h"

#include <QWaitCondition>

static QMutex g_imageRequestMutex;
static QHash<QString, QWaitCondition*> g_imageRequestConditions;
static QHash<QString, QImage> g_imageRequestImages;
static QHash<sp_image*, QString> g_imageRequestObjects;

SpotifySession* SpotifySession::m_instance = nullptr;

SpotifySession::SpotifySession()
{

}

SpotifySession* SpotifySession::instance()
{
    static QMutex mutex;
    if(m_instance == nullptr) {
        mutex.lock();
        if(m_instance == nullptr) {
            m_instance = new SpotifySession();
            m_instance->init();
        }
    }

    return m_instance;
}

void SpotifySession::init()
{

}
