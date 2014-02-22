#include <QtSpotify/Core/albumbrowse.h>
#include <QtSpotify/Core/spotify.h>
#include <QtSpotify/Core/deleters.h>

#include <QtSpotify/Core/artist.h>
#include <QtSpotify/Core/album.h>
#include <QtSpotify/Core/track.h>
#include <QtSpotify/Core/playlist.h>

#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>

namespace QtSpotify {

QHash<sp_albumbrowse*, AlbumBrowse*> AlbumBrowse::albumBrowseObjects = QHash<sp_albumbrowse*, AlbumBrowse*>();

AlbumBrowse::AlbumBrowse(sp_album* album)
{
    m_spAlbumBrowse = std::shared_ptr<sp_albumbrowse>(
                sp_albumbrowse_create(Spotify::instance().session().get(), album, &AlbumBrowse::albumBrowseCompleteCallback, nullptr),
                deleteAlbumBrowse);
    sp_albumbrowse_add_ref(m_spAlbumBrowse.get());
    albumBrowseObjects.insert(m_spAlbumBrowse.get(), this);
}

AlbumBrowse::~AlbumBrowse()
{

}

QQmlListProperty<Track> AlbumBrowse::tracks()
{
    return QQmlListProperty<Track>(this, nullptr, &AlbumBrowse::tracksCountFunction, &AlbumBrowse::tracksAtFunction);
}

QString AlbumBrowse::review() const
{
    return m_review;
}

QStringList AlbumBrowse::copyrights() const
{
    return m_copyrights;
}

Album* AlbumBrowse::album() const
{
    return m_album.get();
}

Artist* AlbumBrowse::artist() const
{
    return m_artist.get();
}

bool AlbumBrowse::event(QEvent* e)
{
    switch(e->type()) {
        case QEvent::User:
            onAlbumBrowseComplete();
            e->accept();
            return true;
        default:
            return QObject::event(e);
    }
}

void AlbumBrowse::onAlbumBrowseComplete()
{
    QString review = QString::fromUtf8(sp_albumbrowse_review(m_spAlbumBrowse.get()));

    if(m_tracks.empty()) {
        qint32 numTracks = sp_albumbrowse_num_tracks(m_spAlbumBrowse.get());

        for(qint32 i=0 ; i<numTracks ; ++i) {
            std::shared_ptr<Track> track = std::make_shared<Track>(sp_albumbrowse_track(m_spAlbumBrowse.get(), i));
        }
    }

    if(m_copyrights.isEmpty()) {
        qint32 numCopyrights = sp_albumbrowse_num_copyrights(m_spAlbumBrowse.get());

        for(qint32 i=0 ; i<numCopyrights ; ++i) {
            QString copyright = QString::fromUtf8(sp_albumbrowse_copyright(m_spAlbumBrowse.get(), i));
            m_copyrights.append(copyright);
        }
    }

    m_album = std::make_shared<Album>(sp_albumbrowse_album(m_spAlbumBrowse.get()));
    m_artist = std::make_shared<Artist>(sp_albumbrowse_artist(m_spAlbumBrowse.get()));
}

void SP_CALLCONV AlbumBrowse::albumBrowseCompleteCallback(sp_albumbrowse* result, void*)
{
    QCoreApplication::postEvent(AlbumBrowse::albumBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
    AlbumBrowse::albumBrowseObjects.remove(result);
}

Track* AlbumBrowse::tracksAtFunction(QQmlListProperty<Track>* list, int index)
{
    auto albumBrowseObject = qobject_cast<AlbumBrowse*>(list->object);

    if(albumBrowseObject != nullptr) {
        return albumBrowseObject->m_tracks.at(index).get();
    }

    return nullptr;
}

qint32 AlbumBrowse::tracksCountFunction(QQmlListProperty<Track>* list)
{
    auto albumBrowseObject = qobject_cast<AlbumBrowse*>(list->object);

    if(albumBrowseObject != nullptr) {
        return albumBrowseObject->m_tracks.count();
    }

    return 0;
}

}
