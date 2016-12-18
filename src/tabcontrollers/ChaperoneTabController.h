
#pragma once

#include <QObject>
#include <memory>
#include <openvr.h>

class QQuickWindow;
// application namespace
namespace advsettings {

// forward declaration
class OverlayController;


struct ChaperoneProfile {
	std::string profileName;

	bool includesChaperoneGeometry = false;
	unsigned chaperoneGeometryQuadCount = 0;
	std::unique_ptr<vr::HmdQuad_t> chaperoneGeometryQuads = nullptr;
	vr::HmdMatrix34_t standingCenter;
	float playSpaceAreaX = 0.0f;
	float playSpaceAreaZ = 0.0f;

	bool includesVisibility = false;
	float visibility = 0.6f;

	bool includesFadeDistance = false;
	float fadeDistance = 0.7f;

	bool includesCenterMarker = false;
	bool centerMarker = false;

	bool includesPlaySpaceMarker = false;
	bool playSpaceMarker = false;

	bool includesFloorBoundsMarker = false;
	bool floorBoundsMarker = false;

	bool includesBoundsColor = false;
	int boundsColor[3] = {0, 0, 0};

	bool includesChaperoneStyle = false;
	int chaperoneStyle = 0;

	bool includesForceBounds = false;
	bool forceBounds = false;
};


class ChaperoneTabController : public QObject {
	Q_OBJECT
	Q_PROPERTY(float boundsVisibility READ boundsVisibility WRITE setBoundsVisibility NOTIFY boundsVisibilityChanged)
	Q_PROPERTY(float fadeDistance READ fadeDistance WRITE setFadeDistance NOTIFY fadeDistanceChanged)
	Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
	Q_PROPERTY(bool centerMarker READ centerMarker WRITE setCenterMarker NOTIFY centerMarkerChanged)
	Q_PROPERTY(bool playSpaceMarker READ playSpaceMarker WRITE setPlaySpaceMarker NOTIFY playSpaceMarkerChanged)
	Q_PROPERTY(bool forceBounds READ forceBounds WRITE setForceBounds NOTIFY forceBoundsChanged)

private:
	OverlayController* parent;
	QQuickWindow* widget;

	float m_visibility = 0.6f;
	float m_fadeDistance = 0.7f;
	float m_height = 2.0f;
	bool m_centerMarker = false;
	bool m_playSpaceMarker = false;
	bool m_forceBounds = false;

	unsigned settingsUpdateCounter = 0;

	std::vector<ChaperoneProfile> chaperoneProfiles;

public:
	void initStage1();
	void initStage2(OverlayController* parent, QQuickWindow* widget);

	void eventLoopTick();

	float boundsVisibility() const;
	float fadeDistance() const;
	float height() const;
	bool centerMarker() const;
	bool playSpaceMarker() const;
	bool forceBounds() const;

	void reloadChaperoneProfiles();
	void saveChaperoneProfiles();

	Q_INVOKABLE unsigned getChaperoneProfileCount();
	Q_INVOKABLE QString getChaperoneProfileName(unsigned index);

	float getBoundsMaxY();

public slots:
	void setBoundsVisibility(float value, bool notify = true);
	void setFadeDistance(float value, bool notify = true);
	void setHeight(float value, bool notify = true);
	void updateHeight(float value, bool notify = true);
	void setCenterMarker(bool value, bool notify = true);
	void setPlaySpaceMarker(bool value, bool notify = true);
	void setForceBounds(bool value, bool notify = true);

	void flipOrientation();
	void reloadFromDisk();

	void addChaperoneProfile(QString name, bool includeGeometry, bool includeVisbility, bool includeFadeDistance, bool includeCenterMarker,
	bool includePlaySpaceMarker, bool includeFloorBounds, bool includeBoundsColor, bool includeChaperoneStyle, bool includeForceBounds);
	void applyChaperoneProfile(unsigned index);
	void deleteChaperoneProfile(unsigned index);

	void reset();

signals:
	void boundsVisibilityChanged(float value);
	void fadeDistanceChanged(float value);
	void heightChanged(float value);
	void centerMarkerChanged(bool value);
	void playSpaceMarkerChanged(bool value);
	void forceBoundsChanged(bool value);

	void chaperoneProfilesUpdated();
};

} // namespace advsettings
