#ifndef QTSPOTIFY_PLAYLIST_H_
#define QTSPOTIFY_PLAYLIST_H_

#include <QtSpotify/Core/global.h>
#include <QtSpotify/Core/playlistcallbacks.h>
#include <QtSpotify/Core/track.h>

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtQml/QQmlListProperty>
#include <QtCore/QEvent>
#include <QtCore/QHash>

#include <libspotify/api.h>
#include <memory>

namespace QtSpotify {

class User;

class QTS_EXPORT Playlist : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Track> tracks READ tracks NOTIFY tracksChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool collaborative READ collaborative WRITE setCollaborative NOTIFY collaborativeChanged)
    Q_PROPERTY(qint32 totalDuration READ totalDuration NOTIFY totalDurationChanged)

public:

    static QHash<sp_playlist*, Playlist*> playlistObjects;

    /*!
     * \brief Constructs a playlist object from the native representation and loads all metadata
     * \param playlist The native representation of the playlist
     */
    explicit Playlist(sp_playlist* playlist = nullptr);

    /*!
     * \brief Destroys the playlist and all references to tracks, user etc.
     */
    virtual ~Playlist();

    /*!
     * \brief Returns the name of the playlist
     * \return The name of the playlist
     */
    QString name() const;

    /*!
     * \brief Sets the name of the playlist
     * \param name The new name to be set
     */
    void setName(const QString& name);

    /*!
     * \brief Returns the description for that playlist
     * \return The description
     */
    QString description() const;

    /*!
     * \brief Returns the total duration of the playlist in milliseconds
     * \return The duration in milliseconds
     */
    quint32 totalDuration() const;

    /*!
     * \brief Returns a pointer to the User owning the playlist
     * \return
     */
    QtSpotify::User* owner() const;

    /*!
     * \brief Returns the collaborative-status of the playlist
     * \return True if the playlist is collaborative, false otherwise
     */
    bool collaborative() const;

    /*!
     * \brief Sets the collaborative-status of the playlist
     * \param collaborative True if the playlist should be collaborative, false otherwise
     */
    void setCollaborative(bool collaborative);

    /*!
     * \brief Returns a list of tracks in the playlist
     * \return The track list
     */
    QQmlListProperty<Track> tracks();

private slots:

    void onMetadataUpdated();

protected:

    bool event(QEvent*);

signals:

    /*!
     * \brief Emitted whenever the playlist's list of tracks changes
     */
    void tracksChanged();

    /*!
     * \brief Emitted whenever the playlist's name changes
     * \param name The new name of the playlist
     */
    void nameChanged(const QString& name);

    /*!
     * \brief Emitted whenever the playlist's description changes
     * \param description The new description
     */
    void descriptionChanged(const QString& description);

    /*!
     * \brief Emitted whenever the playlist's collaborative-status changes
     * \param collaborative
     */
    void collaborativeChanged(bool collaborative);

    /*!
     * \brief Emitted whenever any of the playlist's metadata changes
     */
    void playlistDataChanged();

    /*!
     * \brief Emitted when the playlist's total duration changes
     * \param duration The new total duration in milliseconds
     */
    void totalDurationChanged(quint32 duration);

private:

    void onNameChanged(const QString& newName);
    void onCollaborativeChanged(bool collaborative);
    void onDescriptionChanged(const QString& description);
    void onTracksAdded(TracksAddedEvent* event);
    void onTracksMoved(TracksMovedEvent* event);
    void onTracksRemoved(TracksRemovedEvent* event);
    void onTrackCreatedChanged(TrackCreatedChangedEvent* event);
    void onTrackSeenChanged(TrackSeenChangedEvent* event);
    void onSubscribersChanged();

    QString m_name;
    QString m_description;
    bool m_collaborative;
    QList<std::shared_ptr<Track> > m_tracks;
    std::shared_ptr<QtSpotify::User> m_owner;
    std::shared_ptr<sp_playlist> m_spPlaylist;

    quint32 m_totalDuration;
    sp_playlist_callbacks* m_callbacks;

    static void tracksAppendFunction(QQmlListProperty<Track>* list, Track* track);
    static Track* tracksAtFunction(QQmlListProperty<Track>* list, int index);
    static void tracksClearFunction(QQmlListProperty<Track>* list);
    static qint32 tracksCountFunction(QQmlListProperty<Track>* list);

    static void SP_CALLCONV tracksAddedCallback(sp_playlist* playlist, sp_track* const* tracks, int numTracks, int position, void*);
    static void SP_CALLCONV tracksRemovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, void*);
    static void SP_CALLCONV tracksMovedCallback(sp_playlist* playlist, const int* tracks, int numTracks, int newPosition, void*);
    static void SP_CALLCONV playlistRenamedCallback(sp_playlist* playlist, void*);
    static void SP_CALLCONV playlistStateChangedCallback(sp_playlist* playlist, void*);
    static void SP_CALLCONV playlistUpdateInProgressCallback(sp_playlist* playlist, bool done, void*);
    static void SP_CALLCONV playlistMetadataUpdatedCallback(sp_playlist* playlist, void*);
    static void SP_CALLCONV trackCreatedChangedCallback(sp_playlist* playlist, int position, sp_user* user, int when, void*);
    static void SP_CALLCONV trackSeenChangedCallback(sp_playlist* playlist, int position, bool seen, void*);
    static void SP_CALLCONV descriptionChangedCallback(sp_playlist* playlist, const char* description, void*);
    static void SP_CALLCONV imageChangedCallback(sp_playlist* playlist, const byte* image, void*);
    static void SP_CALLCONV trackMessageChangedCallback(sp_playlist* playlist, int position, const char* message, void*);
    static void SP_CALLCONV subscribersChangedCallback(sp_playlist* playlist, void*);

};

}

#endif
