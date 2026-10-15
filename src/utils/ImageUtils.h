#pragma once

#include <QImage>
#include <QSize>
#include <QString>

namespace utils
{
// Renders the svg at path into a new transparent image of the given size.
// Returns a null QImage if the svg could not be loaded.
QImage renderSvgToImage( const QString& path, const QSize& size );

// Renders the svg at path on top of the existing contents of image, scaled to
// the image's size. Returns false if the svg could not be loaded.
bool renderSvgOntoImage( QImage& image, const QString& path );

} // end namespace utils
