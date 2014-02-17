#ifndef QTSPOTIFY_SESSION_H_
#define QTSPOTIFY_SESSION_H_

#include <QtSpotify/Core/global.h>
#include <QtCore/QObject>
#include <QtCore/QMutex>

#include <libspotify/api.h>

class User;

class QTS_EXPORT Session : public QObject
{

    Q_OBJECT

public:

    static Session* instance();
    sp_session* native() const;

    bool offlineMode() const;
    User* user() const;

signals:

    void metadataUpdated();

private:

    Session();
    Q_DISABLE_COPY(Session)

    bool m_offlineMode;
    User* m_user;

    sp_session* m_spSession;

};
#endif
