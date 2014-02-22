#include <QtSpotify/Core/album.h>
#include <QtSpotify/Core/deleters.h>

#include <QtSpotify/Core/artist.h>

#include <QtSpotify/Spotify>

namespace QtSpotify {

Album::Album(sp_album *album)
{
    sp_album_add_ref(album);
    m_spAlbum = std::shared_ptr<sp_album>(album, deleteAlbum);

    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Album::onMetadataUpdated);
}

Album::~Album()
{

}

Artist* Album::artist() const
{
    return m_artist.get();
}

QString Album::name() const
{
    return m_name;
}

QString Album::coverId() const
{
    return m_coverId;
}

qint32 Album::year() const
{
    return m_year;
}

AlbumType Album::type() const
{
    return m_type;
}

void Album::onMetadataUpdated()
{
    bool updated = false;

    QString name = sp_album_name(m_spAlbum.get());
    qint32 year = sp_album_year(m_spAlbum.get());
    AlbumType type = AlbumType(sp_album_type(m_spAlbum.get()));

    if(m_artist == nullptr) {
        m_artist = std::make_shared<Artist>(sp_album_artist(m_spAlbum.get()));
        updated = true;
    }

    if(m_coverId.isEmpty()) {
        sp_link* link = sp_link_create_from_album_cover(m_spAlbum.get(), SP_IMAGE_SIZE_NORMAL);

        if(link != nullptr) {
            char buffer[255];
            qint32 uriSize = sp_link_as_string(link, &buffer[0], 255);
            m_coverId = QString::fromUtf8(&buffer[0], uriSize);
            sp_link_release(link);

            updated = true;
        }
    }

    updated |= exchange(m_name, name);
    updated |= exchange(m_year, year);
    updated |= exchange(m_type, type);

    if(updated) {
        emit albumDataChanged();
    }
}

}
