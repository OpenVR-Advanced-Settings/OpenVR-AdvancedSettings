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
class DesktopOverlay : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY( double width READ getCurrentWidth WRITE setWidth )

    Q_INVOKABLE void update();

    bool isAvailable() const;

    void setWidth( double width );
    double getCurrentWidth() const noexcept;
};

} // namespace overlay
