#ifndef SPOTIFYARTISTBROWSE_H
#define SPOTIFYARTISTBROWSE_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <libspotify/api.h>

class SpotifyTrack;
class SpotifyAlbum;
class SpotifyArtist;

enum class ArtistBrowseType  {
    Full = SP_ARTISTBROWSE_FULL,
    NoTracks = SP_ARTISTBROWSE_NO_TRACKS,
    NoAlbums = SP_ARTISTBROWSE_NO_ALBUMS
};

class QTSPOTIFYSHARED_EXPORT SpotifyArtistBrowse : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QList<SpotifyTrack*> tracks READ tracks NOTIFY completed)
    Q_PROPERTY(QList<SpotifyTrack*> tophits READ tophits NOTIFY completed)
    Q_PROPERTY(QList<SpotifyAlbum*> albums READ albums NOTIFY completed)
    Q_PROPERTY(SpotifyArtist* artist READ artist NOTIFY completed)
    Q_PROPERTY(QList<SpotifyArtist*> similarArtists READ similarArtists NOTIFY completed)
    Q_PROPERTY(QString biography READ biography NOTIFY completed)
    Q_PROPERTY(qint32 serveTime READ serveTime NOTIFY completed)

    Q_ENUMS(ArtistBrowseType)

public:

    static QHash<sp_artistbrowse*, SpotifyArtistBrowse> artistBrowseObjects;

    SpotifyArtistBrowse(sp_artist* artist);
    ~SpotifyArtistBrowse();

    SpotifyArtist* artist() const;

    QList<SpotifyTrack*> tracks() const;
    QList<SpotifyTrack*> tophits() const;
    QList<SpotifyAlbum*> albums() const;

    QList<SpotifyArtist*> similarArtists() const;
    QString biography() const;

    qint32 serveTime() const;

protected:

    bool event(QEvent*);

private slots:

    void updateData();

signals:

    void completed();

private:

    QList<SpotifyTrack*> m_tracks;
    QList<SpotifyTrack*> m_tophitTracks;

    QList<SpotifyAlbum*> m_albums;
    SpotifyArtist* m_artist;

    QList<SpotifyArtist*> m_similarArtists;
    QString m_biography;

    qint32 m_serveTime;

    sp_artistbrowse* m_spArtistBrowse;
};

#endif // SPOTIFYARTISTBROWSE_H
