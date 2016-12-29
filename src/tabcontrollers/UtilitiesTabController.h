
#pragma once

#include <QObject>

class QQuickWindow;
// application namespace
namespace advsettings {

	// forward declaration
	class OverlayController;


	class UtilitiesTabController : public QObject {
		Q_OBJECT

	private:
		OverlayController* parent;
		QQuickWindow* widget;

	public:
		void initStage1();
		void initStage2(OverlayController* parent, QQuickWindow* widget);

	public slots:
		void sendKeyboardInput(QString input);
		void sendKeyboardEnter();
		void sendKeyboardBackspace(int count);

	signals:
	};

} // namespace advsettings
