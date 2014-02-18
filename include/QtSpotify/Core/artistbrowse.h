#ifndef QTSPOTIFY_ARTISTBROWSE_H_
#define QTSPOTIFY_ARTISTBROWSE_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>

#include <libspotify/api.h>

#include <QtSpotify/Album>
#include <QtSpotify/Track>
#include <QtSpotify/Artist>

class QTS_EXPORT ArtistBrowse : public QObject
{

    Q_OBJECT

    Q_PROPERTY(Artist* artist READ artist NOTIFY completed)
    Q_PROPERTY(QList<Artist*> similarArtists READ similarArtists NOTIFY completed)
    Q_PROPERTY(QList<Track*> tracks READ tracks NOTIFY completed)
    Q_PROPERTY(QList<Album*> albums READ albums NOTIFY completed)
    Q_PROPERTY(QString biography READ biography NOTIFY completed)

public:

    static QHash<sp_artistbrowse*, ArtistBrowse*> artistBrowseObjects;

    ArtistBrowse(sp_artist* artist);
    Q_DISABLE_COPY(ArtistBrowse)
    virtual ~ArtistBrowse();

    Artist* artist() const;
    QList<Artist*> similarArtists() const;

    QList<Track*> tracks() const;
    QList<Album*> albums() const;

    QString biography() const;

protected:

    bool event(QEvent*);

signals:

    void completed();

private:

    void clearData();
    void loadResults();

    Artist* m_artist;
    QList<Artist*> m_similarArtists;

    QList<Track*> m_tracks;
    QList<Album*> m_albums;

    QString m_biography;

    sp_artistbrowse* m_spArtistBrowse;

};
#endif
