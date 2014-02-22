#ifndef QTSPOTIFY_ARTIST_H_
#define QTSPOTIFY_ARTIST_H_

#include <QtSpotify/Core/global.h>

#include <libspotify/api.h>
#include <memory>

#include <QtCore/QObject>

namespace QtSpotify {

class QTS_EXPORT Artist : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString portraitId READ portraitId NOTIFY portraitIdChanged)

public:

    explicit Artist(sp_artist* artist);
    virtual ~Artist();

    QString name() const;
    QString portraitId() const;

private slots:

    void onMetadataUpdated();

signals:

    void artistDataChanged();
    void nameChanged(const QString& name);
    void portraitIdChanged(const QString& coverId);

private:

    QString m_name;
    QString m_portraitId;

    std::shared_ptr<sp_artist> m_spArtist;

};

}

#endif
