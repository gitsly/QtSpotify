#include <QtSpotify/Core/spotify.h>

namespace QtSpotify {

Spotify::Spotify()
{

}

Spotify::~Spotify()
{

}

Spotify& Spotify::instance()
{
    static Spotify inst;
    return inst;
}

User* Spotify::user() const
{
    return m_user.get();
}

}
