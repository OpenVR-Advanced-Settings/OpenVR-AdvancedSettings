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

static const char* vrsettings_steamvr_supersampleScale = "supersampleScale";
static const char* vrsettings_steamvr_allowSupersampleFiltering
    = "allowSupersampleFiltering";
static const char* vrsettings_compositor_category = "compositor";

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
		/*
        entry.includesReprojectionSettings
            = settings->value( "includesReprojectionSettings", false ).toBool();
        if ( entry.includesReprojectionSettings )
        {
            entry.asynchronousReprojection
                = settings->value( "asynchronousReprojection", true ).toBool();
            entry.interleavedReprojection
                = settings->value( "interleavedReprojection", true ).toBool();
            entry.alwaysOnReprojection
                = settings->value( "alwaysOnReprojection", true ).toBool();
        }
		*/
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
		/*
        settings->setValue( "includesReprojectionSettings",
                            p.includesReprojectionSettings );
        if ( p.includesReprojectionSettings )
        {
            settings->setValue( "asynchronousReprojection",
                                p.asynchronousReprojection );
            settings->setValue( "interleavedReprojection",
                                p.interleavedReprojection );
            settings->setValue( "alwaysOnReprojection",
                                p.alwaysOnReprojection );
        }
		*/
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

void SteamVRTabController::eventLoopTick()
{
    if ( settingsUpdateCounter >= 50 )
    {
        vr::EVRSettingsError vrSettingsError;
        auto ss
            = vr::VRSettings()->GetFloat( vr::k_pch_SteamVR_Section,
                                          vrsettings_steamvr_supersampleScale,
                                          &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vrsettings_steamvr_supersampleScale
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
        auto css = vr::VRSettings()->GetFloat(
            vrsettings_compositor_category,
            vr::k_pch_SteamVR_RenderTargetMultiplier_Float,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            if ( vrSettingsError
                 != vr::VRSettingsError_UnsetSettingHasNoDefault )
            { // does not appear in the default settings file as of beta
              // 1477423729
                LOG( WARNING )
                    << "Could not read \"compositor::"
                    << vr::k_pch_SteamVR_RenderTargetMultiplier_Float
                    << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                           vrSettingsError );
            }
            if ( m_compositorSuperSampling != 1.0 )
            {
                LOG( DEBUG ) << "OpenVR returns an error and we have a custom "
                                "compositor supersampling value: "
                             << m_compositorSuperSampling;
                setCompositorSuperSampling( 1.0 );
            }
        }
        else if ( m_compositorSuperSampling != css )
        {
            LOG( DEBUG )
                << "OpenVR reports a changed compositor supersampling value: "
                << m_compositorSuperSampling << " => " << css;
            setCompositorSuperSampling( css );
        }
		/*
        auto air = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowReprojection_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_AllowReprojection_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowInterleavedReprojection( air );
        auto aar = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowAsyncReprojection_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_AllowAsyncReprojection_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowAsyncReprojection( aar );
        auto fr = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_ForceReprojection_Bool,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vr::k_pch_SteamVR_ForceReprojection_Bool
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setForceReprojection( fr );
		*/
        auto sf = vr::VRSettings()->GetBool(
            vr::k_pch_SteamVR_Section,
            vrsettings_steamvr_allowSupersampleFiltering,
            &vrSettingsError );
        if ( vrSettingsError != vr::VRSettingsError_None )
        {
            LOG( WARNING ) << "Could not read \""
                           << vrsettings_steamvr_allowSupersampleFiltering
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  vrSettingsError );
        }
        setAllowSupersampleFiltering( sf );
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
                                    vrsettings_steamvr_supersampleScale,
                                    m_superSampling );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit superSamplingChanged( m_superSampling );
        }
    }
}

void SteamVRTabController::setCompositorSuperSampling( float value,
                                                       bool notify )
{
    bool override = false;
    if ( value <= 0.01f )
    {
        LOG( WARNING ) << "Encountered a compositor supersampling value <= "
                          "0.01, setting supersampling to 1.0";
        value = 1.0f;
        override = true;
    }
    if ( override || m_compositorSuperSampling != value )
    {
        LOG( DEBUG ) << "Compositor supersampling value changed: "
                     << m_compositorSuperSampling << " => " << value;
        m_compositorSuperSampling = value;
        vr::VRSettings()->SetFloat(
            vrsettings_compositor_category,
            vr::k_pch_SteamVR_RenderTargetMultiplier_Float,
            m_compositorSuperSampling );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit compositorSuperSamplingChanged( m_compositorSuperSampling );
        }
    }
}

float SteamVRTabController::superSampling() const
{
    return m_superSampling;
}

float SteamVRTabController::compositorSuperSampling() const
{
    return m_compositorSuperSampling;
}



bool SteamVRTabController::motionSmoothing() const {
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

void SteamVRTabController::setMotionSmoothing(bool value,
	bool notify)
{
	if (m_motionSmoothing!= value)
	{
		m_motionSmoothing = value;
		vr::VRSettings()->SetBool(vr::k_pch_SteamVR_Section,
			vr::k_pch_SteamVR_MotionSmoothing_Bool,
			m_motionSmoothing);
		vr::VRSettings()->Sync();
		if (notify)
		{
			emit motionSmoothingChanged(
				m_motionSmoothing);
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
default value of true for motion smoothing is initialized, if user has disabled motion smoothing on their end,
they will have to toggle the variable twice to get it to re-sync.
*/
void SteamVRTabController::initMotionSmoothing() {
	bool temporary = false;
	vr::EVRSettingsError vrSettingsError;
	temporary = vr::VRSettings()->GetBool(vr::k_pch_SteamVR_Section,
		vr::k_pch_SteamVR_MotionSmoothing_Bool,
		&vrSettingsError);
	if (vrSettingsError != vr::VRSettingsError_None)
	{
		LOG(WARNING) << "Could not get MotionSmoothing State \""
			<< vr::k_pch_SteamVR_MotionSmoothing_Bool << "\" setting: "
			<< vr::VRSettings()->GetSettingsErrorNameFromEnum(
				vrSettingsError);
	}
	vr::VRSettings()->Sync();
	setMotionSmoothing(temporary, true);
		

}

/*
bool SteamVRTabController::allowInterleavedReprojection() const
{
    return m_allowInterleavedReprojection;
}

bool SteamVRTabController::allowAsyncReprojection() const
{
    return m_allowAsyncReprojection;
}

void SteamVRTabController::setAllowInterleavedReprojection( bool value,
                                                            bool notify )
{
    if ( m_allowInterleavedReprojection != value )
    {
        m_allowInterleavedReprojection = value;
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vr::k_pch_SteamVR_AllowReprojection_Bool,
                                   m_allowInterleavedReprojection );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit allowInterleavedReprojectionChanged(
                m_allowInterleavedReprojection );
        }
    }
}

void SteamVRTabController::setAllowAsyncReprojection( bool value, bool notify )
{
    if ( m_allowAsyncReprojection != value )
    {
        m_allowAsyncReprojection = value;
        vr::VRSettings()->SetBool(
            vr::k_pch_SteamVR_Section,
            vr::k_pch_SteamVR_AllowAsyncReprojection_Bool,
            m_allowAsyncReprojection );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit allowAsyncReprojectionChanged( m_allowAsyncReprojection );
        }
    }
}

bool SteamVRTabController::forceReprojection() const
{
    return m_forceReprojection;
}

void SteamVRTabController::setForceReprojection( bool value, bool notify )
{
    if ( m_forceReprojection != value )
    {
        m_forceReprojection = value;
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vr::k_pch_SteamVR_ForceReprojection_Bool,
                                   m_forceReprojection );
        vr::VRSettings()->Sync();
        if ( notify )
        {
            emit forceReprojectionChanged( m_forceReprojection );
        }
    }
}
*/
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
        vr::VRSettings()->SetBool( vr::k_pch_SteamVR_Section,
                                   vrsettings_steamvr_allowSupersampleFiltering,
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

    vr::VRSettings()->RemoveKeyInSection( vr::k_pch_SteamVR_Section,
                                          vrsettings_steamvr_supersampleScale,
                                          &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vrsettings_steamvr_supersampleScale << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    setCompositorSuperSampling( 1.0f ); // Is not in default.vrsettings yet
    vr::VRSettings()->RemoveKeyInSection(
        vrsettings_compositor_category,
        vr::k_pch_SteamVR_RenderTargetMultiplier_Float,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vrsettings_compositor_category
                       << "::" << vr::k_pch_SteamVR_RenderTargetMultiplier_Float
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }
	/*
    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_AllowReprojection_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_AllowReprojection_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_AllowAsyncReprojection_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_AllowAsyncReprojection_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vr::k_pch_SteamVR_ForceReprojection_Bool,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vr::k_pch_SteamVR_ForceReprojection_Bool
                       << "\" setting: "
                       << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                              vrSettingsError );
    }

	*/
    vr::VRSettings()->RemoveKeyInSection(
        vr::k_pch_SteamVR_Section,
        vrsettings_steamvr_allowSupersampleFiltering,
        &vrSettingsError );
    if ( vrSettingsError != vr::VRSettingsError_None )
    {
        LOG( WARNING ) << "Could not remove \""
                       << vrsettings_steamvr_allowSupersampleFiltering
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
                                              bool includeSupersampleFiltering)
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
	/*
    profile->includesReprojectionSettings = includeReprojectionSettings;
    if ( includeReprojectionSettings )
    {
        profile->asynchronousReprojection = m_allowAsyncReprojection;
        profile->interleavedReprojection = m_allowInterleavedReprojection;
        profile->alwaysOnReprojection = m_forceReprojection;
    }
	*/
    saveSteamVRProfiles();
    OverlayController::appSettings()->sync();
    emit steamVRProfilesUpdated();
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
		/*
        if ( profile.includesReprojectionSettings )
        {
            setAllowAsyncReprojection( profile.asynchronousReprojection );
            setAllowInterleavedReprojection( profile.interleavedReprojection );
            setForceReprojection( profile.alwaysOnReprojection );
        }
		*/
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
