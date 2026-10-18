#include "sound_player.h"

#include <QUrl>
#include <QtLogging>
#include <QtDebug>
#ifdef _WIN32
#    include <objbase.h>
#endif

namespace
{
QSoundEffect* createSoundEffect( QObject* parent, const char* source )
{
    auto effect = new QSoundEffect( parent );
    QObject::connect( effect,
                      &QSoundEffect::statusChanged,
                      effect,
                      [effect, source]()
                      {
                          if ( effect->status() == QSoundEffect::Error )
                          {
                              qWarning() << "Failed to load sound" << source;
                          }
                      } );
    effect->setSource( QUrl( source ) );
    return effect;
}
} // namespace

void SoundWorker::init( double volume )
{
#ifdef _WIN32
    //Windows needs COM
    const HRESULT hr = CoInitializeEx( nullptr, COINIT_MULTITHREADED );
    m_comInitialized = SUCCEEDED( hr );
    if ( !m_comInitialized )
    {
        qWarning() << "SoundWorker: CoInitializeEx failed" << Qt::hex << hr;
    }
#endif

    m_activationSoundEffect = createSoundEffect(
        this, "qrc:/qt/qml/AdvancedSettings/sounds/click.wav" );
    m_activationSoundEffect->setVolume( volume );

    m_focusChangedSoundEffect = createSoundEffect(
        this, "qrc:/qt/qml/AdvancedSettings/sounds/focus.wav" );
    m_focusChangedSoundEffect->setVolume( volume );

    m_alarm01SoundEffect = createSoundEffect(
        this, "qrc:/qt/qml/AdvancedSettings/sounds/alarm01.wav" );
    m_alarm01SoundEffect->setVolume( 1.0 );
}

void SoundWorker::cleanup()
{
    // Sound effects must go before COM is torn down on this thread.
    delete m_activationSoundEffect;
    delete m_focusChangedSoundEffect;
    delete m_alarm01SoundEffect;
    m_activationSoundEffect = nullptr;
    m_focusChangedSoundEffect = nullptr;
    m_alarm01SoundEffect = nullptr;
#ifdef _WIN32
    if ( m_comInitialized )
    {
        CoUninitialize();
        m_comInitialized = false;
    }
#endif
}

void SoundWorker::playActivation()
{
    m_activationSoundEffect->play();
}

void SoundWorker::playFocusChanged()
{
    m_focusChangedSoundEffect->play();
}

void SoundWorker::playAlarm01( bool loop )
{
    if ( m_alarm01SoundEffect->isPlaying() )
    {
        return;
    }
    m_alarm01SoundEffect->setLoopCount( loop ? QSoundEffect::Infinite : 1 );
    m_alarm01SoundEffect->play();
}

void SoundWorker::setVolume( double volume )
{
    m_activationSoundEffect->setVolume( volume );
    m_focusChangedSoundEffect->setVolume( volume );
    // MAYBE chaperone warning volume
}

void SoundWorker::setAlarm01Volume( double volume )
{
    m_alarm01SoundEffect->setVolume( volume );
}

void SoundWorker::cancelAlarm01()
{
    m_alarm01SoundEffect->stop();
}

SoundPlayer::SoundPlayer( double volume ) : m_worker( new SoundWorker )
{
    m_thread.setObjectName( "SoundPlayer" );
    m_worker->moveToThread( &m_thread );
    QObject::connect(
        &m_thread, &QThread::finished, m_worker, &QObject::deleteLater );
    m_thread.start();
    QMetaObject::invokeMethod( m_worker,
                               &SoundWorker::init,
                               Qt::QueuedConnection,
                               volume );
}

SoundPlayer::~SoundPlayer()
{
    shutdown();
}

void SoundPlayer::shutdown()
{
    if ( m_thread.isRunning() )
    {
        // Blocking so cleanup finishes on the worker thread before it exits.
        QMetaObject::invokeMethod(
            m_worker, &SoundWorker::cleanup, Qt::BlockingQueuedConnection );
        m_thread.quit();
        m_thread.wait();
    }
}

void SoundPlayer::playActivation()
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod(
        m_worker, &SoundWorker::playActivation, Qt::QueuedConnection );
}

void SoundPlayer::playFocusChanged()
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod(
        m_worker, &SoundWorker::playFocusChanged, Qt::QueuedConnection );
}

void SoundPlayer::playAlarm01( bool loop )
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod(
        m_worker, &SoundWorker::playAlarm01, Qt::QueuedConnection, loop );
}

void SoundPlayer::setVolume( double volume )
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod(
        m_worker, &SoundWorker::setVolume, Qt::QueuedConnection, volume );
}

void SoundPlayer::setAlarm01Volume( double volume )
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod( m_worker,
                               &SoundWorker::setAlarm01Volume,
                               Qt::QueuedConnection,
                               volume );
}

void SoundPlayer::cancelAlarm01()
{
    if ( !m_worker )
    {
        return;
    }
    QMetaObject::invokeMethod(
        m_worker, &SoundWorker::cancelAlarm01, Qt::QueuedConnection );
}
