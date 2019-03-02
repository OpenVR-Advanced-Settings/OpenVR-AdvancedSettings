#include "PttController.h"
#include <QQuickWindow>
#include <QApplication>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
bool PttController::pttEnabled() const
{
    return m_pttEnabled;
}

bool PttController::pttActive() const
{
    return m_pttActive;
}

bool PttController::pttShowNotification() const
{
    return m_pttShowNotification;
}

bool PttController::pttLeftControllerEnabled() const
{
    return m_pttLeftControllerEnabled;
}

bool PttController::pttRightControllerEnabled() const
{
    return m_pttRightControllerEnabled;
}

void PttController::setPttControllerConfig( unsigned controller,
                                            QVariantList buttons,
                                            unsigned triggerMode,
                                            unsigned padMode,
                                            unsigned padAreas )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( controller < 2 )
    {
        uint64_t buttonMask = 0;
        for ( auto b : buttons )
        {
            buttonMask |= vr::ButtonMaskFromId(
                ( static_cast<vr::EVRButtonId>( b.toInt() ) ) );
        }
        m_pttControllerConfigs[controller].digitalButtons = buttons;
        m_pttControllerConfigs[controller].digitalButtonMask = buttonMask;
        m_pttControllerConfigs[controller].triggerMode = triggerMode;
        m_pttControllerConfigs[controller].touchpadMode = padMode;
        m_pttControllerConfigs[controller].touchpadAreas = padAreas;
        savePttConfig();
    }
}

void PttController::reloadPttProfiles()
{
    pttProfiles.clear();
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    auto profileCount = settings->beginReadArray( "pttProfiles" );
    for ( int i = 0; i < profileCount; i++ )
    {
        settings->setArrayIndex( i );
        pttProfiles.emplace_back();
        auto& entry = pttProfiles[static_cast<size_t>( i )];
        entry.profileName
            = settings->value( "profileName" ).toString().toStdString();
        entry.showNotification
            = settings->value( "showNotification", true ).toBool();
        entry.leftControllerEnabled
            = settings->value( "leftControllerEnabled", false ).toBool();
        entry.rightControllerEnabled
            = settings->value( "rightControllerEnabled", false ).toBool();
        entry.controllerConfigs[0].digitalButtons
            = settings->value( "digitalButtons_0" ).toList();
        uint64_t buttonMask = 0;
        for ( auto b : entry.controllerConfigs[0].digitalButtons )
        {
            buttonMask |= vr::ButtonMaskFromId(
                static_cast<vr::EVRButtonId>( b.toInt() ) );
        }
        entry.controllerConfigs[0].digitalButtonMask = buttonMask;
        entry.controllerConfigs[0].triggerMode
            = settings->value( "triggerMode_0", 0 ).toUInt();
        entry.controllerConfigs[0].touchpadMode
            = settings->value( "touchpadMode_0", 0 ).toUInt();
        entry.controllerConfigs[0].touchpadAreas
            = settings->value( "touchPadAreas_0", 0 ).toUInt();
        entry.controllerConfigs[1].digitalButtons
            = settings->value( "digitalButtons_1" ).toList();
        buttonMask = 0;
        for ( auto b : entry.controllerConfigs[1].digitalButtons )
        {
            buttonMask |= vr::ButtonMaskFromId(
                static_cast<vr::EVRButtonId>( b.toInt() ) );
        }
        entry.controllerConfigs[1].digitalButtonMask = buttonMask;
        entry.controllerConfigs[1].triggerMode
            = settings->value( "triggerMode_1", 0 ).toUInt();
        entry.controllerConfigs[1].touchpadMode
            = settings->value( "touchpadMode_1", 0 ).toUInt();
        entry.controllerConfigs[1].touchpadAreas
            = settings->value( "touchPadAreas_1", 0 ).toUInt();
    }
    settings->endArray();
    settings->endGroup();
}

void PttController::savePttProfiles()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->beginWriteArray( "pttProfiles" );
    unsigned i = 0;
    for ( auto& p : pttProfiles )
    {
        settings->setArrayIndex( static_cast<int>( i ) );
        settings->setValue( "profileName",
                            QString::fromStdString( p.profileName ) );
        settings->setValue( "showNotification", p.showNotification );
        settings->setValue( "leftControllerEnabled", p.leftControllerEnabled );
        settings->setValue( "rightControllerEnabled",
                            p.rightControllerEnabled );
        settings->setValue( "digitalButtons_0",
                            p.controllerConfigs[0].digitalButtons );
        settings->setValue( "triggerMode_0",
                            p.controllerConfigs[0].triggerMode );
        settings->setValue( "touchpadMode_0",
                            p.controllerConfigs[0].touchpadMode );
        settings->setValue( "touchPadAreas_0",
                            p.controllerConfigs[0].touchpadAreas );
        settings->setValue( "digitalButtons_1",
                            p.controllerConfigs[1].digitalButtons );
        settings->setValue( "triggerMode_1",
                            p.controllerConfigs[1].triggerMode );
        settings->setValue( "touchpadMode_1",
                            p.controllerConfigs[1].touchpadMode );
        settings->setValue( "touchPadAreas_1",
                            p.controllerConfigs[1].touchpadAreas );
        i++;
    }
    settings->endArray();
    settings->endGroup();
}

void PttController::reloadPttConfig()
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    setPttEnabled(
        settings->value( "pttEnabled", false ).toBool(), true, false );
    setPttLeftControllerEnabled(
        settings->value( "pttLeftControllerEnabled", false ).toBool(),
        true,
        false );
    setPttRightControllerEnabled(
        settings->value( "pttRightControllerEnabled", false ).toBool(),
        true,
        false );
    setPttShowNotification(
        settings->value( "pttShowNotification", true ).toBool(), true, false );
    m_pttControllerConfigs[0].digitalButtons
        = settings->value( "pttDigitalButtons_0" ).toList();
    uint64_t buttonMask = 0;
    for ( auto b : m_pttControllerConfigs[0].digitalButtons )
    {
        buttonMask |= vr::ButtonMaskFromId(
            static_cast<vr::EVRButtonId>( b.toInt() ) );
    }
    m_pttControllerConfigs[0].digitalButtonMask = buttonMask;
    m_pttControllerConfigs[0].triggerMode
        = settings->value( "pttTriggerMode_0", 0 ).toUInt();
    m_pttControllerConfigs[0].touchpadMode
        = settings->value( "pttTouchpadMode_0", 0 ).toUInt();
    m_pttControllerConfigs[0].touchpadAreas
        = settings->value( "pttTouchPadAreas_0", 0 ).toUInt();
    m_pttControllerConfigs[1].digitalButtons
        = settings->value( "pttDigitalButtons_1" ).toList();
    buttonMask = 0;
    for ( auto b : m_pttControllerConfigs[0].digitalButtons )
    {
        buttonMask |= vr::ButtonMaskFromId(
            static_cast<vr::EVRButtonId>( b.toInt() ) );
    }
    m_pttControllerConfigs[1].digitalButtonMask = buttonMask;
    m_pttControllerConfigs[1].triggerMode
        = settings->value( "pttTriggerMode_1", 0 ).toUInt();
    m_pttControllerConfigs[1].touchpadMode
        = settings->value( "pttTouchpadMode_1", 0 ).toUInt();
    m_pttControllerConfigs[1].touchpadAreas
        = settings->value( "pttTouchPadAreas_1", 0 ).toUInt();
    settings->endGroup();
}

void PttController::savePttConfig()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( getSettingsName() );
    settings->setValue( "pttEnabled", pttEnabled() );
    settings->setValue( "pttLeftControllerEnabled",
                        pttLeftControllerEnabled() );
    settings->setValue( "pttRightControllerEnabled",
                        pttRightControllerEnabled() );
    settings->setValue( "pttShowNotification", pttShowNotification() );
    settings->setValue( "pttDigitalButtons_0",
                        m_pttControllerConfigs[0].digitalButtons );
    settings->setValue( "pttTriggerMode_0",
                        m_pttControllerConfigs[0].triggerMode );
    settings->setValue( "pttTouchpadMode_0",
                        m_pttControllerConfigs[0].touchpadMode );
    settings->setValue( "pttTouchPadAreas_0",
                        m_pttControllerConfigs[0].touchpadAreas );
    settings->setValue( "pttDigitalButtons_1",
                        m_pttControllerConfigs[1].digitalButtons );
    settings->setValue( "pttTriggerMode_1",
                        m_pttControllerConfigs[1].triggerMode );
    settings->setValue( "pttTouchpadMode_1",
                        m_pttControllerConfigs[1].touchpadMode );
    settings->setValue( "pttTouchPadAreas_1",
                        m_pttControllerConfigs[1].touchpadAreas );
    settings->endGroup();
}

void PttController::checkPttStatus()
{
    // PTT Controls are now handled in the overlaycontroller using public
    // functions.
}

void PttController::startPtt()
{
    m_pttActive = true;
    onPttStart();
    if ( m_pttShowNotification
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->ShowOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void PttController::stopPtt()
{
    m_pttActive = false;
    onPttStop();
    if ( m_pttShowNotification
         && getNotificationOverlayHandle() != vr::k_ulOverlayHandleInvalid )
    {
        vr::VROverlay()->HideOverlay( getNotificationOverlayHandle() );
    }
    emit pttActiveChanged( m_pttActive );
}

void PttController::addPttProfile( QString name )
{
    PttProfile* profile = nullptr;
    for ( auto& p : pttProfiles )
    {
        if ( p.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &p;
            break;
        }
    }
    if ( !profile )
    {
        auto i = pttProfiles.size();
        pttProfiles.emplace_back();
        profile = &pttProfiles[i];
    }
    profile->profileName = name.toStdString();
    profile->showNotification = m_pttShowNotification;
    profile->leftControllerEnabled = m_pttLeftControllerEnabled;
    profile->rightControllerEnabled = m_pttRightControllerEnabled;
    profile->controllerConfigs[0].digitalButtons
        = m_pttControllerConfigs[0].digitalButtons;
    profile->controllerConfigs[0].digitalButtonMask
        = m_pttControllerConfigs[0].digitalButtonMask;
    profile->controllerConfigs[0].triggerMode
        = m_pttControllerConfigs[0].triggerMode;
    profile->controllerConfigs[0].touchpadMode
        = m_pttControllerConfigs[0].touchpadMode;
    profile->controllerConfigs[0].touchpadAreas
        = m_pttControllerConfigs[0].touchpadAreas;
    profile->controllerConfigs[1].digitalButtons
        = m_pttControllerConfigs[1].digitalButtons;
    profile->controllerConfigs[1].digitalButtonMask
        = m_pttControllerConfigs[1].digitalButtonMask;
    profile->controllerConfigs[1].triggerMode
        = m_pttControllerConfigs[1].triggerMode;
    profile->controllerConfigs[1].touchpadMode
        = m_pttControllerConfigs[1].touchpadMode;
    profile->controllerConfigs[1].touchpadAreas
        = m_pttControllerConfigs[1].touchpadAreas;
    savePttProfiles();
    OverlayController::appSettings()->sync();
    emit pttProfilesUpdated();
    emit pttProfileAdded();
}

void PttController::applyPttProfile( unsigned index )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( index < pttProfiles.size() )
    {
        auto& profile = pttProfiles[index];
        setPttShowNotification( profile.showNotification );
        setPttLeftControllerEnabled( profile.leftControllerEnabled );
        setPttRightControllerEnabled( profile.rightControllerEnabled );
        m_pttControllerConfigs[0].digitalButtons
            = profile.controllerConfigs[0].digitalButtons;
        m_pttControllerConfigs[0].digitalButtonMask
            = profile.controllerConfigs[0].digitalButtonMask;
        m_pttControllerConfigs[0].triggerMode
            = profile.controllerConfigs[0].triggerMode;
        m_pttControllerConfigs[0].touchpadMode
            = profile.controllerConfigs[0].touchpadMode;
        m_pttControllerConfigs[0].touchpadAreas
            = profile.controllerConfigs[0].touchpadAreas;
        m_pttControllerConfigs[1].digitalButtons
            = profile.controllerConfigs[1].digitalButtons;
        m_pttControllerConfigs[1].digitalButtonMask
            = profile.controllerConfigs[1].digitalButtonMask;
        m_pttControllerConfigs[1].triggerMode
            = profile.controllerConfigs[1].triggerMode;
        m_pttControllerConfigs[1].touchpadMode
            = profile.controllerConfigs[1].touchpadMode;
        m_pttControllerConfigs[1].touchpadAreas
            = profile.controllerConfigs[1].touchpadAreas;
    }
}

void PttController::deletePttProfile( unsigned index )
{
    if ( index < pttProfiles.size() )
    {
        auto pos = pttProfiles.begin() + index;
        pttProfiles.erase( pos );
        savePttProfiles();
        OverlayController::appSettings()->sync();
        emit pttProfilesUpdated();
    }
}

QVariantList PttController::pttDigitalButtons( unsigned controller )
{
    if ( controller < 2 )
    {
        return m_pttControllerConfigs[controller].digitalButtons;
    }
    return QVariantList();
}

unsigned long PttController::pttDigitalButtonMask( unsigned controller )
{
    if ( controller < 2 )
    {
        return static_cast<unsigned long>(
            m_pttControllerConfigs[controller].digitalButtonMask );
    }
    return 0;
}

unsigned PttController::pttTouchpadMode( unsigned controller )
{
    if ( controller < 2 )
    {
        return m_pttControllerConfigs[controller].touchpadMode;
    }
    return 0;
}

unsigned PttController::pttTriggerMode( unsigned controller )
{
    if ( controller < 2 )
    {
        return m_pttControllerConfigs[controller].triggerMode;
    }
    return 0;
}

unsigned PttController::pttTouchpadArea( unsigned controller )
{
    if ( controller < 2 )
    {
        return m_pttControllerConfigs[controller].touchpadAreas;
    }
    return 0;
}

unsigned PttController::getPttProfileCount()
{
    return static_cast<unsigned int>( pttProfiles.size() );
}

QString PttController::getPttProfileName( unsigned index )
{
    if ( index < pttProfiles.size() )
    {
        return QString::fromStdString( pttProfiles[index].profileName );
    }
    return "";
}

void PttController::setPttEnabled( bool value, bool notify, bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttEnabled )
    {
        m_pttEnabled = value;
        if ( value )
        {
            onPttEnabled();
        }
        else
        {
            onPttDisabled();
        }
        if ( notify )
        {
            emit pttEnabledChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

void PttController::setPttShowNotification( bool value, bool notify, bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttShowNotification )
    {
        m_pttShowNotification = value;
        if ( notify )
        {
            emit pttShowNotificationChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

void PttController::setPttLeftControllerEnabled( bool value,
                                                 bool notify,
                                                 bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttLeftControllerEnabled )
    {
        m_pttLeftControllerEnabled = value;
        if ( notify )
        {
            emit pttLeftControllerEnabledChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

void PttController::setPttRightControllerEnabled( bool value,
                                                  bool notify,
                                                  bool save )
{
    std::lock_guard<std::recursive_mutex> lock( eventLoopMutex );
    if ( value != m_pttRightControllerEnabled )
    {
        m_pttRightControllerEnabled = value;
        if ( notify )
        {
            emit pttRightControllerEnabledChanged( value );
        }
        if ( save )
        {
            savePttConfig();
        }
    }
}

void logControllerState( const vr::VRControllerState_t& state,
                         const std::string& prefix )
{
    if ( state.ulButtonPressed
         & vr::ButtonMaskFromId( vr::k_EButton_ApplicationMenu ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_ApplicationMenu )
                    << " pressed";
    }
    if ( state.ulButtonTouched
         & vr::ButtonMaskFromId( vr::k_EButton_ApplicationMenu ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_ApplicationMenu )
                    << " touched";
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Grip ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Grip )
                    << " pressed";
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Grip ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Grip )
                    << " touched";
    }
    if ( state.ulButtonPressed
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Left ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Left )
                    << " pressed";
    }
    if ( state.ulButtonTouched
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Left ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Left )
                    << " touched";
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_DPad_Up ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Up )
                    << " pressed";
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_DPad_Up ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Up )
                    << " touched";
    }
    if ( state.ulButtonPressed
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Right ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Right )
                    << " pressed";
    }
    if ( state.ulButtonTouched
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Right ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Right )
                    << " touched";
    }
    if ( state.ulButtonPressed
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Down ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Down )
                    << " pressed";
    }
    if ( state.ulButtonTouched
         & vr::ButtonMaskFromId( vr::k_EButton_DPad_Down ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_DPad_Down )
                    << " touched";
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_A ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_A )
                    << " pressed";
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_A ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_A )
                    << " touched";
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Axis0 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis0 )
                    << " pressed";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis0 )
                    << " x: " << state.rAxis[0].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Axis0 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis0 )
                    << " touched";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis0 )
                    << " x: " << state.rAxis[0].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Axis1 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis1 )
                    << " pressed";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis1 )
                    << " x: " << state.rAxis[1].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Axis1 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis1 )
                    << " touched";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis1 )
                    << " x: " << state.rAxis[1].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Axis2 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis2 )
                    << " pressed";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis2 )
                    << " x: " << state.rAxis[2].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Axis2 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis2 )
                    << " touched";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis2 )
                    << " x: " << state.rAxis[2].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Axis3 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis3 )
                    << " pressed";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis3 )
                    << " x: " << state.rAxis[3].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Axis3 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis3 )
                    << " touched";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis3 )
                    << " x: " << state.rAxis[3].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonPressed & vr::ButtonMaskFromId( vr::k_EButton_Axis4 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis4 )
                    << " pressed";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis4 )
                    << " x: " << state.rAxis[4].x
                    << "  y: " << state.rAxis[0].y;
    }
    if ( state.ulButtonTouched & vr::ButtonMaskFromId( vr::k_EButton_Axis4 ) )
    {
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis4 )
                    << " touched";
        LOG( INFO ) << prefix
                    << vr::VRSystem()->GetButtonIdNameFromEnum(
                           vr::k_EButton_Axis4 )
                    << " x: " << state.rAxis[4].x
                    << "  y: " << state.rAxis[0].y;
    }
}

} // namespace advsettings
