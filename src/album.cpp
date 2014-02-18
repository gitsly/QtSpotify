#include <QtSpotify/Album>
#include <QtSpotify/Session>

Album::Album(sp_album* album) :
    QObject(nullptr),
    m_name(""),
    m_coverId(""),
    m_year(0),
    m_type(AlbumType::Unknown)
{
    sp_album_add_ref(album);
    m_spAlbum = album;

    connect(Session::instance(), &Session::metadataUpdated, this, &Album::onMetadataUpdated);
    onMetadataUpdated();
}

Album::~Album()
{
    sp_album_release(m_spAlbum);
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

sp_album* Album::native() const
{
    return m_spAlbum;
}

void Album::onMetadataUpdated()
{
    bool updated = false;

    QString name = sp_album_name(m_spAlbum);
    qint32 year = sp_album_year(m_spAlbum);
    AlbumType type = AlbumType(sp_album_type(m_spAlbum));

    if(m_coverId.isEmpty()) {
        sp_link* link = sp_link_create_from_album_cover(m_spAlbum, SP_IMAGE_SIZE_NORMAL);
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
