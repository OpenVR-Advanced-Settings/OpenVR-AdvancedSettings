
#pragma once

#include <QObject>

// application namespace
namespace advsettings {

// forward declaration
class OverlayWidget;
class OverlayController;


class SettingsTabController : public QObject {
	Q_OBJECT

private:
	OverlayController* parent;
	OverlayWidget* widget;

public:
	void init(OverlayController* parent, OverlayWidget* widget);

	public slots:
	void UpdateTab();

	void AutoStartToggled(bool value);
};

} // namespace advsettings
