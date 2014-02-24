#ifndef QTSPOTIFY_TRACK_H_
#define QTSPOTIFY_TRACK_H_

#include <QtSpotify/Core/global.h>

#include <QtQml/QQmlListProperty>
#include <QtCore/QObject>
#include <libspotify/api.h>
#include <memory>

#include <QtSpotify/Core/user.h>

namespace QtSpotify {

class Album;
class Artist;
class Playlist;

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

class QTS_EXPORT Track : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY trackDataChanged)
    Q_PROPERTY(QQmlListProperty<Artist> artists READ artists NOTIFY trackDataChanged)
    Q_PROPERTY(Album* album READ album NOTIFY trackDataChanged)
    Q_PROPERTY(qint32 duration READ duration NOTIFY trackDataChanged)
    Q_PROPERTY(qint32 popularity READ popularity NOTIFY trackDataChanged)
    Q_PROPERTY(qint32 disc READ disc NOTIFY trackDataChanged)
    Q_PROPERTY(qint32 discIndex READ discIndex NOTIFY trackDataChanged)

public:

    explicit Track(sp_track* track);
    virtual ~Track();

    bool loaded() const;

    QString name() const;

    QQmlListProperty<Artist> artists();
    Album* album() const;
    User* creator() const;

    bool starred() const;
    bool seen() const;

    qint32 duration() const;
    qint32 popularity() const;
    qint32 disc() const;
    qint32 discIndex() const;

    TrackOfflineStatus offlineStatus() const;

private slots:

    void onMetadataUpdated();

signals:

    void trackDataChanged();

private:

    QString m_name;

    QList<std::shared_ptr<Artist> > m_artists;
    std::shared_ptr<Album> m_album;
    std::shared_ptr<User> m_creator;

    bool m_starred;
    bool m_seen;

    qint32 m_duration;
    qint32 m_popularity;
    qint32 m_disc;
    qint32 m_discIndex;

    TrackOfflineStatus m_offlineStatus;

    std::weak_ptr<Playlist> m_playlist;
    std::shared_ptr<sp_track> m_spTrack;

    static qint32 artistsCountFunction(QQmlListProperty<Artist>* list);
    static Artist* artistsAtFunction(QQmlListProperty<Artist>* list, qint32 index);
};

}

#endif
