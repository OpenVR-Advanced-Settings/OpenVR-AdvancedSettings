
#pragma once

#include "PttController.h"

class QQuickWindow;
// application namespace
namespace advsettings {

// forward declaration
class OverlayController;


class AccessibilityTabController : public PttController {
  Q_OBJECT
  Q_PROPERTY(float heightOffset READ heightOffset WRITE setHeightOffset NOTIFY heightOffsetChanged)

private:
  OverlayController* parent;
  QQuickWindow* widget;

  int m_trackingUniverse = (int)vr::TrackingUniverseStanding;
  float m_heightOffset = 0.0f;
  float m_toggleHeightOffset = 0.0f;

  unsigned settingsUpdateCounter = 0;

  QString getSettingsName() override { return "accessibilitySettings"; }
  void onPttStart() override;
  void onPttStop() override;
  void onPttDisabled() override;

public:
  void initStage1();
  void initStage2(OverlayController* parent, QQuickWindow* widget);
  void eventLoopTick(vr::ETrackingUniverseOrigin universe);

  float heightOffset() const;

public slots:
  void setTrackingUniverse(int value);
  void setHeightOffset(float value, bool notify = true);
  void modHeightOffset(float value, bool notify = true);
  void reset();

signals:
  void heightOffsetChanged(float value);
};

} // namespace advsettings
