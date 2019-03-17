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
    const auto path = QStandardPaths::locate(
        QStandardPaths::AppDataLocation, QString::fromStdString( fileName ) );
    if ( path == "" )
    {
        LOG( ERROR ) << "Could not find file '" << fileName.c_str()
                     << "' in binary directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> qmlDirectory()
{
    const auto path
        = QStandardPaths::locate( QStandardPaths::AppDataLocation, "res/qml/" );
    if ( path == "" )
    {
        LOG( ERROR ) << "Unable to find QML directory.";
        return std::nullopt;
    }

    return path.toStdString();
}

optional<string> qmlDirectoryFindFile( string fileName )
{
    const auto qmlPath = qmlDirectory();
    if ( !qmlPath.has_value() )
    {
        return std::nullopt;
    }

    const auto path = ( *qmlPath ) + fileName;
    QFileInfo filePath( QString::fromStdString( path ) );
    if ( !filePath.exists() )
    {
        LOG( ERROR ) << "File: '" << path << "' not found.";
        return std::nullopt;
    }

    return std::move( path );
}

} // namespace paths
