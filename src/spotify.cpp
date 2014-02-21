#include <QtSpotify/Core/spotify.h>

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
