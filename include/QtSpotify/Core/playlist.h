#ifndef QTSPOTIFY_PLAYLIST_H_
#define QTSPOTIFY_PLAYLIST_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QEvent>
#include <libspotify/api.h>

class Track;
class User;

enum class PlaylistOfflineStatus {
    No = SP_PLAYLIST_OFFLINE_STATUS_NO,
    Yes = SP_PLAYLIST_OFFLINE_STATUS_YES,
    Downloading = SP_PLAYLIST_OFFLINE_STATUS_DOWNLOADING,
    Waiting = SP_PLAYLIST_OFFLINE_STATUS_WAITING
};

class QTS_EXPORT Playlist : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QList<Track*> tracks READ tracks NOTIFY metadataChanged)
    Q_PROPERTY(QString name READ name NOTIFY metadataChanged)
    Q_PROPERTY(User* owner READ owner NOTIFY metadataChanged)
    Q_PROPERTY(bool collaborative READ collaborative WRITE setCollaborative NOTIFY metadataChanged)
    Q_PROPERTY(QString description READ description NOTIFY metadataChanged)
    Q_PROPERTY(QList<User*> subscribers READ subscribers NOTIFY metadataChanged)
    Q_PROPERTY(PlaylistOfflineStatus offlineStatus READ offlineStatus NOTIFY metadataChanged)

    Q_ENUMS(PlaylistOfflineStatus)

public:

    static QHash<sp_playlist*, Playlist*> playlistObjects;

    explicit Playlist(sp_playlist* playlist);
    virtual ~Playlist();

    bool loaded() const;

    QList<Track*> tracks() const;

    QString name() const;

    User* owner() const;

    bool collaborative() const;

    void setCollaborative(bool collaborative);

    QString description() const;

    QList<User*> subscribers() const;

    PlaylistOfflineStatus offlineStatus() const;

    sp_playlist* native() const;

    void add(Track* track, qint32 position = -1);
    void remove(Track* track);
    void move(Track* track, qint32 newPosition);

protected:

    bool event(QEvent*);

private slots:

    void onMetadataUpdated();

signals:

    void metadataChanged();
    void nameChanged(const QString& name);
    void descriptionChanged(const QString& description);

private:

    qint32 getTrackIndex(sp_track* track);

    QList<Track*> m_tracks;

    QString m_name;
    User* m_owner;
    bool m_collaborative;
    QString m_description;
    QList<User*> m_subscribers;
    PlaylistOfflineStatus m_offlineStatus;

    sp_playlist* m_spPlaylist;
    sp_playlist_callbacks* m_callbacks;

};
#endif
