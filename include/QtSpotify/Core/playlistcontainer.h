#ifndef QTSPOTIFY_PLAYLISTCONTAINER_H_
#define QTSPOTIFY_PLAYLISTCONTAINER_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Core/playlistcontainerevents.h>

#include <QtQml/QQmlListProperty>
#include <QtCore/QObject>
#include <QtCore/QHash>

#include <memory>
#include <libspotify/api.h>

namespace QtSpotify {

class Playlist;
class User;

class QTS_EXPORT PlaylistContainer : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Playlist> playlists READ playlists NOTIFY playlistsChanged)
    Q_PROPERTY(User* owner READ owner NOTIFY playlistContainerDataChanged)


public:

    static QHash<sp_playlistcontainer*, PlaylistContainer*> containerObjects;

    explicit PlaylistContainer(sp_playlistcontainer* container);
    virtual ~PlaylistContainer();

    User* owner() const;
    QQmlListProperty<Playlist> playlists();

protected:

    bool event(QEvent*);

private slots:

    void onMetadataUpdated();

signals:

    void playlistsChanged();
    void playlistContainerDataChanged();

private:

    void onPlaylistAdded(PlaylistAddedEvent* event);
    void onPlaylistRemoved(PlaylistRemovedEvent* event);
    void onPlaylistMoved(PlaylistMovedEvent* event);

    QList<std::shared_ptr<Playlist> > m_playlists;
    std::shared_ptr<User> m_owner;

    std::shared_ptr<sp_playlistcontainer> m_spContainer;
    std::shared_ptr<sp_playlistcontainer_callbacks> m_callbacks;

    static void SP_CALLCONV playlistAddedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, void*);
    static void SP_CALLCONV playlistRemovedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, void*);
    static void SP_CALLCONV playlistMovedCallback(sp_playlistcontainer* container, sp_playlist* playlist, int position, int newPosition, void*);
    static void SP_CALLCONV containerLoadedCallback(sp_playlistcontainer* container, void*);

    static qint32 playlistsCountFunction(QQmlListProperty<Playlist>* list);
    static Playlist* playlistsAtFunction(QQmlListProperty<Playlist>* list, qint32 index);
};

}

#endif
