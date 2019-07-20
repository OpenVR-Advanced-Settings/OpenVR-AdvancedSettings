
#pragma once

#include <QObject>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

struct ReviveControllerProfile
{
    std::string profileName;
    int gripButtonMode;
    bool triggerAsGrip;
    float thumbDeadzone;
    float thumbRange;
    float touchPitch;
    float touchYaw;
    float touchRoll;
    float touchX;
    float touchY;
    float touchZ;
};

class ReviveTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int isOverlayInstalled READ isOverlayInstalled NOTIFY
                    isOverlayInstalledChanged )
    Q_PROPERTY( int gripButtonMode READ gripButtonMode WRITE setGripButtonMode
                    NOTIFY gripButtonModeChanged )
    Q_PROPERTY( int triggerAsGrip READ triggerAsGrip WRITE setTriggerAsGrip
                    NOTIFY triggerAsGripChanged )
    Q_PROPERTY( bool pixelsPerDisplayPixelOverrideEnabled READ
                    isPixelsPerDisplayPixelOverrideEnabled WRITE
                        setPixelsPerDisplayPixelOverrideEnabled NOTIFY
                            pixelsPerDisplayPixelOverrideEnabledChanged )
    Q_PROPERTY( float toggleDelay READ toggleDelay WRITE setToggleDelay NOTIFY
                    toggleDelayChanged )
    Q_PROPERTY(
        float pixelsPerDisplayPixelOverride READ pixelsPerDisplayPixelOverride
            WRITE setPixelsPerDisplayPixelOverride NOTIFY
                pixelsPerDisplayPixelOverrideChanged )
    Q_PROPERTY( float thumbDeadzone READ thumbDeadzone WRITE setThumbDeadzone
                    NOTIFY thumbDeadzoneChanged )
    Q_PROPERTY( float thumbRange READ thumbRange WRITE setThumbRange NOTIFY
                    thumbRangeChanged )
    Q_PROPERTY( float touchPitch READ touchPitch WRITE setTouchPitch NOTIFY
                    touchPitchChanged )
    Q_PROPERTY(
        float touchYaw READ touchYaw WRITE setTouchYaw NOTIFY touchYawChanged )
    Q_PROPERTY( float touchRoll READ touchRoll WRITE setTouchRoll NOTIFY
                    touchRollChanged )
    Q_PROPERTY( float touchX READ touchX WRITE setTouchX NOTIFY touchXChanged )
    Q_PROPERTY( float touchY READ touchY WRITE setTouchY NOTIFY touchYChanged )
    Q_PROPERTY( float touchZ READ touchZ WRITE setTouchZ NOTIFY touchZChanged )
    Q_PROPERTY( float piPlayerHeight READ piPlayerHeight WRITE setPiPlayerHeight
                    NOTIFY piPlayerHeightChanged )
    Q_PROPERTY( float piEyeHeight READ piEyeHeight WRITE setPiEyeHeight NOTIFY
                    piEyeHeightChanged )
    Q_PROPERTY( QString piUsername READ piUsername WRITE setPiUsername NOTIFY
                    piUsernameChanged )
    Q_PROPERTY(
        QString piName READ piName WRITE setPiName NOTIFY piNameChanged )
    Q_PROPERTY(
        int piGender READ piGender WRITE setPiGender NOTIFY piGenderChanged )

private:
    OverlayController* parent;

    bool m_isOverlayInstalled = false;
    int m_gripButtonMode = 0;
    float m_toggleDelay = 0.5f;
    bool m_triggerAsGrip = false;
    bool m_pixelsPerDisplayPixelOverrideEnabled = false;
    float m_pixelsPerDisplayPixelOverride = 1.0f;
    float m_thumbDeadzone = 0.3f;
    float m_thumbRange = 2.0f;
    float m_touchPitch = -28.0f;
    float m_touchYaw = 0.0f;
    float m_touchRoll = -14.0f;
    float m_touchX = 0.016f;
    float m_touchY = -0.036f;
    float m_touchZ = 0.016f;
    float m_piPlayerHeight = 1.778f;
    float m_piEyeHeight = 1.675f;
    QString m_piUsername = "";
    QString m_piName = "";
    int m_piGender = 0; // 0 .. Unknown, 1 .. Male, 2 .. Female

    unsigned settingsUpdateCounter = 0;

    std::vector<ReviveControllerProfile> controllerProfiles;

public:
    void initStage1( bool forceRevivePage );
    void initStage2( OverlayController* parent );

    void eventLoopTick();

    bool isOverlayInstalled() const;

    int gripButtonMode() const;
    bool triggerAsGrip() const;
    float toggleDelay() const;

    bool isPixelsPerDisplayPixelOverrideEnabled() const;
    float pixelsPerDisplayPixelOverride() const;

    float thumbDeadzone() const;
    float thumbRange() const;

    float touchPitch() const;
    float touchYaw() const;
    float touchRoll() const;
    float touchX() const;
    float touchY() const;
    float touchZ() const;

    float piPlayerHeight() const;
    float piEyeHeight() const;
    const QString& piUsername() const;
    const QString& piName() const;
    int piGender() const; // 0 .. Unknown, 1 .. Male, 2 .. Female

    Q_INVOKABLE float getCurrentHMDHeight();

    void reloadControllerProfiles();
    void saveControllerProfiles();

    Q_INVOKABLE unsigned getControllerProfileCount();
    Q_INVOKABLE QString getControllerProfileName( unsigned index );

public slots:
    void setGripButtonMode( int value, bool notify = true );
    void setTriggerAsGrip( bool value, bool notify = true );
    void setToggleDelay( float value, bool notify = true );

    void setPixelsPerDisplayPixelOverrideEnabled( bool value,
                                                  bool notify = true );
    void setPixelsPerDisplayPixelOverride( float value, bool notify = true );

    void setThumbDeadzone( float value, bool notify = true );
    void setThumbRange( float value, bool notify = true );

    void setTouchPitch( float value, bool notify = true );
    void setTouchYaw( float value, bool notify = true );
    void setTouchRoll( float value, bool notify = true );
    void setTouchX( float value, bool notify = true );
    void setTouchY( float value, bool notify = true );
    void setTouchZ( float value, bool notify = true );

    void setPiPlayerHeight( float value, bool notify = true );
    void setPiEyeHeight( float value, bool notify = true );
    void setPiUsername( const QString& value, bool notify = true );
    void setPiName( const QString& value, bool notify = true );
    void setPiGender( int value,
                      bool notify
                      = true ); // 0 .. Unknown, 1 .. Male, 2 .. Female

    void addControllerProfile( QString name );
    void applyControllerProfile( unsigned index );
    void deleteControllerProfile( unsigned index );

    void reset();

signals:
    void isOverlayInstalledChanged( bool value );

    void gripButtonModeChanged( int value );
    void triggerAsGripChanged( bool value );
    void toggleDelayChanged( float value );

    void pixelsPerDisplayPixelOverrideEnabledChanged( bool value );
    void pixelsPerDisplayPixelOverrideChanged( float value );

    void thumbDeadzoneChanged( float value );
    void thumbRangeChanged( float value );

    void touchPitchChanged( float value );
    void touchYawChanged( float value );
    void touchRollChanged( float value );
    void touchXChanged( float value );
    void touchYChanged( float value );
    void touchZChanged( float value );

    void piPlayerHeightChanged( float value );
    void piEyeHeightChanged( float value );
    void piUsernameChanged( const QString& value );
    void piNameChanged( const QString& value );
    void piGenderChanged( int value );

    void controllerProfilesUpdated();
};

} // namespace advsettings
