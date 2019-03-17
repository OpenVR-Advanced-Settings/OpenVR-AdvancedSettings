#include "paths.h"
#include <QStandardPaths>
namespace paths
{
optional<string> binaryDirectory()
{
    const auto binaryDir
        = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );

    if ( binaryDir != "" )
    {
        return binaryDir.toStdString();
    }
    else
    {
        return {};
    }
}
} // namespace paths
