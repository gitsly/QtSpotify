#ifndef QTSPOTIFY_PLAYLIST_H_
#define QTSPOTIFY_PLAYLIST_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQml/QQmlListProperty>
#include <libspotify/api.h>
#include <memory>

class Track;

class QTS_EXPORT Playlist : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Track> tracks READ tracks NOTIFY tracksChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool collaborative READ collaborative NOTIFY collaborativeChanged)

public:

    explicit Playlist(sp_playlist* playlist);
    virtual ~Playlist();

    QString name() const;
    bool collaborative() const;
    QQmlListProperty<Track> tracks();

private slots:

    void onMetadataUpdated();

signals:

    void tracksChanged();
    void nameChanged(const QString& name);
    void collaborativeChanged(bool collaborative);

    void playlistDataChanged();

private:

    QString m_name;
    bool m_collaborative;
    QList<std::shared_ptr<Track>> m_tracks;

    std::shared_ptr<sp_playlist> m_spPlaylist;

    static void tracksAppendFunction(QQmlListProperty<Track>* list, Track* track);
    static Track* tracksAtFunction(QQmlListProperty<Track>* list, int index);
    static void tracksClearFunction(QQmlListProperty<Track>* list);
    static qint32 tracksCountFunction(QQmlListProperty<Track>* list);

};

#endif
