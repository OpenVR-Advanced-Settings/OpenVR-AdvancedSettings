
#pragma once

#include <QObject>

// openvr's github repo hasn't been updated yet to reflect changes in beta 1477423729.
//static const char* vrsettings_steamvr_allowAsyncReprojection = "allowAsyncReprojection";
//static const char* vrsettings_steamvr_allowInterleavedReprojection = "allowInterleavedReprojection";
static const char* vrsettings_compositor_category = "compositor";
static const char* vrsettings_steamvr_supersampleScale = "supersampleScale";
static const char* vrsettings_steamvr_allowSupersampleFiltering = "allowSupersampleFiltering";


class QQuickWindow;
// application namespace
namespace advsettings {

// forward declaration
class OverlayController;


class SteamVRTabController : public QObject {
	Q_OBJECT
	Q_PROPERTY(float superSampling READ superSampling WRITE setSuperSampling NOTIFY superSamplingChanged)
	Q_PROPERTY(float compositorSuperSampling READ compositorSuperSampling WRITE setCompositorSuperSampling NOTIFY compositorSuperSamplingChanged)
	Q_PROPERTY(bool allowInterleavedReprojection READ allowInterleavedReprojection WRITE setAllowInterleavedReprojection NOTIFY allowInterleavedReprojectionChanged)
	Q_PROPERTY(bool allowAsyncReprojection READ allowAsyncReprojection WRITE setAllowAsyncReprojection NOTIFY allowAsyncReprojectionChanged)
	Q_PROPERTY(bool forceReprojection READ forceReprojection WRITE setForceReprojection NOTIFY forceReprojectionChanged)
	Q_PROPERTY(bool allowSupersampleFiltering READ allowSupersampleFiltering WRITE setAllowSupersampleFiltering NOTIFY allowSupersampleFilteringChanged)

private:
	OverlayController* parent;
	QQuickWindow* widget;

	float m_superSampling = 1.0;
	float m_compositorSuperSampling = 1.0;
	bool m_allowInterleavedReprojection = true;
	bool m_allowAsyncReprojection = true;
	bool m_forceReprojection = false;
	bool m_allowSupersampleFiltering = true;

	unsigned settingsUpdateCounter = 0;

public:
	void initStage1();
	void initStage2(OverlayController* parent, QQuickWindow* widget);

	void eventLoopTick();

	float superSampling() const;
	float compositorSuperSampling() const;
	bool allowInterleavedReprojection() const;
	bool allowAsyncReprojection() const;
	bool forceReprojection() const;
	bool allowSupersampleFiltering() const;

public slots:
	void setSuperSampling(float value, bool notify = true);
	void setCompositorSuperSampling(float value, bool notify = true);
	void setAllowInterleavedReprojection(bool value, bool notify = true);
	void setAllowAsyncReprojection(bool value, bool notify = true);
	void setForceReprojection(bool value, bool notify = true);
	void setAllowSupersampleFiltering(bool value, bool notify = true);

	void reset();
	void restartSteamVR();

signals:
	void superSamplingChanged(float value);
	void compositorSuperSamplingChanged(float value);
	void allowInterleavedReprojectionChanged(bool value);
	void allowAsyncReprojectionChanged(bool value);
	void forceReprojectionChanged(bool value);
	void allowSupersampleFilteringChanged(bool value);
};

} // namespace advsettings
