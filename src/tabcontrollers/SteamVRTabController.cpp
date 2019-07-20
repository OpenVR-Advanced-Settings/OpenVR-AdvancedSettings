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

void SteamVRTabController::initStage2( OverlayController* var_parent )
{
    this->parent = var_parent;
}

void SteamVRTabController::eventLoopTick()
{
    if ( settingsUpdateCounter >= k_steamVrSettingsUpdateCounter )
    {
        if ( parent->isDashboardVisible() )
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
                LOG( WARNING )
                    << "Could not read \""
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
                LOG( WARNING )
                    << "Could not read \""
                    << vr::k_pch_SteamVR_SupersampleScale_Float
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
                if ( m_superSampling != 1.0f )
                {
                    LOG( DEBUG )
                        << "OpenVR returns an error and we have a custom "
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
                LOG( WARNING )
                    << "Could not read \""
                    << vr::k_pch_SteamVR_AllowSupersampleFiltering_Bool
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            setAllowSupersampleFiltering( sf );

            // Checks if Motion smoothing setting can be read and synchs adv
            // settings to steamvr/openvr
            auto ms = vr::VRSettings()->GetBool(
                vr::k_pch_SteamVR_Section,
                vr::k_pch_SteamVR_MotionSmoothing_Bool,
                &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not read \""
                    << vr::k_pch_SteamVR_MotionSmoothing_Bool << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            setMotionSmoothing( ms );

            // Checks and synchs performance graph
            auto pg
                = vr::VRSettings()->GetBool( vr::k_pch_Perf_Section,
                                             vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                             &vrSettingsError );
            if ( vrSettingsError != vr::VRSettingsError_None )
            {
                LOG( WARNING )
                    << "Could not read \"" << vr::k_pch_Perf_PerfGraphInHMD_Bool
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            setPerformanceGraph( pg );
        }
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

void SteamVRTabController::addSteamVRProfile(
    const QString name,
    const bool includeSupersampling,
    const bool includeSupersampleFiltering,
    const bool includeMotionSmoothing )
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

void SteamVRTabController::applySteamVRProfile( const unsigned index )
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

void SteamVRTabController::deleteSteamVRProfile( const unsigned index )
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
        auto& entry = steamvrProfiles[static_cast<size_t>( i )];
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

void SteamVRTabController::saveSteamVRProfiles()
{
    auto settings = OverlayController::appSettings();
    settings->beginGroup( "steamVRSettings" );
    settings->beginWriteArray( "steamVRProfiles" );
    unsigned i = 0;
    for ( auto& p : steamvrProfiles )
    {
        settings->setArrayIndex( static_cast<int>( i ) );
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
int SteamVRTabController::getSteamVRProfileCount()
{
    return static_cast<int>( steamvrProfiles.size() );
}

QString SteamVRTabController::getSteamVRProfileName( const unsigned index )
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

void SteamVRTabController::setSuperSampling( float value, const bool notify )
{
    bool override = false;
    // Mirrors Desktop Clamp
    if ( value <= 0.2f )
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

void SteamVRTabController::setMotionSmoothing( const bool value,
                                               const bool notify )
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

void SteamVRTabController::setAllowSupersampleFiltering( const bool value,
                                                         const bool notify )
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

void SteamVRTabController::setAllowSupersampleOverride( const bool value,
                                                        const bool notify )
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

bool SteamVRTabController::performanceGraph() const
{
    return m_performanceGraphToggle;
}

void SteamVRTabController::setPerformanceGraph( const bool value,
                                                const bool notify )
{
    if ( m_performanceGraphToggle != value )
    {
        m_performanceGraphToggle = value;
        vr::VRSettings()->SetBool( vr::k_pch_Perf_Section,
                                   vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                   m_performanceGraphToggle );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit performanceGraphChanged( m_performanceGraphToggle );
        }
    }
}

/*------------------------------------------*/
/* -----------------------------------------*/

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

    vr::VRSettings()->RemoveKeyInSection( vr::k_pch_Perf_Section,
                                          vr::k_pch_Perf_PerfGraphInHMD_Bool,
                                          &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_Perf_PerfGraphInHMD_Bool << "\" setting: "
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

} // namespace advsettings
