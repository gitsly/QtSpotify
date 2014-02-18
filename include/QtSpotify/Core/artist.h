#ifndef QTSPOTIFY_ARTIST_H_
#define QTSPOTIFY_ARTIST_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QString>

struct sp_artist;

class QTS_EXPORT Artist : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY artistDataChanged)
    Q_PROPERTY(QString portraitId READ portraitId NOTIFY artistDataChanged)

public:

    Artist(sp_artist* artist);
    Q_DISABLE_COPY(Artist)
    virtual ~Artist();

    QString name() const;
    QString portraitId() const;

private slots:

    void onMetadataUpdated();

signals:

    void artistDataChanged();

private:

    QString m_name;
    QString m_portraitId;

    sp_artist* m_spArtist;

};
#endif
