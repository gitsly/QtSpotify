#ifndef SPOTIFYSESSION_H
#define SPOTIFYSESSION_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>

#include <libspotify/api.h>

class QTSPOTIFYSHARED_EXPORT SpotifySession : public QObject
{

    Q_OBJECT

public:

    static SpotifySession* instance();

    sp_session* native() const;

signals:

    void metadataUpdated();

private:

    SpotifySession();
    Q_DISABLE_COPY(SpotifySession)

    sp_session* m_spSession;

};

#endif // SPOTIFYSESSION_H
