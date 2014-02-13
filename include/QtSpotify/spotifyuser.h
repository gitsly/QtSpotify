#ifndef QTSPOTIFY_SPOTIFYUSER_H
#define QTSPOTIFY_SPOTIFYUSER_H

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <libspotify/api.h>

enum class UserRelation {
    Unknown = SP_RELATION_TYPE_UNKNOWN,
    None = SP_RELATION_TYPE_NONE,
    Unidirectional = SP_RELATION_TYPE_UNIDIRECTIONAL,
    Bidirectional = SP_RELATION_TYPE_BIDIRECTIONAL
};

class QTSPOTIFYSHARED_EXPORT SpotifyUser : public QObject
{

public:

    SpotifyUser(sp_user* user);
    ~SpotifyUser();

    /*!
     * \brief Returns the display name of the currently logged in user, as opposed to their canonical name
     * \return The display name string
     */
    QString displayName() const;

    /*!
     * \brief Returns the canonical name of the currently logged in user, as opposed to their display name
     * \return The canonical name string
     */
    QString canonicalName() const;

    /*!
     * \brief Returns true if the user is loaded by libspotify
     * \return True if the user is loaded, false otherwise
     */
    bool loaded() const;

    sp_user* native() const;

    //Not used in libspotify *sadface*
    //UserRelation relation() const;

private slots:

    void updateData();

signals:

    /*!
     * \brief Emitted whenever the user loaded status changes
     * \param loaded True if the user is loaded by libspotify, false otherwise
     */
    void loadedChanged(bool loaded);

private:

    QString m_displayName;
    QString m_canonicalName;

    bool m_loaded;

    sp_user* m_spUser;
};

#endif
