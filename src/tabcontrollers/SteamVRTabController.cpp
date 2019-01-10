#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

namespace vr
{
// this was removed from openvr_api.h in 1.0.8
static const char* const k_pch_SteamVR_RenderTargetMultiplier_Float
    = "renderTargetMultiplier";

// This was removed when updating from an unknown version to OpenVR
// version 1.1.3. This should probably be removed along with any features
// connected to it. Valve probably removed it for a reason.
static const char* const k_pch_SteamVR_AllowAsyncReprojection_Bool
    = "allowAsyncReprojection";
static const char* const k_pch_SteamVR_AllowReprojection_Bool
    = "allowInterleavedReprojection";
static const char* const k_pch_SteamVR_ForceReprojection_Bool
    = "forceReprojection";
} // namespace vr

// application namespace
namespace advsettings
{
// openvr's github repo hasn't been updated yet to reflect changes in beta
// 1477423729.
// static const char* vrsettings_steamvr_allowAsyncReprojection =
// "allowAsyncReprojection"; static const char*
// vrsettings_steamvr_allowInterleavedReprojection =
// "allowInterleavedReprojection";

// static const char* vrsettings_steamvr_supersampleScale = "supersampleScale";
// static const char* vrsettings_steamvr_allowSupersampleFiltering    =
// "allowSupersampleFiltering"; static const char*
// vrsettings_compositor_category = "compositor";

void SteamVRTabController::initStage1()
{
    initMotionSmoothing();
    eventLoopTick();
    reloadSteamVRProfiles();
}

void SteamVRTabController::initStage2( OverlayController* var_parent,
                                       QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
}

void SteamVRTabController::reloadSteamVRProfiles()
{
    steamvrProfiles.clear();
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "steamVRSettings" );
    auto profileCount = settings->beginReadArray( "steamVRProfiles" );
    for ( int i = 0; i < profileCount; i++ )
    {
        settings->setArrayIndex( i );
        steamvrProfiles.emplace_back();
        auto& entry = steamvrProfiles[i];
        entry.profileName
            = settings->value( "profileName" ).toString().toStdString();
        entry.includesSupersampling
            = settings->value( "includesSupersampling", false ).toBool();
        if ( entry.includesSupersampling )
        {
            entry.supersampling
                = settings->value( "supersampling", 1.0f ).toFloat();
        }
        entry.includesSupersampleFiltering
            = settings->value( "includesSupersampleFiltering", false ).toBool();
        if ( entry.includesSupersampleFiltering )
        {
            entry.supersampleFiltering
                = settings->value( "supersampleFiltering", true ).toBool();
        }
        entry.includesMotionSmoothing
            = settings->value( "includesMotionSmoothing", false ).toBool();
        if ( entry.includesMotionSmoothing )
        {
            entry.motionSmooth
                = settings->value( "motionSmooth", true ).toBool();
        }
    }
    settings->endArray();
    settings->endGroup();
}

void SteamVRTabController::saveSteamVRProfiles()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "steamVRSettings" );
    settings->beginWriteArray( "steamVRProfiles" );
    unsigned i = 0;
    for ( auto& p : steamvrProfiles )
    {
        settings->setArrayIndex( i );
        settings->setValue( "profileName",
                            QString::fromStdString( p.profileName ) );
        settings->setValue( "includesSupersampling", p.includesSupersampling );
        if ( p.includesSupersampling )
        {
            settings->setValue( "supersampling", p.supersampling );
        }
        settings->setValue( "includesSupersampleFiltering",
                            p.includesSupersampleFiltering );
        if ( p.includesSupersampleFiltering )
        {
            settings->setValue( "supersampleFiltering",
                                p.supersampleFiltering );
        }
        settings->setValue( "includesMotionSmoothing",
                            p.includesMotionSmoothing );
        if ( p.includesMotionSmoothing )
        {
            settings->setValue( "motionSmooth", p.motionSmooth );
        }

        i++;
    }
    settings->endArray();
    settings->endGroup();
}

Q_INVOKABLE unsigned SteamVRTabController::getSteamVRProfileCount()
{
    return ( unsigned ) steamvrProfiles.size();
}

Q_INVOKABLE QString
    SteamVRTabController::getSteamVRProfileName( unsigned index )
{
    if ( index >= steamvrProfiles.size() )
    {
        return QString();
    }
    else
    {
        return QString::fromStdString( steamvrProfiles[index].profileName );
    }
}
// TODO DISC does it make any sense to check these settings once every ~
// 1second. primary purpose seems to be to re-sync settings if changed via other
// means?
void SteamVRTabController::eventLoopTick()
{
    if ( settingsUpdateCounter >= 50 )
    {
        vr::EVRSettingsError vrSettingsError;
        // checks supersampling and re-synchs if necessary
        auto ss = vr::VRSettings()->GetFloat(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleScale_Float,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_SupersampleScale_Float
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
            if ( m_superSampling != 1.0 )
            {
                LOG( DEBUG ) << "OpenVR returns an error and we have a custom "
                                "supersampling value: "
                             << m_superSampling;
                setSuperSampling( 1.0 );
            }
        }
        else if ( m_superSampling != ss )
        {
            LOG( DEBUG ) << "OpenVR reports a changed supersampling value: "
                         << m_superSampling << " => " << ss;
            setSuperSampling( ss );
        }
        // checks if Supersampling filter is on and changes if it has been
        // changed elsewhere
        auto sf = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowSupersampleFiltering( sf );

        // Checks if Motion smoothing setting can be read and synchs adv
        // settings to steamvr/openvr
        auto ms
            = vr::VRSettings()->GetBool( vr::k_pch_SteamVR_Section,
                                         vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                         &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_MotionSmoothing_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setMotionSmoothing( ms );

        settingsUpdateCounter = 0;
    }
    else
    {
        settingsUpdateCounter++;
    }
}

void SteamVRTabController::setSuperSampling( float value, bool notify )
{
    bool override = false;
    if ( value <= 0.01f )
    {
        LOG( WARNING ) << "Encountered a supersampling value <= 0.01, setting "
                          "supersampling to 1.0";
        value = 1.0f;
        override = true;
    }
    if ( override || m_superSampling != value )
    {
        LOG( DEBUG ) << "Supersampling value changed: " << m_superSampling
                     << " => " << value;
        m_superSampling = value;
        vr::VRSettings()->SetFloat( vr::k_pch_SteamVR_Section,
                                    vr::k_pch_SteamVR_SupersampleScale_Float,
                                    m_superSampling );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit superSamplingChanged( m_superSampling );
        }
    }
}

float SteamVRTabController::superSampling() const
{
    return m_superSampling;
}

bool SteamVRTabController::motionSmoothing() const
{
    return m_motionSmoothing;
}

/*
Name: setMotionSmoothing

input: bool value, bool notify
output: none

@value - value to be passed to m_motionSMoothing
@notify - whether QML is informed of change (default true)

Description:
sets value of m_motionSmoothing AND applies changes in Steam/OpenVr
*/

void SteamVRTabController::setMotionSmoothing( bool value, bool notify )
{
    if ( m_motionSmoothing != value )
    {
        m_motionSmoothing = value;
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                   m_motionSmoothing );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit motionSmoothingChanged( m_motionSmoothing );
        }
    }
}

/*
Name: initMotionSmoothing

input: none
output: none

description: Gets the Value of Motion Smoothing from Steam/OpenVR.
Sets the member variable of motion smoothing for UI
Adds error to log if unable to find variable.

OnError:
default value of true for motion smoothing is initialized, if user has disabled
motion smoothing on their end, they will have to toggle the variable twice to
get it to re-sync.
*/
void SteamVRTabController::initMotionSmoothing()
{
    bool temporary = false;
    vr::EVRSettingsError vrSettingsError;
    temporary
        = vr::VRSettings()->GetBool( vr::k_pch_SteamVR_Section,
                                     vr::k_pch_SteamVR_MotionSmoothing_Bool,
                                     &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not get MotionSmoothing State \""
                       << vr::k_pch_SteamVR_MotionSmoothing_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    vr::VRSettings()->Sync();
    setMotionSmoothing( temporary, true );
}

bool SteamVRTabController::allowSupersampleFiltering() const
{
    return m_allowSupersampleFiltering;
}

void SteamVRTabController::setAllowSupersampleFiltering( bool value,
                                                         bool notify )
{
    if ( m_allowSupersampleFiltering != value )
    {
        m_allowSupersampleFiltering = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool,
            m_allowSupersampleFiltering );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit allowSupersampleFilteringChanged(
                m_allowSupersampleFiltering );
        }
    }
}

void SteamVRTabController::reset()
{
    vr::EVRSettingsError vrSettingsError;

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SupersampleScale_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_SupersampleScale_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->Sync();
    settingsUpdateCounter = 999; // Easiest way to get default values
}

void SteamVRTabController::restartSteamVR()
{
    QString cmd = QString( "cmd.exe /C restartvrserver.bat \"" )
                  + parent->getVRRuntimePathUrl().toLocalFile()
                  + QString( "\"" );
    LOG( INFO ) << "SteamVR Restart Command: " << cmd;
    QProcess::startDetached( cmd );
}

void SteamVRTabController::addSteamVRProfile( QString name,
                                              bool includeSupersampling,
                                              bool includeSupersampleFiltering,
                                              bool includeMotionSmoothing )
{
    SteamVRProfile* profile = nullptr;
    for ( auto& p : steamvrProfiles )
    {
        if ( p.profileName.compare( name.toStdString() ) == 0 )
        {
            profile = &p;
            break;
        }
    }
    if ( !profile )
    {
        auto i = steamvrProfiles.size();
        steamvrProfiles.emplace_back();
        profile = &steamvrProfiles[i];
    }
    profile->profileName = name.toStdString();
    profile->includesSupersampling = includeSupersampling;
    if ( includeSupersampling )
    {
        profile->supersampling = m_superSampling;
    }
    profile->includesSupersampleFiltering = includeSupersampleFiltering;
    if ( includeSupersampleFiltering )
    {
        profile->supersampleFiltering = m_allowSupersampleFiltering;
    }
    profile->includesMotionSmoothing = includeMotionSmoothing;
    if ( includeMotionSmoothing )
    {
        profile->motionSmooth = m_motionSmoothing;
    }
    saveSteamVRProfiles();
    OverlayController::appSettings()->sync();
    emit steamVRProfilesUpdated();
    emit steamVRProfileAdded();
}

void SteamVRTabController::applySteamVRProfile( unsigned index )
{
    if ( index < steamvrProfiles.size() )
    {
        auto& profile = steamvrProfiles[index];
        if ( profile.includesSupersampling )
        {
            setSuperSampling( profile.supersampling );
        }
        if ( profile.includesSupersampleFiltering )
        {
            setAllowSupersampleFiltering( profile.supersampleFiltering );
        }
        if ( profile.includesMotionSmoothing )
        {
            setMotionSmoothing( profile.motionSmooth );
        }
        vr::VRSettings()->Sync( true );
    }
}

void SteamVRTabController::deleteSteamVRProfile( unsigned index )
{
    if ( index < steamvrProfiles.size() )
    {
        auto pos = steamvrProfiles.begin() + index;
        steamvrProfiles.erase( pos );
        saveSteamVRProfiles();
        OverlayController::appSettings()->sync();
        emit steamVRProfilesUpdated();
    }
}

} // namespace advsettings
