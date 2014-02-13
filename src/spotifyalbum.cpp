#include <QtSpotify/spotifyalbum.h>
#include <QtSpotify/spotifysession.h>
#include <QtSpotify/spotifyartist.h>

SpotifyAlbum::SpotifyAlbum(sp_album *album) :
    QObject(nullptr),
    m_available(false),
    m_artist(nullptr),
    m_name(""),
    m_year(0),
    m_type(AlbumType::Unknown)
{
    sp_album_add_ref(album);
    m_spAlbum = album;

    connect(SpotifySession::instance(), &SpotifySession::metadataUpdated, this, &SpotifyAlbum::updateData);
    updateData();
}

SpotifyAlbum::~SpotifyAlbum()
{
    sp_album_release(m_spAlbum);
    delete m_artist;
}

bool SpotifyAlbum::available() const
{
    return m_available;
}

bool SpotifyAlbum::loaded() const
{
    return sp_album_is_loaded(m_spAlbum);
}

SpotifyArtist* SpotifyAlbum::artist() const
{
    return m_artist;
}

QString SpotifyAlbum::name() const
{
    return m_name;
}

QString SpotifyAlbum::coverId() const
{
    return m_coverId;
}

qint32 SpotifyAlbum::year() const
{
    return m_year;
}

AlbumType SpotifyAlbum::type() const
{
    return m_type;
}

void SpotifyAlbum::updateData()
{
    bool updated = false;

    bool loaded = sp_album_is_loaded(m_spAlbum);
    if(loaded != m_loaded) {
        m_loaded = loaded;
        emit loadedChanged(m_loaded);
    }

    QString name = QString::fromUtf8(sp_album_name(m_spAlbum));
    qint32 year = sp_album_year(m_spAlbum);
    AlbumType type = AlbumType(sp_album_type(m_spAlbum));
    bool available = sp_album_is_available(m_spAlbum);

    const byte* coverId = sp_album_cover(m_spAlbum, SP_IMAGE_SIZE_NORMAL);
    if(coverId != nullptr && m_coverId.isEmpty()) {
        sp_link* link = sp_link_create_from_album_cover(m_spAlbum, SP_IMAGE_SIZE_NORMAL);
        if(link != nullptr) {
            char buffer[255];
            qint32 urlSize = sp_link_as_string(link, &buffer[0], 255);
            m_coverId = QString::fromUtf8(&buffer[0], urlSize);
            sp_link_release(link);
            updated = true;
        }
    }

    if(m_available != available) {
        m_available = available;
        updated = true;
    }

    if(m_name != name) {
        m_name = name;
        updated = true;
    }

    if(m_year != year) {
        m_year = year;
        updated = true;
    }

    if(m_type != type) {
        m_type = type;
        updated = true;
    }

    if(m_artist == nullptr) {
        m_artist = new SpotifyArtist(sp_album_artist(m_spAlbum));
        updated = true;
    }

    if(updated) {
        emit albumDataChanged();
    }
}
