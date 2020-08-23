#include "paths.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QString>
#include <QFileInfo>
#include <easylogging++.h>
#include <QDir>

namespace paths
{
optional<string> binaryDirectory()
{
    const auto path = QCoreApplication::applicationDirPath();

    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> binaryDirectoryFindFile( const string fileName )
{
    const auto path = binaryDirectory();

    if ( !path )
    {
        return std::nullopt;
    }

    const auto filePath = QDir( QString::fromStdString( *path ) + '/'
                                + QString::fromStdString( fileName ) );
    QFileInfo file( filePath.path() );

    if ( !file.exists() )
    {
        LOG( ERROR ) << "Could not find file '" << fileName.c_str()
                     << "' in binary directory.";
        return std::nullopt;
    }

    return QDir::toNativeSeparators( file.filePath() ).toStdString();
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

std::optional<std::string> verifyIconFilePath( std::string filename )
{
    const auto notifIconPath = paths::binaryDirectoryFindFile( filename );
    if ( !notifIconPath.has_value() )
    {
        LOG( ERROR ) << "Could not find icon \"" << filename << "\"";
    }

    return notifIconPath;
}

} // namespace paths
