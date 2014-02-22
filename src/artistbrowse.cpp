#include <QtSpotify/Core/artistbrowse.h>

#include <QtSpotify/core/spotify.h>
#include <QtSpotify/Core/artist.h>
#include <QtSpotify/Core/album.h>
#include <QtSpotify/Core/track.h>
#include <QtSpotify/Core/playlist.h>

#include <QtCore/QCoreApplication>

QHash<sp_artistbrowse*, ArtistBrowse*> ArtistBrowse::artistBrowseObjects = QHash<sp_artistbrowse*, ArtistBrowse*>();

ArtistBrowse::ArtistBrowse(sp_artist* artist)
{
    m_spArtistBrowse = std::shared_ptr<sp_artistbrowse>(sp_artistbrowse_create(
                                                            Spotify::instance().session().get(),
                                                            artist,
                                                            SP_ARTISTBROWSE_FULL,
                                                            &ArtistBrowse::aristBrowseCompleteCallback,
                                                            nullptr), deleteArtistBrowse);
    sp_artistbrowse_add_ref(m_spArtistBrowse.get());
    artistBrowseObjects.insert(m_spArtistBrowse.get(), this);
}

ArtistBrowse::~ArtistBrowse()
{

}

QQmlListProperty<Track> ArtistBrowse::tracks()
{
    return QQmlListProperty<Track>(this, nullptr, &ArtistBrowse::tracksCountFunction, &ArtistBrowse::tracksAtFunction);
}

QQmlListProperty<Album> ArtistBrowse::albums()
{
    return QQmlListProperty<Album>(this, nullptr, &ArtistBrowse::albumsCountFunction, &ArtistBrowse::albumsAtFunction);
}

QQmlListProperty<Track> ArtistBrowse::topHits()
{
    return QQmlListProperty<Track>(this, nullptr, &ArtistBrowse::topHitsCountFunction, &ArtistBrowse::topHitsAtFunction);
}

QQmlListProperty<Artist> ArtistBrowse::similarArtists()
{
    return QQmlListProperty<Artist>(this, nullptr, &ArtistBrowse::similarArtistsCountFunction, &ArtistBrowse::similarArtistsAtFunction);
}

Artist* ArtistBrowse::artist() const
{
    return m_artist.get();
}

QString ArtistBrowse::biography() const
{
    return m_biography;
}

bool ArtistBrowse::event(QEvent* e)
{
    switch(e->type()) {
        case QEvent::User:
            onArtistBrowseComplete();
            e->accept();
            return true;
        default:
            return QObject::event(e);
    }
}

void ArtistBrowse::onArtistBrowseComplete()
{
    if(m_tracks.isEmpty()) {
        qint32 numTracks = sp_artistbrowse_num_tracks(m_spArtistBrowse.get());

        for(qint32 i=0 ; i<numTracks ; ++i) {
            m_tracks.append(std::make_shared<Track>(sp_artistbrowse_track(m_spArtistBrowse.get(), i)));
        }
    }

    if(m_topHits.isEmpty()) {
        qint32 numTopHits = sp_artistbrowse_num_tophit_tracks(m_spArtistBrowse.get());

        for(qint32 i=0 ; i<numTopHits ; ++i) {
            m_topHits.append(std::make_shared<Track>(sp_artistbrowse_tophit_track(m_spArtistBrowse.get(), i)));
        }
    }

    if(m_albums.isEmpty()) {
        qint32 numAlbums = sp_artistbrowse_num_albums(m_spArtistBrowse.get());

        for(qint32 i=0 ; i<numAlbums ; ++i) {
            m_albums.append(std::make_shared<Album>(sp_artistbrowse_album(m_spArtistBrowse.get(), i)));
        }
    }

    if(m_similarArtists.isEmpty()) {
        qint32 numSimilar = sp_artistbrowse_num_similar_artists(m_spArtistBrowse.get());

        for(qint32 i=0 ; i<numSimilar ; ++i) {
            m_similarArtists.append(std::make_shared<Artist>(sp_artistbrowse_similar_artist(m_spArtistBrowse.get(), i)));
        }
    }


    m_artist = std::make_shared<Artist>(sp_artistbrowse_artist(m_spArtistBrowse.get()));
    m_biography = QString::fromUtf8(sp_artistbrowse_biography(m_spArtistBrowse.get()));

    emit artistBrowseDataChanged();
}

void SP_CALLCONV ArtistBrowse::aristBrowseCompleteCallback(sp_artistbrowse* result, void*)
{
    QCoreApplication::postEvent(ArtistBrowse::artistBrowseObjects.value(result), new QEvent(QEvent::Type(QEvent::User)));
    ArtistBrowse::artistBrowseObjects.remove(result);
}

Track* ArtistBrowse::tracksAtFunction(QQmlListProperty<Track>* list, int index)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_tracks.at(index).get();
    }

    return nullptr;
}

qint32 ArtistBrowse::tracksCountFunction(QQmlListProperty<Track>* list)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_tracks.count();
    }

    return 0;
}

Track* ArtistBrowse::topHitsAtFunction(QQmlListProperty<Track>* list, int index)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_topHits.at(index).get();
    }

    return nullptr;
}

qint32 ArtistBrowse::topHitsCountFunction(QQmlListProperty<Track>* list)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_topHits.count();
    }

    return 0;
}

Album* ArtistBrowse::albumsAtFunction(QQmlListProperty<Album>* list, int index)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_albums.at(index).get();
    }

    return nullptr;
}

qint32 ArtistBrowse::albumsCountFunction(QQmlListProperty<Album>* list)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_albums.count();
    }

    return 0;
}

Artist* ArtistBrowse::similarArtistsAtFunction(QQmlListProperty<Artist>* list, int index)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_similarArtists.at(index).get();
    }

    return nullptr;
}

qint32 ArtistBrowse::similarArtistsCountFunction(QQmlListProperty<Artist>* list)
{
    auto artistBrowseObject = qobject_cast<ArtistBrowse*>(list->object);

    if(artistBrowseObject != nullptr) {
        return artistBrowseObject->m_similarArtists.count();
    }

    return 0;
}
