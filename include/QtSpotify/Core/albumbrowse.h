#ifndef QTSPOTIFY_ALBUMBROWSE_H_
#define QTSPOTIFY_ALBUMBROWSE_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Track>
#include <QtSpotify/Artist>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>

class QTS_EXPORT AlbumBrowse : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QStringList copyrights READ copyrights NOTIFY completed)
    Q_PROPERTY(QList<Track*> tracks READ tracks NOTIFY completed)
    Q_PROPERTY(Artist* artist READ artist NOTIFY completed)
    Q_PROPERTY(QString review READ review NOTIFY completed)

public:

    static QHash<sp_albumbrowse*, AlbumBrowse*> albumBrowseObjects;

    AlbumBrowse(sp_album* album);
    Q_DISABLE_COPY(AlbumBrowse)
    virtual ~AlbumBrowse();

    QStringList copyrights() const;
    QList<Track*> tracks() const;
    Artist* artist() const;
    QString review() const;

protected:

    bool event(QEvent*);

signals:

    void completed();

private:

    void clearData();
    void loadResults();

    QStringList m_copyrights;
    QList<Track*> m_tracks;
    Artist* m_artist;
    QString m_review;

    sp_albumbrowse* m_spAlbumBrowse;

};
#endif
