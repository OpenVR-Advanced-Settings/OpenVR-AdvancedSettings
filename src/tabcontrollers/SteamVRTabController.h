
#pragma once

#include <QObject>

class QQuickWindow;
// application namespace
namespace advsettings
{
// forward declaration
class OverlayController;

struct SteamVRProfile
{
    std::string profileName;

    bool includesSupersampling = false;
    float supersampling = 1.0f;

    bool includesSupersampleFiltering = false;
    bool supersampleFiltering = false;
    bool motionSmooth = false;
    bool includesMotionSmoothing = false;
    bool supersampleOverride = false;
};

class SteamVRTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float superSampling READ superSampling WRITE setSuperSampling
                    NOTIFY superSamplingChanged )

    Q_PROPERTY( bool motionSmoothing READ motionSmoothing WRITE
                    setMotionSmoothing NOTIFY motionSmoothingChanged )
    Q_PROPERTY( bool allowSupersampleFiltering READ allowSupersampleFiltering
                    WRITE setAllowSupersampleFiltering NOTIFY
                        allowSupersampleFilteringChanged )
    Q_PROPERTY(
        bool allowSupersampleOverride READ allowSupersampleOverride WRITE
            setAllowSupersampleOverride NOTIFY allowSupersampleOverrideChanged )

    Q_PROPERTY( bool performanceGraph READ performanceGraph WRITE
                    setPerformanceGraph NOTIFY performanceGraphChanged )

private:
    OverlayController* parent;

    float m_superSampling = 1.0;
    bool m_motionSmoothing = true;
    bool m_allowSupersampleFiltering = true;
    bool m_allowSupersampleOverride = false;
    bool m_performanceGraphToggle = false;

    void initMotionSmoothing();
    void initSupersampleOverride();

    std::vector<SteamVRProfile> steamvrProfiles;

    unsigned settingsUpdateCounter = 0;

public:
    void initStage1();
    void initStage2( OverlayController* parent );

    void eventLoopTick();

    float superSampling() const;
    bool motionSmoothing() const;
    bool allowSupersampleFiltering() const;
    bool allowSupersampleOverride() const;
    bool performanceGraph() const;

    void reloadSteamVRProfiles();
    void saveSteamVRProfiles();

    Q_INVOKABLE int getSteamVRProfileCount();
    Q_INVOKABLE QString getSteamVRProfileName( unsigned index );

public slots:
    void setSuperSampling( float value, bool notify = true );
    void setMotionSmoothing( bool value, bool notify = true );
    void setAllowSupersampleFiltering( bool value, bool notify = true );
    void setAllowSupersampleOverride( bool value, bool notify = true );
    void setPerformanceGraph( bool value, bool notify = true );

    void addSteamVRProfile( QString name,
                            bool includeSupersampling,
                            bool includeSupersampleFiltering,
                            bool includeMotionSmoothing );
    void applySteamVRProfile( unsigned index );
    void deleteSteamVRProfile( unsigned index );

    void reset();
    void restartSteamVR();

signals:
    void superSamplingChanged( float value );
    void motionSmoothingChanged( bool value );
    void allowSupersampleFilteringChanged( bool value );
    void allowSupersampleOverrideChanged( bool value );
    void performanceGraphChanged( bool value );

    void steamVRProfilesUpdated();
    void steamVRProfileAdded();
};

} // namespace advsettings
