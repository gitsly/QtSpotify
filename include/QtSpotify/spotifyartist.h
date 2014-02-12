#ifndef QTSPOTIFY_SPOTIFYARTIST_H_
#define QTSPOTIFY_SPOTIFYARTIST_H_

#include <QtSpotify/qtspotify_global.h>
#include <QtCore/QObject>

struct sp_artist;

class QTSPOTIFYSHARED_EXPORT SpotifyArtist : public QObject
{

    Q_OBJECT

public:

    /*!
     * \brief SpotifyArtist
     * \param artist
     */
    SpotifyArtist(sp_artist* artist);
    ~SpotifyArtist();

    /*!
     * \brief Returns true if the artist is loaded by Libspotify
     * \return True if the artist is loaded, false otherwise
     */
    bool loaded() const;

    /*!
     * \brief Returns a string representation of the name of the artist
     * \return The artist's name
     */
    QString name() const;

    /*!
     * \brief The ID string of the portrait for the artist
     * \return The portrait ID
     */
    QString portraitId() const;

    /*!
     * \brief Returns the Libspotify native representation of the artist
     * \return The native representation
     */
    sp_artist* native() const;

private slots:

    void updateData();

signals:

    /*!
     * \brief Emitted whenever the loaded status of the artist changes
     * \param loaded True if the artist is loaded, false otherwise
     */
    void loadedChanged(bool loaded);

    /*!
     * \brief Emitted whenever the metadata of the artist is updated
     */
    void artistDataChanged();

private:

    QString m_name;
    QString m_portraitId;

    bool m_loaded;

    sp_artist* m_spArtist;
};

#endif
