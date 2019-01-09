
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

    //bool includesReprojectionSettings = false;
    //bool asynchronousReprojection = true;
    //bool interleavedReprojection = true;
    //bool alwaysOnReprojection = true;
};

class SteamVRTabController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( float superSampling READ superSampling WRITE setSuperSampling
                    NOTIFY superSamplingChanged )
    Q_PROPERTY(
        float compositorSuperSampling READ compositorSuperSampling WRITE
            setCompositorSuperSampling NOTIFY compositorSuperSamplingChanged )
    /*
		Q_PROPERTY(
        bool allowInterleavedReprojection READ allowInterleavedReprojection
            WRITE setAllowInterleavedReprojection NOTIFY
                allowInterleavedReprojectionChanged )
    Q_PROPERTY(
        bool allowAsyncReprojection READ allowAsyncReprojection WRITE
            setAllowAsyncReprojection NOTIFY allowAsyncReprojectionChanged )
    Q_PROPERTY( bool forceReprojection READ forceReprojection WRITE
                    setForceReprojection NOTIFY forceReprojectionChanged )
	*/

	Q_PROPERTY(bool motionSmoothing READ motionSmoothing
			WRITE setMotionSmoothing NOTIFY
			motionSmoothingChanged)
    Q_PROPERTY( bool allowSupersampleFiltering READ allowSupersampleFiltering
                    WRITE setAllowSupersampleFiltering NOTIFY
                        allowSupersampleFilteringChanged )

private:
    OverlayController* parent;
    QQuickWindow* widget;

    float m_superSampling = 1.0;
    float m_compositorSuperSampling = 1.0;
    //bool m_allowInterleavedReprojection = true;
    //bool m_allowAsyncReprojection = true;
    //bool m_forceReprojection = false;
	bool m_motionSmoothing = true;
    bool m_allowSupersampleFiltering = true;

	void initMotionSmoothing();

    std::vector<SteamVRProfile> steamvrProfiles;

    unsigned settingsUpdateCounter = 0;

public:
    void initStage1();
    void initStage2( OverlayController* parent, QQuickWindow* widget );

    void eventLoopTick();

    float superSampling() const;
    float compositorSuperSampling() const;
    //bool allowInterleavedReprojection() const;
    //bool allowAsyncReprojection() const;
    //bool forceReprojection() const;
	bool motionSmoothing() const;
    bool allowSupersampleFiltering() const;

    void reloadSteamVRProfiles();
    void saveSteamVRProfiles();

    Q_INVOKABLE unsigned getSteamVRProfileCount();
    Q_INVOKABLE QString getSteamVRProfileName( unsigned index );

public slots:
    void setSuperSampling( float value, bool notify = true );
    void setCompositorSuperSampling( float value, bool notify = true );
    //void setAllowInterleavedReprojection( bool value, bool notify = true );
    //void setAllowAsyncReprojection( bool value, bool notify = true );
    //void setForceReprojection( bool value, bool notify = true );
	void setMotionSmoothing(bool value, bool notify = true);
    void setAllowSupersampleFiltering( bool value, bool notify = true );

    void addSteamVRProfile( QString name,
                            bool includeSupersampling,
                            bool includeSupersampleFiltering);
    void applySteamVRProfile( unsigned index );
    void deleteSteamVRProfile( unsigned index );

    void reset();
    void restartSteamVR();

signals:
    void superSamplingChanged( float value );
    void compositorSuperSamplingChanged( float value );
    //void allowInterleavedReprojectionChanged( bool value );
    //void allowAsyncReprojectionChanged( bool value );
    //void forceReprojectionChanged( bool value );
	void motionSmoothingChanged(bool value);
    void allowSupersampleFilteringChanged( bool value );

    void steamVRProfilesUpdated();
};

} // namespace advsettings
