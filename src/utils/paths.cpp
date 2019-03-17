#include "paths.h"
#include <QStandardPaths>
#include <QString>
#include <easylogging++.h>

namespace paths
{
optional<string> binaryDirectory()
{
    const auto path
        = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );

    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> binaryDirectoryFindFile( const string fileName )
{
    const auto path = QStandardPaths::locate(
        QStandardPaths::AppDataLocation, QString::fromStdString( fileName ) );

    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find file '" << fileName
                     << "' in binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

} // namespace paths
