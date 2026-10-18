#include "ImageUtils.h"
#include <QPainter>
#include <QSvgRenderer>
#include <QtLogging>
#include <QtDebug>

namespace utils
{
QImage renderSvgToImage( const QString& path, const QSize& size )
{
    QImage image( size, QImage::Format_ARGB32_Premultiplied );
    image.fill( Qt::transparent );
    if ( !renderSvgOntoImage( image, path ) )
    {
        return {};
    }
    return image;
}

bool renderSvgOntoImage( QImage& image, const QString& path )
{
    QSvgRenderer renderer( path );
    if ( !renderer.isValid() )
    {
        qCritical() << "Could not load svg:" << path;
        return false;
    }

    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing );
    renderer.render( &painter );
    return true;
}

} // end namespace utils
