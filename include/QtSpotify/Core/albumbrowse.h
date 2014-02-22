#ifndef QTSPOTIFY_ALBUMBROWSE_H_
#define QTSPOTIFY_ALBUMBROWSE_H_

#include <QtSpotify/Core/global.h>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtQml/QQmlListProperty>

#include <memory>

#include <libspotify/api.h>

namespace QtSpotify {

class Track;
class Album;
class Artist;

class QTS_EXPORT AlbumBrowse : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Track> tracks READ tracks NOTIFY albumBrowseDataChanged)
    Q_PROPERTY(QString review READ review NOTIFY albumBrowseDataChanged)
    Q_PROPERTY(QStringList copyrights READ copyrights NOTIFY albumBrowseDataChanged)

public:

    static QHash<sp_albumbrowse*, AlbumBrowse*> albumBrowseObjects;

    explicit AlbumBrowse(sp_album* album);
    virtual ~AlbumBrowse();

    QQmlListProperty<Track> tracks();

    QString review() const;

    QStringList copyrights() const;

    Album* album() const;

    Artist* artist() const;

protected:

    bool event(QEvent*);

signals:

    void albumBrowseDataChanged();

private:

    void onAlbumBrowseComplete();

    QList<std::shared_ptr<Track>> m_tracks;
    std::shared_ptr<Album> m_album;
    std::shared_ptr<Artist> m_artist;
    std::shared_ptr<sp_albumbrowse> m_spAlbumBrowse;

    QString m_review;
    QStringList m_copyrights;

    static void SP_CALLCONV albumBrowseCompleteCallback(sp_albumbrowse* result, void*);

    static Track* tracksAtFunction(QQmlListProperty<Track>* list, int index);
    static qint32 tracksCountFunction(QQmlListProperty<Track>* list);

};

}

#endif
