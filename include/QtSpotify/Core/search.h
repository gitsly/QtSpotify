#ifndef QTSPOTIFY_SEARCH_H_
#define QTSPOTIFY_SEARCH_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QEvent>

class Track;
class Album;
class Artist;
struct sp_search;

class QTS_EXPORT Search : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)


public:

    static QHash<sp_search*, Search*> searchObjects;

    Search();
    virtual ~Search();

    QString query() const;
    void setQuery(const QString& query);

    QList<Track*> trackResults() const;
    QList<Artist*> artistsResult() const;
    QList<Album*> albumsResult() const;

    Q_INVOKABLE void search();

protected:

    bool event(QEvent*);

signals:

    void queryChanged(const QString& query);

private:

    void loadResults();

    QString m_query;

    qint32 m_tracksLimit;
    qint32 m_artistsLimit;
    qint32 m_albumsLimit;

    qint32 m_tracksOffset;
    qint32 m_artistsOffset;
    qint32 m_albumsOffset;

    QList<Track*> m_tracksResult;
    QList<Artist*> m_artistsResult;
    QList<Album*> m_albumsResult;

    sp_search* m_spSearch;

};
#endif
