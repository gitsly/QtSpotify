#include <QtSpotify/playlistcontainer.h>
#include <QtSpotify/session.h>

#include <libspotify/api.h>

static QHash<sp_playlistcontainer*, SpotifyPlaylistContainer*> g_containerObjects;

class QSpotifyPlaylistAddedEvent : public QEvent
{

public:

    QSpotifyPlaylistAddedEvent(sp_playlist *playlist, int pos)
        : QEvent(Type(User + 1))
        , m_playlist(playlist)
        , m_position(pos)
    { }

    sp_playlist *playlist() const { return m_playlist; }
    int position() const { return m_position; }

private:

    sp_playlist *m_playlist;
    int m_position;

};

class QSpotifyPlaylistRemovedEvent : public QEvent
{

public:

    QSpotifyPlaylistRemovedEvent(int position)
        : QEvent(Type(User + 2))
        , m_position(position)
    { }

    int position() const { return m_position; }

private:

    int m_position;

};

class QSpotifyPlaylistMovedEvent : public QEvent
{

public:

    QSpotifyPlaylistMovedEvent(int oldpos, int newpos)
        : QEvent(Type(User + 3))
        , m_position(oldpos)
        , m_newposition(newpos)
    { }

    int position() const { return m_position; }
    int newPosition() const { return m_newposition; }

private:

    int m_position;
    int m_newposition;

};

static void SP_CALLCONV containerLoadedCallback(sp_playlistcontainer *pc, void *)
{
    QCoreApplication::postEvent(g_containerObjects.value(pc), new QEvent(QEvent::User));
}

static void SP_CALLCONV playlistAddedCallback(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *)
{
    QCoreApplication::postEvent(g_containerObjects.value(pc), new QSpotifyPlaylistAddedEvent(playlist, position));
}

static void SP_CALLCONV playlistRemovedCallback(sp_playlistcontainer *pc, sp_playlist *, int position, void *)
{
    QCoreApplication::postEvent(g_containerObjects.value(pc), new QSpotifyPlaylistRemovedEvent(position));
}

static void SP_CALLCONV playlistMovedCallback(sp_playlistcontainer *pc, sp_playlist *, int position, int new_position, void *)
{
    QCoreApplication::postEvent(g_containerObjects.value(pc), new QSpotifyPlaylistMovedEvent(position, new_position));
}

SpotifyPlaylistContainer::SpotifyPlaylistContainer(sp_playlistcontainer* playlistContainer) :
    QObject(nullptr)
{
    m_spContainer = playlistContainer;

    g_containerObjects.insert(m_spContainer, this);
    m_containerCallbacks = new sp_playlistcontainer_callbacks;
    m_containerCallbacks->container_loaded = containerLoadedCallback;
    m_containerCallbacks->playlist_added = playlistAddedCallback;
    m_containerCallbacks->playlist_moved = playlistMovedCallback;
    m_containerCallbacks->playlist_removed = playlistRemovedCallback;
    sp_playlistcontainer_add_callbacks(m_spContainer, m_containerCallbacks, nullptr);

    connect(SpotifySession::instance(), &SpotifySession::offlineModeChanged, this, &SpotifyPlaylistContainer::updatePlaylists);

    updateData();
}

SpotifyPlaylistContainer::~SpotifyPlaylistContainer()
{
    g_containerObjects.remove(m_spContainer);
    sp_playlistcontainer_remove_callbacks(m_spContainer, m_containerCallbacks, nullptr);
    qDeleteAll(m_playlists);
    sp_playlistcontainer_release(m_spContainer);

    delete m_containerCallbacks;
}

bool SpotifyPlaylistContainer::loaded()
{
    return sp_playlistcontainer_is_loaded(m_spContainer);
}

QList<SpotifyPlaylist*> SpotifyPlaylistContainer::playlists() const
{
    return m_playlists;
}

sp_playlistcontainer* SpotifyPlaylistContainer::spContainer() const
{
    return m_spContainer;
}

void SpotifyPlaylistContainer::updateData()
{
    bool updated = false;

    if(m_playlists.isEmpty()) {
        for(qint32 i=0 ; i<sp_playlistcontainer_num_playlists(m_spContainer) ; ++i) {
            addPlaylist(sp_playlistcontainer_playlist(m_spContainer, i), i);
            if(sp_playlistcontainer_playlist_type(m_spContainer, i) == SP_PLAYLIST_TYPE_PLACEHOLDER) {
                sp_playlistcontainer_remove_playlist(m_spContainer, i);
            }
        }

        updated = true;
        updatePlaylists();
    }

    return updated;
}
