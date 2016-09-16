
#pragma once

#include <QObject>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;


class SteamVRTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

public slots:
	void UpdateTab();

	void SupersamplingChanged(int value);
	void ReprojectionToggled(bool value);
	void ForceReprojectionToggled(bool value);
	void ResetClicked();
	void RestartClicked();
};

} // namespace advsettings
