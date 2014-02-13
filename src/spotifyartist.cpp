#include <QtSpotify/spotifyartist.h>
#include <libspotify/api.h>

SpotifyArtist::SpotifyArtist(sp_artist *artist) :
    QObject(nullptr),
    m_name(""),
    m_portraitId(""),
    m_loaded(false)
{
    sp_artist_add_ref(artist);
    m_spArtist = artist;
}

SpotifyArtist::~SpotifyArtist()
{
    sp_artist_release(m_spArtist);
}

bool SpotifyArtist::loaded() const
{
    return sp_artist_is_loaded(m_spArtist);
}

QString SpotifyArtist::name() const
{
    return m_name;
}

QString SpotifyArtist::portraitId() const
{
    return m_portraitId;
}

sp_artist* SpotifyArtist::native() const
{
    return m_spArtist;
}

void SpotifyArtist::updateData()
{
    bool updated = false;

    bool loaded = sp_artist_is_loaded(m_spArtist);
    if(m_loaded != loaded) {
        m_loaded = loaded;
        emit loadedChanged(m_loaded);
        updated = true;
    }

    QString name = QString::fromUtf8(sp_artist_name(m_spArtist));

    const byte* portraitId = sp_artist_portrait(m_spArtist, SP_IMAGE_SIZE_NORMAL);
    if(portraitId != nullptr && m_portraitId.isEmpty()) {
        sp_link* link = sp_link_create_from_artist_portrait(m_spArtist, SP_IMAGE_SIZE_NORMAL);
        if(link != nullptr) {
            char buffer[255];
            qint32 urlSize = sp_link_as_string(link, &buffer[0], 255);
            m_portraitId = QString::fromUtf8(&buffer[0], urlSize);
            sp_link_release(link);
            updated = true;
        }
    }

    if(m_name != name) {
        m_name = name;
        updated = true;
    }

    if(updated) {
        emit artistDataChanged();
    }
}
