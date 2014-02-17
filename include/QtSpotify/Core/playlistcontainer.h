#ifndef QTSPOTIFY_PLAYLISTCONTAINER_H_
#define QTSPOTIFY_PLAYLISTCONTAINER_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>

struct sp_playlistcontainer;
class Playlist;

class QTS_EXPORT PlaylistContainer : public QObject
{

    Q_OBJECT

public:

    PlaylistContainer(sp_playlistcontainer* pc);
    virtual ~PlaylistContainer();

    sp_playlistcontainer* native() const;

    void add(Playlist* playlist);

    void add(QList<Playlist*> playlist);

    void remove(Playlist* playlist);

    void move(Playlist* playlist, qint32 newPosition);

private:

    sp_playlistcontainer* m_spContainer;

};
#endif
