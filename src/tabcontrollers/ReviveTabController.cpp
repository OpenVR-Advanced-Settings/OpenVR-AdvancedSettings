#include "ReviveTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
constexpr auto appkey_overlay = "revive.dashboard.overlay";
constexpr auto section_revive = "revive";
constexpr auto key_pixelsPerDisplayPixel = "pixelsPerDisplayPixel";
constexpr auto key_toggleGrip = "ToggleGrip";
constexpr auto key_triggerAsGrip = "TriggerAsGrip";
constexpr auto key_toggleDelay = "ToggleDelay";
constexpr auto key_thumbDeadzone = "ThumbDeadzone";
constexpr auto key_thumbSensitivity = "ThumbSensitivity";
constexpr auto key_touchPitch = "TouchPitch";
constexpr auto key_touchYaw = "TouchYaw";
constexpr auto key_touchRoll = "TouchRoll";
constexpr auto key_touchX = "TouchX";
constexpr auto key_touchY = "TouchY";
constexpr auto key_touchZ = "TouchZ";
constexpr auto key_playerHeight = "PlayerHeight";
constexpr auto key_eyeHeight = "EyeHeight";
constexpr auto key_piUser = "User";
constexpr auto key_piName = "Name";
constexpr auto key_piGender = "Gender";

void ReviveTabController::initStage1( bool forceRevivePage )
{
    m_isOverlayInstalled
        = vr::VRApplications()->IsApplicationInstalled( appkey_overlay );
    if ( m_isOverlayInstalled || forceRevivePage )
    {
        vr::EVRSettingsError vrSettingsError;

        float valuef = vr::VRSettings()->GetFloat(
            section_revive, key_pixelsPerDisplayPixel, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::"
                           << key_pixelsPerDisplayPixel << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            if ( valuef == 0.0f )
            {
                m_pixelsPerDisplayPixelOverrideEnabled = false;
                m_pixelsPerDisplayPixelOverride = 1.0;
            }
            else
            {
                m_pixelsPerDisplayPixelOverrideEnabled = true;
                m_pixelsPerDisplayPixelOverride = valuef;
            }
        }

        int valuei = vr::VRSettings()->GetInt32(
            section_revive, key_toggleGrip, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_toggleGrip
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_gripButtonMode = valuei;
        }

        bool valueb = vr::VRSettings()->GetBool(
            section_revive, key_triggerAsGrip, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_triggerAsGrip
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_triggerAsGrip = valueb;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_toggleDelay, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_toggleDelay
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_toggleDelay = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_thumbDeadzone, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_thumbDeadzone
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_thumbDeadzone = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_thumbSensitivity, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::"
                           << key_thumbSensitivity << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_thumbRange = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchPitch, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchPitch
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchPitch = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchYaw, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchYaw
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchYaw = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchRoll, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchRoll
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchRoll = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchX, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchX
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchX = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchY, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchY
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchY = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_touchZ, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_touchZ
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_touchZ = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_playerHeight, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_playerHeight
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_piPlayerHeight = valuef;
        }

        valuef = vr::VRSettings()->GetFloat(
            section_revive, key_eyeHeight, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_eyeHeight
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_piEyeHeight = valuef;
        }

        char buffer[1024];
        vr::VRSettings()->GetString(
            section_revive, key_piUser, buffer, 1024, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_piUser
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_piUsername = buffer;
        }

        vr::VRSettings()->GetString(
            section_revive, key_piName, buffer, 1024, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_piName
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            m_piName = buffer;
        }

        vr::VRSettings()->GetString(
            section_revive, key_piGender, buffer, 1024, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \"revive::" << key_piGender
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        else
        {
            if ( QString( buffer ).compare( "male", Qt::CaseInsensitive ) == 0 )
            {
                m_piGender = 1;
            }
            else if ( QString( buffer ).compare( "female", Qt::CaseInsensitive )
                      == 0 )
            {
                m_piGender = 2;
            }
            else
            {
                m_piGender = 0;
            }
        }

        reloadControllerProfiles();
    }
}

void ReviveTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void ReviveTabController::eventLoopTick()
{
    if ( m_isOverlayInstalled
         || parent->m_settingsTabController.forceRevivePage() )
    {
        if ( settingsUpdateCounter >= k_reviveSettingsUpdateCounter )
        {
            if ( parent->isDashboardVisible() )
            {
                vr::EVRSettingsError vrSettingsError;

                float valuef
                    = vr::VRSettings()->GetFloat( section_revive,
                                                  key_pixelsPerDisplayPixel,
                                                  &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    if ( valuef == 0.0f )
                    {
                        setPixelsPerDisplayPixelOverrideEnabled( false );
                    }
                    else
                    {
                        setPixelsPerDisplayPixelOverrideEnabled( true );
                        setPixelsPerDisplayPixelOverride( valuef );
                    }
                }

                int valuei = vr::VRSettings()->GetInt32(
                    section_revive, key_toggleGrip, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setGripButtonMode( valuei );
                }

                bool valueb = vr::VRSettings()->GetBool(
                    section_revive, key_triggerAsGrip, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTriggerAsGrip( valueb );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_toggleDelay, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setToggleDelay( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_thumbDeadzone, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setThumbDeadzone( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_thumbSensitivity, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setThumbRange( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchPitch, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchPitch( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchYaw, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchYaw( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchRoll, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchRoll( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchX, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchX( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchY, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchY( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_touchZ, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setTouchZ( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_playerHeight, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setPiPlayerHeight( valuef );
                }

                valuef = vr::VRSettings()->GetFloat(
                    section_revive, key_eyeHeight, &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setPiEyeHeight( valuef );
                }

                char buffer[1024];
                vr::VRSettings()->GetString( section_revive,
                                             key_piUser,
                                             buffer,
                                             1024,
                                             &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setPiUsername( buffer );
                }

                vr::VRSettings()->GetString( section_revive,
                                             key_piName,
                                             buffer,
                                             1024,
                                             &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    setPiName( buffer );
                }

                vr::VRSettings()->GetString( section_revive,
                                             key_piGender,
                                             buffer,
                                             1024,
                                             &vrSettingsError );
                if ( vrSettingsError == vr::VRSettingsError_None )
                {
                    if ( QString( buffer ).compare( "male",
                                                    Qt::CaseInsensitive )
                         == 0 )
                    {
                        valuei = 1;
                    }
                    else if ( QString( buffer ).compare( "female",
                                                         Qt::CaseInsensitive )
                              == 0 )
                    {
                        valuei = 2;
                    }
                    else
                    {
                        valuei = 0;
                    }
                    setPiGender( valuei );
                }
            }
            settingsUpdateCounter = 0;
        }
        else
        {
            settingsUpdateCounter++;
        }
    }
}

bool ReviveTabController::isOverlayInstalled() const
{
    return m_isOverlayInstalled;
}

int ReviveTabController::gripButtonMode() const
{
    return m_gripButtonMode;
}

bool ReviveTabController::triggerAsGrip() const
{
    return m_triggerAsGrip;
}

float ReviveTabController::toggleDelay() const
{
    return m_toggleDelay;
}

bool ReviveTabController::isPixelsPerDisplayPixelOverrideEnabled() const
{
    return m_pixelsPerDisplayPixelOverrideEnabled;
}

float ReviveTabController::pixelsPerDisplayPixelOverride() const
{
    return m_pixelsPerDisplayPixelOverride;
}

float ReviveTabController::thumbDeadzone() const
{
    return m_thumbDeadzone;
}

float ReviveTabController::thumbRange() const
{
    return m_thumbRange;
}

float ReviveTabController::touchPitch() const
{
    return m_touchPitch;
}

float ReviveTabController::touchYaw() const
{
    return m_touchYaw;
}

float ReviveTabController::touchRoll() const
{
    return m_touchRoll;
}

float ReviveTabController::touchX() const
{
    return m_touchX;
}

float ReviveTabController::touchY() const
{
    return m_touchY;
}

float ReviveTabController::touchZ() const
{
    return m_touchZ;
}

float ReviveTabController::piPlayerHeight() const
{
    return m_piPlayerHeight;
}

float ReviveTabController::piEyeHeight() const
{
    return m_piEyeHeight;
}

const QString& ReviveTabController::piUsername() const
{
    return m_piUsername;
}

const QString& ReviveTabController::piName() const
{
    return m_piName;
}

int ReviveTabController::piGender() const
{
    return m_piGender;
}

void ReviveTabController::setGripButtonMode( int value, bool notify )
{
    if ( m_gripButtonMode != value )
    {
        m_gripButtonMode = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetInt32( section_revive,
                                    key_toggleGrip,
                                    m_gripButtonMode,
                                    &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_toggleGrip
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit gripButtonModeChanged( m_gripButtonMode );
        }
    }
}

void ReviveTabController::setTriggerAsGrip( bool value, bool notify )
{
    if ( m_triggerAsGrip != value )
    {
        m_triggerAsGrip = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetBool( section_revive,
                                   key_triggerAsGrip,
                                   m_triggerAsGrip,
                                   &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_toggleGrip
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit triggerAsGripChanged( m_triggerAsGrip );
        }
    }
}

void ReviveTabController::setToggleDelay( float value, bool notify )
{
    if ( m_toggleDelay != value )
    {
        m_toggleDelay = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_toggleDelay, m_toggleDelay, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_toggleDelay
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit toggleDelayChanged( m_toggleDelay );
        }
    }
}

void ReviveTabController::setPixelsPerDisplayPixelOverrideEnabled( bool value,
                                                                   bool notify )
{
    if ( m_pixelsPerDisplayPixelOverrideEnabled != value )
    {
        m_pixelsPerDisplayPixelOverrideEnabled = value;
        if ( m_pixelsPerDisplayPixelOverrideEnabled )
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetFloat( section_revive,
                                        key_pixelsPerDisplayPixel,
                                        m_pixelsPerDisplayPixelOverride,
                                        &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not set \"revive::" << key_pixelsPerDisplayPixel
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
        }
        else
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->RemoveKeyInSection(
                section_revive, key_pixelsPerDisplayPixel, &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not remove \"revive::"
                    << key_pixelsPerDisplayPixel << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit pixelsPerDisplayPixelOverrideEnabledChanged(
                m_pixelsPerDisplayPixelOverrideEnabled );
        }
    }
}

void ReviveTabController::setPixelsPerDisplayPixelOverride( float value,
                                                            bool notify )
{
    if ( m_pixelsPerDisplayPixelOverride != value )
    {
        m_pixelsPerDisplayPixelOverride = value;
        if ( m_pixelsPerDisplayPixelOverrideEnabled )
        {
            vr::EVRSettingsError vrSettingsError;
            vr::VRSettings()->SetFloat( section_revive,
                                        key_pixelsPerDisplayPixel,
                                        m_pixelsPerDisplayPixelOverride,
                                        &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not set \"revive::" << key_pixelsPerDisplayPixel
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            vr::VRSettings()->Sync();
        }
        if ( notify )
        {
            emit pixelsPerDisplayPixelOverrideChanged(
                m_pixelsPerDisplayPixelOverride );
        }
    }
}

void ReviveTabController::setThumbDeadzone( float value, bool notify )
{
    if ( m_thumbDeadzone != value )
    {
        m_thumbDeadzone = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat( section_revive,
                                    key_thumbDeadzone,
                                    m_thumbDeadzone,
                                    &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_thumbDeadzone
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit thumbDeadzoneChanged( m_thumbDeadzone );
        }
    }
}

void ReviveTabController::setThumbRange( float value, bool notify )
{
    if ( m_thumbRange != value )
    {
        m_thumbRange = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat( section_revive,
                                    key_thumbSensitivity,
                                    m_thumbRange,
                                    &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_thumbSensitivity
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit thumbRangeChanged( m_thumbRange );
        }
    }
}

void ReviveTabController::setTouchPitch( float value, bool notify )
{
    if ( m_touchPitch != value )
    {
        m_touchPitch = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchPitch, m_touchPitch, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchPitch
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchPitchChanged( m_touchPitch );
        }
    }
}

void ReviveTabController::setTouchYaw( float value, bool notify )
{
    if ( m_touchYaw != value )
    {
        m_touchYaw = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchYaw, m_touchYaw, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchYaw
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchYawChanged( m_touchYaw );
        }
    }
}

void ReviveTabController::setTouchRoll( float value, bool notify )
{
    if ( m_touchRoll != value )
    {
        m_touchRoll = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchRoll, m_touchRoll, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchRoll
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchRollChanged( m_touchRoll );
        }
    }
}

void ReviveTabController::setTouchX( float value, bool notify )
{
    if ( m_touchX != value )
    {
        m_touchX = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchX, m_touchX, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchX
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchXChanged( m_touchX );
        }
    }
}

void ReviveTabController::setTouchY( float value, bool notify )
{
    if ( m_touchY != value )
    {
        m_touchY = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchY, m_touchY, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchY
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchYChanged( m_touchY );
        }
    }
}

void ReviveTabController::setTouchZ( float value, bool notify )
{
    if ( m_touchZ != value )
    {
        m_touchZ = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_touchZ, m_touchZ, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_touchZ
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit touchZChanged( m_touchZ );
        }
    }
}

void ReviveTabController::setPiPlayerHeight( float value, bool notify )
{
    if ( m_piPlayerHeight != value )
    {
        m_piPlayerHeight = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat( section_revive,
                                    key_playerHeight,
                                    m_piPlayerHeight,
                                    &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_playerHeight
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit piPlayerHeightChanged( m_piPlayerHeight );
        }
    }
}

void ReviveTabController::setPiEyeHeight( float value, bool notify )
{
    if ( m_piEyeHeight != value )
    {
        m_piEyeHeight = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetFloat(
            section_revive, key_eyeHeight, m_piEyeHeight, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_eyeHeight
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit piEyeHeightChanged( m_piEyeHeight );
        }
    }
}

void ReviveTabController::setPiUsername( const QString& value, bool notify )
{
    if ( m_piUsername.compare( value ) != 0 )
    {
        m_piUsername = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetString( section_revive,
                                     key_piUser,
                                     m_piUsername.toStdString().c_str(),
                                     &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_piUser
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit piUsernameChanged( m_piUsername );
        }
    }
}

void ReviveTabController::setPiName( const QString& value, bool notify )
{
    if ( m_piName.compare( value ) != 0 )
    {
        m_piName = value;
        vr::EVRSettingsError vrSettingsError;
        vr::VRSettings()->SetString( section_revive,
                                     key_piName,
                                     m_piName.toStdString().c_str(),
                                     &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_piName
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit piNameChanged( m_piName );
        }
    }
}

void ReviveTabController::setPiGender( int value, bool notify )
{
    if ( m_piGender != value )
    {
        m_piGender = value;
        vr::EVRSettingsError vrSettingsError;
        const char* txt;
        switch ( value )
        {
        case 1:
            txt = static_cast<const char*>( "Male" );
            break;
        case 2:
            txt = static_cast<const char*>( "Female" );
            break;
        default:
            txt = static_cast<const char*>( "Unknown" );
            break;
        }
        vr::VRSettings()->SetString(
            section_revive, key_piGender, txt, &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not set \"revive::" << key_piGender
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit piGenderChanged( m_piGender );
        }
    }
}

void ReviveTabController::reset()
{
    vr::EVRSettingsError vrSettingsError;

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_pixelsPerDisplayPixel, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::"
                       << key_pixelsPerDisplayPixel << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_toggleGrip, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_toggleGrip
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_triggerAsGrip, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_triggerAsGrip
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_toggleDelay, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_toggleDelay
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_thumbDeadzone, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_thumbDeadzone
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_thumbSensitivity, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_thumbSensitivity
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchPitch, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchPitch
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchYaw, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchYaw
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchRoll, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchRoll
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchX, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchX
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchY, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchY
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        section_revive, key_touchZ, &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \"revive::" << key_touchZ
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->Sync();
    settingsUpdateCounter = 999; // Easiest way to get default values
}

float ReviveTabController::getCurrentHMDHeight()
{
    float hmdHeight = 0.0f;
    vr::TrackedDevicePose_t devicePoses[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0.0f,
        devicePoses,
        vr::k_unMaxTrackedDeviceCount );
    vr::TrackedDevicePose_t& hmdPose
        = devicePoses[vr::k_unTrackedDeviceIndex_Hmd];
    if ( hmdPose.eTrackingResult == vr::TrackingResult_Running_OK )
    {
        hmdHeight = hmdPose.mDeviceToAbsoluteTracking.m[1][3];
    }
    else
    {
        LOG( WARNING ) << "Error while getting HMD pose: Tracking result ("
                       << hmdPose.eTrackingResult
                       << ") is not TrackingResult_Running_OK";
    }
    return hmdHeight;
}

void ReviveTabController::reloadControllerProfiles()
{
    controllerProfiles.clear();
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "reviveSettings" );
    auto profileCount = settings->beginReadArray( "controllerProfiles" );
    for ( int i = 0; i < profileCount; i++ )
    {
        settings->setArrayIndex( i );
        controllerProfiles.emplace_back();
        auto& entry = controllerProfiles[static_cast<size_t>( i )];
        entry.profileName
            = settings->value( "profileName" ).toString().toStdString();
        entry.gripButtonMode = settings->value( "gripButtonMode", 0 ).toInt();
        entry.triggerAsGrip = settings->value( "triggerAsGrip", 0 ).toBool();
        entry.thumbDeadzone
            = settings->value( "thumbDeadzone", 0.3f ).toFloat();
        entry.thumbRange = settings->value( "thumbRange", 2.0f ).toFloat();
        entry.touchPitch = settings->value( "touchPitch", -28.0f ).toFloat();
        entry.touchYaw = settings->value( "touchYaw", 0.0f ).toFloat();
        entry.touchRoll = settings->value( "touchRoll", -14.0f ).toFloat();
        entry.touchX = settings->value( "touchX", 0.016f ).toFloat();
        entry.touchY = settings->value( "touchY", 0.0f ).toFloat();
        entry.touchZ = settings->value( "touchZ", 0.016f ).toFloat();
    }
    settings->endArray();
    settings->endGroup();
}

void ReviveTabController::saveControllerProfiles()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "reviveSettings" );
    settings->beginWriteArray( "controllerProfiles" );
    unsigned i = 0;
    for ( auto& p : controllerProfiles )
    {
        settings->setArrayIndex( static_cast<int>( i ) );
        settings->setValue( "profileName",
                            QString::fromStdString( p.profileName ) );
        settings->setValue( "gripButtonMode", p.gripButtonMode );
        settings->setValue( "triggerAsGrip", p.triggerAsGrip );
        settings->setValue( "thumbDeadzone", p.thumbDeadzone );
        settings->setValue( "thumbRange", p.thumbRange );
        settings->setValue( "touchPitch", p.touchPitch );
        settings->setValue( "touchYaw", p.touchYaw );
        settings->setValue( "touchRoll", p.touchRoll );
        settings->setValue( "touchX", p.touchX );
        settings->setValue( "touchY", p.touchY );
        settings->setValue( "touchZ", p.touchZ );
        i++;
    }
    settings->endArray();
    settings->endGroup();
}

Q_INVOKABLE unsigned ReviveTabController::getControllerProfileCount()
{
    return static_cast<unsigned int>( controllerProfiles.size() );
}

Q_INVOKABLE QString
    ReviveTabController::getControllerProfileName( unsigned index )
{
    if ( index >= controllerProfiles.size() )
    {
        return QString();
    }
    else
    {
        return QString::fromStdString( controllerProfiles[index].profileName );
    }
}

void ReviveTabController::addControllerProfile( QString name )
{
    ReviveControllerProfile* profile = nullptr;
    for ( auto& p : controllerProfiles )
    {
        if ( p.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &p;
            break;
        }
    }
    if ( !profile )
    {
        auto i = controllerProfiles.size();
        controllerProfiles.emplace_back();
        profile = &controllerProfiles[i];
    }
    profile->profileName = name.toStdString();
    profile->gripButtonMode = gripButtonMode();
    profile->triggerAsGrip = triggerAsGrip();
    profile->thumbDeadzone = thumbDeadzone();
    profile->thumbRange = thumbRange();
    profile->touchPitch = touchPitch();
    profile->touchYaw = touchYaw();
    profile->touchRoll = touchRoll();
    profile->touchX = touchX();
    profile->touchY = touchY();
    profile->touchZ = touchZ();
    saveControllerProfiles();
    OverlayController::appSettings()->sync();
    emit controllerProfilesUpdated();
}

void ReviveTabController::applyControllerProfile( unsigned index )
{
    if ( index < controllerProfiles.size() )
    {
        auto& profile = controllerProfiles[index];
        setGripButtonMode( profile.gripButtonMode );
        setTriggerAsGrip( profile.triggerAsGrip );
        setThumbDeadzone( profile.thumbDeadzone );
        setThumbRange( profile.thumbRange );
        setTouchPitch( profile.touchPitch );
        setTouchYaw( profile.touchYaw );
        setTouchRoll( profile.touchRoll );
        setTouchX( profile.touchX );
        setTouchY( profile.touchY );
        setTouchZ( profile.touchZ );
    }
}

void ReviveTabController::deleteControllerProfile( unsigned index )
{
    if ( index < controllerProfiles.size() )
    {
        auto pos = controllerProfiles.begin() + index;
        controllerProfiles.erase( pos );
        saveControllerProfiles();
        OverlayController::appSettings()->sync();
        emit controllerProfilesUpdated();
    }
}

} // namespace advsettings
