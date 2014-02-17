#ifndef QTSPOTIFY_USER_H_
#define QTSPOTIFY_USER_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>

class Playlist;
class PlaylistContainer;
struct sp_user;

class QTS_EXPORT User : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString canonicalName READ canonicalName NOTIFY metadataChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY metadataChanged)
    Q_PROPERTY(Playlist* starredList READ starredList NOTIFY metadataChanged)
    Q_PROPERTY(PlaylistContainer* playlistContainer READ playlistContainer NOTIFY metadataChanged)

public:

    User(sp_user* user);
    virtual ~User();

    bool loaded() const;

    Playlist* starredList() const;
    PlaylistContainer* playlistContainer() const;

    QString canonicalName() const;
    QString displayName() const;

    sp_user* native() const;

private slots:

    void onMetadataUpdated();

signals:

    void metadataChanged();

private:

    Playlist* m_starredList;
    PlaylistContainer* m_playlistContainer;

    QString m_canonicalName;
    QString m_displayName;

    sp_user* m_spUser;

};

#endif
