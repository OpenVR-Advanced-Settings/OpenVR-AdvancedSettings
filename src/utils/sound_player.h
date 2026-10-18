#pragma once

#include <QObject>
#include <QPointer>
#include <QSoundEffect>
#include <QThread>

class SoundWorker : public QObject
{
    Q_OBJECT

public slots:
    void init( double volume );
    void playActivation();
    void playFocusChanged();
    void playAlarm01( bool loop );
    void setVolume( double volume );
    void setAlarm01Volume( double volume );
    void cancelAlarm01();
    void cleanup();

private:
    bool m_comInitialized = false;
    QSoundEffect* m_activationSoundEffect = nullptr;
    QSoundEffect* m_focusChangedSoundEffect = nullptr;
    QSoundEffect* m_alarm01SoundEffect = nullptr;
};

// Thread-safe front end, all calls are queued to the worker and return
// immediately.
class SoundPlayer
{
public:
    explicit SoundPlayer( double volume );
    ~SoundPlayer();

    SoundPlayer( const SoundPlayer& ) = delete;
    SoundPlayer& operator=( const SoundPlayer& ) = delete;

    void playActivation();
    void playFocusChanged();
    void playAlarm01( bool loop );
    void setVolume( double volume );
    void setAlarm01Volume( double volume );
    void cancelAlarm01();
    void shutdown();

private:
    QThread m_thread;
    QPointer<SoundWorker> m_worker;
};
