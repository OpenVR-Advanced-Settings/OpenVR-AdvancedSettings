#include "paths.h"
#include <QStandardPaths>
#include <QString>
#include <QFileInfo>
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
    const auto path
        = QStandardPaths::locate( QStandardPaths::AppDataLocation,
                                  QString::fromStdString( fileName ),
                                  QStandardPaths::LocateFile );
    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find file '" << fileName.c_str()
                     << "' in binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> binaryDirectoryFindDirectory( const string directoryName )
{
    const auto path
        = QStandardPaths::locate( QStandardPaths::AppDataLocation,
                                  QString::fromStdString( directoryName ),
                                  QStandardPaths::LocateDirectory );
    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find directory '" << directoryName.c_str()
                     << "' in binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> settingsDirectory()
{
    const auto path
        = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find settings directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

} // namespace paths
