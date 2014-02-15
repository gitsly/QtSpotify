#include <QtSpotify/spotifyalbumbrowse.h>
#include <libspotify/api.h>

#include <QtSpotify/spotifysession.h>
#include <QtSpotify/albumbrowsecallbacks.h>

QHash<sp_albumbrowse*, SpotifyAlbumBrowse*> SpotifyAlbumBrowse::albumBrowseObjects = QHash<sp_albumbrowse*, SpotifyAlbumBrowse*>();

SpotifyAlbumBrowse::SpotifyAlbumBrowse(sp_album* album)
{
    m_spAlbumBrowse = sp_albumbrowse_create(SpotifySession::instance()->native(), album, albumBrowseCompleteCallback, nullptr);
    albumBrowseObjects.insert(m_spAlbumBrowse, this);

}

SpotifyAlbumBrowse::~SpotifyAlbumBrowse()
{

}

QStringList SpotifyAlbumBrowse::copyrights() const
{
    return m_copyrights;
}

QList<SpotifyTrack*> SpotifyAlbumBrowse::tracks() const
{
    return m_tracks;
}

SpotifyAlbum* SpotifyAlbumBrowse::album() const
{
    return m_album;
}

QString SpotifyAlbumBrowse::review() const
{
    return m_review;
}

qint32 SpotifyAlbumBrowse::serveTime() const
{
    return m_serveTime;
}

sp_albumbrowse* SpotifyAlbumBrowse::native() const
{
    return m_spAlbumBrowse;
}

bool SpotifyAlbumBrowse::event(QEvent* e)
{
    if(e->type() == QEvent::User) {
        updateData();
        emit completed();
        e->accept();
        return true;
    }

    QObject::event(e);
    return false;
}

void SpotifyAlbumBrowse::updateData()
{

}
