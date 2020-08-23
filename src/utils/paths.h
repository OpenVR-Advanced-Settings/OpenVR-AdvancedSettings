#pragma once
#include <string>
#include <optional>

namespace paths
{
using std::optional;
using std::string;

optional<string> binaryDirectory();
optional<string> binaryDirectoryFindFile( string fileName );

optional<string> settingsDirectory();

std::optional<std::string> verifyIconFilePath( std::string filename );

} // namespace paths
