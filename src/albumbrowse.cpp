#include <QtSpotify/AlbumBrowse>
#include <QtSpotify/Session>
#include <QtSpotify/Core/albumbrowsecallbacks.h>

QHash<sp_albumbrowse*, AlbumBrowse*> AlbumBrowse::albumBrowseObjects = QHash<sp_albumbrowse*, AlbumBrowse*>();

AlbumBrowse::AlbumBrowse(sp_album* album)
{
    m_spAlbumBrowse = sp_albumbrowse_create(Session::instance()->native(), album, albumBrowseCompleteCallback, nullptr);
    albumBrowseObjects.insert(m_spAlbumBrowse, this);
}

AlbumBrowse::~AlbumBrowse()
{
    clearData();

    sp_albumbrowse_release(m_spAlbumBrowse);
}

QStringList AlbumBrowse::copyrights() const
{
    return m_copyrights;
}

QList<Track*> AlbumBrowse::tracks() const
{
    return m_tracks;
}

Artist* AlbumBrowse::artist() const
{
    return m_artist;
}

QString AlbumBrowse::review() const
{
    return m_review;
}

bool AlbumBrowse::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        //Album browse complete
        albumBrowseObjects.remove(m_spAlbumBrowse);
        loadResults();
        emit completed();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void AlbumBrowse::clearData()
{
    qDeleteAll(m_tracks);
    m_tracks.clear();

    delete m_artist;
    m_artist = nullptr;
}

void AlbumBrowse::loadResults()
{
    clearData();

    QStringList copyrights;
    qint32 numCopyrights = sp_albumbrowse_num_copyrights(m_spAlbumBrowse);
    for(qint32 i=0 ; i<numCopyrights ; ++i) {
        copyrights.append(QString::fromUtf8(sp_albumbrowse_copyright(m_spAlbumBrowse, i)));
    }

    m_review = QString::fromUtf8(sp_albumbrowse_review(m_spAlbumBrowse));

    qint32 numTracks = sp_albumbrowse_num_tracks(m_spAlbumBrowse);
    for(qint32 i=0 ; i<numTracks ; ++i) {
        m_tracks.append(new Track(sp_albumbrowse_track(m_spAlbumBrowse, i)));
    }

    m_artist = new Artist(sp_albumbrowse_artist(m_spAlbumBrowse));
}
