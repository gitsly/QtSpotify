#include <QtSpotify/Artist>
#include <libspotify/api.h>
#include <QtSpotify/Session>

Artist::Artist(sp_artist* artist) :
    QObject(nullptr),
    m_name(""),
    m_portraitId("")
{
    sp_artist_add_ref(artist);
    m_spArtist = artist;

    connect(Session::instance(), &Session::metadataUpdated, this, &Artist::onMetadataUpdated);
    onMetadataUpdated();
}

Artist::~Artist()
{
    sp_artist_release(m_spArtist);
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

    QString name = sp_artist_name(m_spArtist);

    if(m_portraitId.isEmpty()) {
        sp_link* link = sp_link_create_from_artist_portrait(m_spArtist, SP_IMAGE_SIZE_NORMAL);
        if(link != nullptr) {
            char buffer[255];
            qint32 uriSize = sp_link_as_string(link, &buffer[0], 255);
            m_portraitId = QString::fromUtf8(&buffer[0], uriSize);
            sp_link_release(link);
            updated = true;
        }
    }

    updated |= exchange(m_name, name);

    if(updated) {
        emit artistDataChanged();
    }
}
