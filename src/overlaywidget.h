#pragma once


#include <QWidget>
#include <memory>


// forward declaration
namespace Ui { class OverlayWidget; }

// application namespace
namespace advsettings {

class OverlayWidget : public QWidget {
    Q_OBJECT

public:
    std::unique_ptr<Ui::OverlayWidget> ui;

    explicit OverlayWidget(QWidget *parent = 0);

private slots:
};

} // namespace advsettings


