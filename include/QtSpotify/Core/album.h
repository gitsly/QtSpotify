#ifndef QTSPOTIFY_ALBUM_H_
#define QTSPOTIFY_ALBUM_H_

#include <QtSpotify/Core/global.h>

#include <libspotify/api.h>
#include <memory>

#include <QtCore/QObject>

namespace QtSpotify {

class Artist;

enum class AlbumType {
    Album = SP_ALBUMTYPE_ALBUM,
    Single = SP_ALBUMTYPE_SINGLE,
    Compilation = SP_ALBUMTYPE_COMPILATION,
    Unknown = SP_ALBUMTYPE_UNKNOWN
};

class QTS_EXPORT Album : public QObject
{

    Q_OBJECT

public:

    explicit Album(sp_album* album);
    virtual ~Album();

    Artist* artist() const;

    QString name() const;
    QString coverId() const;
    qint32 year() const;
    AlbumType type() const;

private slots:

    void onMetadataUpdated();

signals:

    void albumDataChanged();

private:

    std::shared_ptr<Artist> m_artist;
    QString m_name;
    QString m_coverId;
    qint32 m_year;
    AlbumType m_type;

    std::shared_ptr<sp_album> m_spAlbum;

};

}

#endif
