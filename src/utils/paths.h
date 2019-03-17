#pragma once
#include <string>
#include <optional>

namespace paths
{
using std::optional;
using std::string;

optional<string> binaryDirectory();
optional<string> binaryDirectoryFindFile( string fileName );
optional<string> binaryDirectoryFindDirectory( string directoryName );

optional<string> settingsDirectory();

} // namespace paths
