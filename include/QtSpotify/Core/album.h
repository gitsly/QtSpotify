#ifndef QTSPOTIFY_ALBUM_H_
#define QTSPOTIFY_ALBUM_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <libspotify/api.h>

enum class AlbumType {
    Album = SP_ALBUMTYPE_ALBUM,
    Single = SP_ALBUMTYPE_SINGLE,
    Compilation = SP_ALBUMTYPE_COMPILATION,
    Unknown = SP_ALBUMTYPE_UNKNOWN
};

class QTS_EXPORT Album : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY albumDataChanged)
    Q_PROPERTY(QString coverId READ coverId NOTIFY albumDataChanged)
    Q_PROPERTY(qint32 year READ year NOTIFY albumDataChanged)
    Q_PROPERTY(AlbumType type READ type NOTIFY albumDataChanged)

    Q_ENUMS(AlbumType)

public:

    Album(sp_album* album);
    Q_DISABLE_COPY(Album)
    virtual ~Album();

    QString name() const;
    QString coverId() const;
    qint32 year() const;
    AlbumType type() const;

    sp_album* native() const;

private slots:

    void onMetadataUpdated();

signals:

    void albumDataChanged();

private:

    QString m_name;
    QString m_coverId;
    qint32 m_year;
    AlbumType m_type;

    sp_album* m_spAlbum;
};
#endif
