#pragma once
#include <QObject>
#include <openvr.h>
#include <easylogging++.h>
#include <QVector2D>
#include <QVector3D>
#include <QSettings>

void outputMat();
void transformMat();

namespace overlay
{
namespace strings
{
    constexpr auto overlaykey = "valve.steam.desktop";
    constexpr auto settingsGroupName = "utilitiesSettings";
    constexpr auto widthSettingsName = "desktopWidth";
} // namespace strings

constexpr auto defaultOverlayWidth = 4.0;

class DesktopOverlay : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY( double width READ getCurrentWidth WRITE setWidth )

    DesktopOverlay();

    Q_INVOKABLE void update();

    bool isAvailable() const;

    void setWidth( double width );
    double getCurrentWidth() const noexcept;

private:
    double m_width = defaultOverlayWidth;
};

} // namespace overlay
