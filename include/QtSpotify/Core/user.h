#ifndef QTSPOTIFY_USER_H_
#define QTSPOTIFY_USER_H_

#include <QtSpotify/Core/global.h>

#include <QtCore/QObject>
#include <QtCore/QString>

#include <memory>

struct sp_user;

namespace QtSpotify {

class Playlist;
class PlaylistContainer;

class QTS_EXPORT User : public QObject
{

    Q_OBJECT

public:

    explicit User(sp_user* user);
    virtual ~User();

    QString displayName() const;
    QString canonicalName() const;

    Playlist* starredList() const;
    PlaylistContainer* playlistContainer() const;

private:

    void loadMetaData();

    QString m_displayName;
    QString m_canonicalName;

    std::shared_ptr<Playlist> m_starredList;
    std::shared_ptr<PlaylistContainer> m_playlistContainer;

    std::shared_ptr<sp_user> m_spUser;

};

}

#endif
