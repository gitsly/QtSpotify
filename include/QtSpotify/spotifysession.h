#ifndef SPOTIFYSESSION_H
#define SPOTIFYSESSION_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QMutex>

#include <libspotify/api.h>

class SpotifyUser;

class QTSPOTIFYSHARED_EXPORT SpotifySession : public QObject
{

    Q_OBJECT

public:

    static SpotifySession* instance();

    sp_session* native() const;

    SpotifyUser* user() const;

signals:

    void metadataUpdated();

private:

    SpotifySession();
    Q_DISABLE_COPY(SpotifySession)

    void init();

    sp_session* m_spSession;
    static SpotifySession* m_instance;

    SpotifyUser* m_user;

};

#endif // SPOTIFYSESSION_H
