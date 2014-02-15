#ifndef SPOTIFYARTISTBROWSE_H
#define SPOTIFYARTISTBROWSE_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <libspotify/api.h>

class SpotifyTrack;
class SpotifyAlbum;

enum class ArtistBrowseType  {
    Full = SP_ARTISTBROWSE_FULL,
    NoTracks = SP_ARTISTBROWSE_NO_TRACKS,
    NoAlbums = SP_ARTISTBROWSE_NO_ALBUMS
};

class QTSPOTIFYSHARED_EXPORT SpotifyArtistBrowse : public QObject
{

    Q_OBJECT

public:

    static QHash<sp_artistbrowse*, SpotifyArtistBrowse> artistBrowseObjects;

    SpotifyArtistBrowse(sp_artist* artist);
    ~SpotifyArtistBrowse();

    QList<SpotifyTrack*> tracks() const;
    QList<SpotifyTrack*> tophits() const;
    QList<SpotifyAlbum*> albums() const;

    QStringList similarArtists() const;
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

    QStringList m_similarArtists;
    QString m_biography;

    qint32 m_serveTime;

    sp_artistbrowse* m_spArtistBrowse;
    sp_artist* m_artist;

};

#endif // SPOTIFYARTISTBROWSE_H
