#ifndef QTSPOTIFY_USER_H_
#define QTSPOTIFY_USER_H_

#include <QtSpotify/Core/global.h>

#include <QtCore/QObject>

#include <memory>

struct sp_user;

namespace QtSpotify {

class QTS_EXPORT User : public QObject
{

    Q_OBJECT

public:

    explicit User(sp_user* user);
    virtual ~User();

private:

    std::shared_ptr<sp_user> m_spUser;

};

}

#endif
