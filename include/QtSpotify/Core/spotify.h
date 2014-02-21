#ifndef QTSPOTIFY_SPOTIFY_H_
#define QTSPOTIFY_SPOTIFY_H_

#include <QtSpotify/Core/global.h>

#include <QtCore/QObject>

class QTS_EXPORT Spotify : public QObject
{

    Q_OBJECT

public:

    virtual ~Spotify();

    static Spotify& instance();


signals:

    void metadataUpdated();

private:

    Spotify();
    Q_DISABLE_COPY(Spotify)

};

#endif
