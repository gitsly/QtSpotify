#include <QtSpotify/Core/artist.h>
#include <QtSpotify/Core/deleters.h>

#include <QtSpotify/Spotify>

namespace QtSpotify {

Artist::Artist(sp_artist *artist)
{
    sp_artist_add_ref(artist);
    m_spArtist = std::shared_ptr<sp_artist>(artist, deleteArtist);

    connect(&Spotify::instance(), &Spotify::metadataUpdated, this, &Artist::onMetadataUpdated);
    connect(this, &Artist::nameChanged, this, &Artist::artistDataChanged);
    connect(this, &Artist::portraitIdChanged, this, &Artist::artistDataChanged);
}

Artist::~Artist()
{

}

QString Artist::name() const
{
    return m_name;
}

QString Artist::portraitId() const
{
    return m_portraitId;
}

void Artist::onMetadataUpdated()
{
    bool updated = false;

    QString name = QString::fromUtf8(sp_artist_name(m_spArtist.get()));

    if(m_portraitId.isEmpty()) {
        sp_link* link = sp_link_create_from_artist_portrait(m_spArtist.get(), SP_IMAGE_SIZE_NORMAL);

        if(link != nullptr) {
            char buffer[255];
            qint32 uriSize = sp_link_as_string(link, &buffer[0], 255);
            m_portraitId = QString::fromUtf8(&buffer[0], uriSize);
            sp_link_release(link);

            updated = true;
        }
    }

    if(exchange(m_name, name)) {
        updated = true;
    }

    if(updated) {
        emit artistDataChanged();
    }
}

}
