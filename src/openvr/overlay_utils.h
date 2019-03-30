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
    constexpr auto forwardsSettingsName = "desktopForwardsMovement";
    constexpr auto rightMovementSettingsName = "desktopRightMovement";
    constexpr auto upMovementSettingsName = "desktopUpMovement";
    constexpr auto positionSettingsName = "desktopOverlayPosition";
} // namespace strings

constexpr auto defaultOverlayWidth = 4.0;
constexpr auto defaultMovement = 0.0;
constexpr auto defaultWidth = 1.0;

class DesktopOverlay : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY( double rightMovement READ getCurrentRightMovement()
                    WRITE setRightMovement )
    Q_PROPERTY( double forwardsMovement READ getCurrentForwardsMovement WRITE
                    setForwardsMovement )
    Q_PROPERTY( double width READ getCurrentWidth WRITE setWidth )
    Q_PROPERTY( double height READ getCurrentHeight WRITE setHeight )

    DesktopOverlay();

    Q_INVOKABLE void update();

    bool isAvailable() const;

    void setWidth( double width );
    double getCurrentWidth() const noexcept;

    void setRightMovement( double distance );
    double getCurrentRightMovement() const noexcept;

    void setForwardsMovement( double distance );
    double getCurrentForwardsMovement() const noexcept;

    void setHeight( double height );
    double getCurrentHeight() const noexcept;

private:
    vr::HmdMatrix34_t getChangedMatrix( vr::HmdMatrix34_t& matrix ) const
        noexcept;

    double m_width = defaultOverlayWidth;
    double m_forwardsMovement = defaultMovement;
    double m_rightMovement = defaultMovement;
    double m_upMovement = defaultMovement;
};

} // namespace overlay
