#include "overlaywidget.h"
#include "ui_overlaywidget.h"

// application namespace
namespace advsettings {

	OverlayWidget::OverlayWidget(QWidget *parent) :	
			QWidget(parent), ui(new Ui::OverlayWidget) {
		ui->setupUi(this);
	}

} // namespace advsettings
