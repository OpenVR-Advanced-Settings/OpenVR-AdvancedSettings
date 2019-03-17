#pragma once
#include <string>
#include <optional>

namespace paths
{
using std::optional;
using std::string;

optional<string> binaryDirectory();
optional<string> binaryDirectoryFindFile( string fileName );

optional<string> qmlDirectory();
optional<string> qmlDirectoryFindFile( string fileName );

optional<string> imageDirectory();
optional<string> imageDirectoryFindFile( string fileName );

optional<string> soundDirectory();
optional<string> soundDirectoryFindFile( string fileName );

optional<string> settingsDirectory();
optional<string> settingsDirectoryFindFile( string fileName );

} // namespace paths
