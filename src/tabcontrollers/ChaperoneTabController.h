
#pragma once

#include <QObject>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;


class ChaperoneTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

	bool forceBoundsFlag = false;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

public slots:
	void UpdateTab();

	void CenterMarkerToggled(bool value);
	void PlaySpaceToggled(bool value);
	void ForceBoundsToggled(bool value);
	void FadeDistanceChanged(int value);
	void ColorAlphaChanged(int value);
	void FlipOrientationClicked();
	void ReloadClicked();
	void ResetDefaultsClicked();
};

} // namespace advsettings
