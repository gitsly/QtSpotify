#ifndef QTSPOTIFY_ARTISTBROWSE_H_
#define QTSPOTIFY_ARTISTBROWSE_H_

#include <QtSpotify/Core/global.h>

#include <libspotify/api.h>
#include <memory>

#include <QtQml/QQmlListProperty>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QEvent>

namespace QtSpotify {

class Track;
class Artist;
class Album;

//TODO: Implement portraits
class QTS_EXPORT ArtistBrowse: public QObject
{

    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Track> tracks READ tracks NOTIFY artistBrowseDataChanged)
    Q_PROPERTY(QQmlListProperty<Track> topHits READ topHits NOTIFY artistBrowseDataChanged)
    Q_PROPERTY(QQmlListProperty<Album> albums READ albums NOTIFY artistBrowseDataChanged)
    Q_PROPERTY(Artist* artist READ artist NOTIFY artistBrowseDataChanged)
    Q_PROPERTY(QString biography READ biography NOTIFY artistBrowseDataChanged)

public:

    static QHash<sp_artistbrowse*, ArtistBrowse*> artistBrowseObjects;

    explicit ArtistBrowse(sp_artist* artist);
    virtual ~ArtistBrowse();

    QQmlListProperty<Track> tracks();
    QQmlListProperty<Album> albums();
    QQmlListProperty<Track> topHits();
    QQmlListProperty<Artist> similarArtists();

    Artist* artist() const;

    QString biography() const;

protected:

    bool event(QEvent*);

signals:

    void artistBrowseDataChanged();

private:

    void onArtistBrowseComplete();

    QList<std::shared_ptr<Track> > m_tracks;
    QList<std::shared_ptr<Track> > m_topHits;
    QList<std::shared_ptr<Album> > m_albums;
    QList<std::shared_ptr<Artist> > m_similarArtists;

    std::shared_ptr<Artist> m_artist;

    QString m_biography;

    std::shared_ptr<sp_artistbrowse> m_spArtistBrowse;

    static void SP_CALLCONV aristBrowseCompleteCallback(sp_artistbrowse* result, void*);

    static Track* tracksAtFunction(QQmlListProperty<Track>* list, int index);
    static qint32 tracksCountFunction(QQmlListProperty<Track>* list);

    static Track* topHitsAtFunction(QQmlListProperty<Track>* list, int index);
    static qint32 topHitsCountFunction(QQmlListProperty<Track>* list);

    static Album* albumsAtFunction(QQmlListProperty<Album>* list, int index);
    static qint32 albumsCountFunction(QQmlListProperty<Album>* list);

    static Artist* similarArtistsAtFunction(QQmlListProperty<Artist>* list, int index);
    static qint32 similarArtistsCountFunction(QQmlListProperty<Artist>* list);

};

}

#endif
