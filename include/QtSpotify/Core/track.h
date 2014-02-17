#ifndef QTSPOTIFY_TRACK_H_
#define QTSPOTIFY_TRACK_H_

#include <QtSpotify/Core/global.h>
#include <libspotify/api.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QDateTime>

class Artist;
class Album;
class Playlist;
class User;

enum class TrackOfflineStatus {
    No = SP_TRACK_OFFLINE_NO,
    Waiting = SP_TRACK_OFFLINE_WAITING,
    Downloading = SP_TRACK_OFFLINE_DOWNLOADING,
    Done = SP_TRACK_OFFLINE_DONE,
    Error = SP_TRACK_OFFLINE_ERROR,
    DoneExpired = SP_TRACK_OFFLINE_DONE_EXPIRED,
    LimitExceeded = SP_TRACK_OFFLINE_LIMIT_EXCEEDED,
    DoneResync = SP_TRACK_OFFLINE_DONE_RESYNC
};

enum class TrackError {
    Ok = SP_ERROR_OK,
    IsLoading = SP_ERROR_IS_LOADING,
    OtherPermanent = SP_ERROR_OTHER_PERMANENT
};

class QTS_EXPORT Track : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY metadataChanged)
    Q_PROPERTY(bool starred READ starred WRITE setStarred NOTIFY starredChanged)
    Q_PROPERTY(QList<Artist*> artists READ artists NOTIFY metadataChanged)
    Q_PROPERTY(Album* album READ album NOTIFY metadataChanged)
    Q_PROPERTY(qint32 duration READ duration NOTIFY metadataChanged)
    Q_PROPERTY(qint32 popularity READ popularity NOTIFY metadataChanged)
    Q_PROPERTY(qint32 disc READ disc NOTIFY metadataChanged)
    Q_PROPERTY(qint32 discIndex READ discIndex NOTIFY metadataChanged)
    Q_PROPERTY(TrackOfflineStatus offlineStatus READ offlineStatus NOTIFY offlineStatusChanged)

    Q_ENUMS(TrackOfflineStatus TrackError)

public:

    explicit Track(sp_track *track, Playlist* playlist = nullptr);
    virtual ~Track();

    bool loaded() const;
    bool available() const;
    bool availableOffline() const;
    QString name() const;
    bool starred() const;
    void setStarred(bool starred);
    QList<Artist*> artists() const;
    Album* album() const;
    qint32 duration() const;
    qint32 popularity() const;
    qint32 disc() const;
    qint32 discIndex() const;
    TrackOfflineStatus offlineStatus() const;

    User* creator() const;
    QDateTime creationTime() const;

    sp_track* native() const;

private slots:

    void onMetadataUpdated();

signals:

    void metadataChanged();
    void starredChanged(bool starred);
    void offlineStatusChanged(TrackOfflineStatus offlineStatus);

private:

    QString m_name;
    bool m_starred;
    bool m_available;
    QList<Artist*> m_artists;
    Album* m_album;
    qint32 m_duration;
    qint32 m_popularity;
    qint32 m_disc;
    qint32 m_discIndex;
    TrackOfflineStatus m_offlineStatus;

    User* m_creator;
    QDateTime m_creationTime;
    Playlist* m_playlist;

    sp_track* m_spTrack;

};

#endif
