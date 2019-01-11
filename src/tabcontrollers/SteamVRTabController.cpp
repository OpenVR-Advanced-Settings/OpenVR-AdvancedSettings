#include "SteamVRTabController.h"
#include <QQuickWindow>
#include "../overlaycontroller.h"

// application namespace
namespace advsettings
{
void SteamVRTabController::initStage1()
{
    initMotionSmoothing();
    initSupersampleOverride();
    eventLoopTick();
    reloadSteamVRProfiles();
}

void SteamVRTabController::initStage2( OverlayController* var_parent,
                                       QQuickWindow* var_widget )
{
    this->parent = var_parent;
    this->widget = var_widget;
}

/*
name: eventLoopTick()
input: none
output: none

Description: Checks all settings, logs error If any, and re-synchs value between
Steam/OVR
*/
void SteamVRTabController::eventLoopTick()
{
    if ( settingsUpdateCounter >= 50 )
    {
        vr::EVRSettingsError vrSettingsError;
        // checks supersampling override and resynchs if necessry
        // also prints error if can't find.
        auto sso = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_SupersampleManualOverride_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowSupersampleOverride( sso );
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

/* -----------------------------------------*/
/*------------------------------------------*/
/*Profile Logic Functions*/

/*
name:

input: @name - QString name of profile (from QML UI)
       @includeSupersampling, @includeSupersampleFiltering,
@includeMotionSmoothing bool values to include associated settings

output: none

Description: Adds a steamVrProfile, name given by @name, and what settings to
save given by bool params

*/
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
        profile->supersampleOverride = m_allowSupersampleOverride;
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

/*
name: applySteamVRProfile

input: @index - unsigned index of selected profile
output: none

description: applies logic of saved settings, based on selected profile via
@index

*/
void SteamVRTabController::applySteamVRProfile( unsigned index )
{
    if ( index < steamvrProfiles.size() )
    {
        auto& profile = steamvrProfiles[index];
        if ( profile.includesSupersampling )
        {
            setAllowSupersampleOverride( profile.supersampleOverride );
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

/*
name: deleteSteamVRProfile

input:@index - unsigned index of a profile
output:none

description: Deletes the profile Given by @index

*/
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

/*
name: reloadSteamVrProfile

input: none
output: none

description: Reloads profiles from disk, and stores in steamvrProfiles

*/
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
            entry.supersampleOverride
                = settings->value( "supersamplingOverride", false ).toBool();
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
/*
name: saveSteamVRProfiles

input:none
output:none

description: saves the steamvrProfile object to disk.

*/

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
            settings->setValue( "supersamplingOverride",
                                p.supersampleOverride );
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
unsigned SteamVRTabController::getSteamVRProfileCount()
{
    return ( unsigned ) steamvrProfiles.size();
}

QString SteamVRTabController::getSteamVRProfileName( unsigned index )
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

/* -----------------------------------------*/
/*------------------------------------------*/
/*SuperSample (value) functions*/

/*
Name: setSuperSampling()

inputs: @value - float value to set supersampling to
        @notify - bool value to decide to update qml or not Default = true
outputs: none

Description:
Sets supersampling value based on @value and synchs with steam/OVR
Updates QML/UI based on @notify
*/

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
/* -----------------------------------------*/
/*------------------------------------------*/
/*Motion Smoothing boolean/toggle functions*/
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

/* -----------------------------------------*/
/*------------------------------------------*/
/*Supersample Filtering boolean/toggle functions*/

bool SteamVRTabController::allowSupersampleFiltering() const
{
    return m_allowSupersampleFiltering;
}

/*
Name setAllowSupersampleFiltering

input: @value - bool value to check against stored member variable
       @notify - bool value, default true. whether to update QML or not
output: none

description:
Sets the value of the supersample filtering option, and handles synchronization
with Steam/OVR
*/

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

/* -----------------------------------------*/
/*------------------------------------------*/
/*SuperSampling Override boolean/toggle functions*/

bool SteamVRTabController::allowSupersampleOverride() const
{
    return m_allowSupersampleOverride;
}

/*
Name setAllowSuperSampleOverride

input: @value - bool value of SSOverride
       @notify - bool value, default true. whether to update QML or not
output: none

description:
Sets the value of the SuperSample override, and handles synchronization with
Steam/OVR
*/
void SteamVRTabController::setAllowSupersampleOverride( bool value,
                                                        bool notify )
{
    if ( m_allowSupersampleOverride != value )
    {
        m_allowSupersampleOverride = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
            m_allowSupersampleOverride );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit allowSupersampleOverrideChanged( m_allowSupersampleOverride );
        }
    }
}

/*
Name: initSupersampleOverride

input: none
output: none

description: Gets the Value of the SuperSample Override from Steam/OpenVR.
Sets the member variable @m_allowSupersampleOverride
Adds error to log if unable to find variable.

OnError:
default value of FALSE for SS overide is initialized, if user has disabled
SS override on their end, they will have to toggle the variable twice to
get it to re-sync.
*/
void SteamVRTabController::initSupersampleOverride()
{
    bool temporary = false;
    vr::EVRSettingsError vrSettingsError;
    temporary = vr::VRSettings()->GetBool(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not get SuperSampling Override State \""
                       << vr::k_pch_SteamVR_SupersampleManualOverride_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
    vr::VRSettings()->Sync();
    setAllowSupersampleOverride( temporary, true );
}

/*------------------------------------------*/
/* -----------------------------------------*/

/*
name: reset

input: none
output: none

description: resets and resynchs the SteamVR keys (settings)
*/
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

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_SupersampleManualOverride_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_SupersampleManualOverride_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

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
        vr::k_pch_SteamVR_MotionSmoothing_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_MotionSmoothing_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->Sync();
    settingsUpdateCounter = 999; // Easiest way to get default values
}
/*
name:restartSteamVR

input: none
output: none

description: restarts steamvr

*/
void SteamVRTabController::restartSteamVR()
{
    QString cmd = QString( "cmd.exe /C restartvrserver.bat \"" )
                  + parent->getVRRuntimePathUrl().toLocalFile()
                  + QString( "\"" );
    LOG( INFO ) << "SteamVR Restart Command: " << cmd;
    QProcess::startDetached( cmd );
}

} // namespace advsettings
