#ifndef SPOTIFYALBUMBROWSE_CPP
#define SPOTIFYALBUMBROWSE_CPP

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QStringList>

class SpotifyTrack;
class SpotifyAlbum;
struct sp_album;
struct sp_albumbrowse;

class QTSPOTIFYSHARED_EXPORT SpotifyAlbumBrowse : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QStringList copyrights READ copyrights NOTIFY completed)
    Q_PROPERTY(QList<SpotifyTrack*> tracks READ tracks NOTIFY completed)
    Q_PROPERTY(SpotifyAlbum* album READ album NOTIFY completed)
    Q_PROPERTY(QString review READ review NOTIFY completed)
    Q_PROPERTY(qint32 serveTime READ serveTime NOTIFY completed)

public:

    QHash<sp_albumbrowse*, SpotifyAlbumBrowse*> albumBrowseObjects;

    SpotifyAlbumBrowse(sp_album* album);
    ~SpotifyAlbumBrowse();

    QStringList copyrights() const;

    QList<SpotifyTrack*> tracks() const;

    SpotifyAlbum* album() const;

    QString review() const;

    qint32 serveTime() const;

    sp_albumbrowse* native() const;

protected:

    bool event(QEvent*);

private slots:

    void updateData();

signals:

    void completed();

private:

    QStringList m_copyrights;
    QList<SpotifyTrack*> m_tracks;

    SpotifyAlbum* m_album;

    QString m_review;
    qint32 m_serveTime;

    sp_albumbrowse* m_spAlbumBrowse;

};

#endif // SPOTIFYALBUMBROWSE_CPP
