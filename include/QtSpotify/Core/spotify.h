#ifndef QTSPOTIFY_SPOTIFY_H_
#define QTSPOTIFY_SPOTIFY_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Core/deleters.h>

#include <memory>

#include <QtCore/QObject>


namespace QtSpotify {

class User;

class QTS_EXPORT Spotify : public QObject
{

    Q_OBJECT

public:

    virtual ~Spotify();

    static Spotify& instance();

   QtSpotify::User* user() const;

signals:

    void metadataUpdated();

private:

    Spotify();
    Q_DISABLE_COPY(Spotify)

    std::shared_ptr<QtSpotify::User> m_user;

};

}

#endif
