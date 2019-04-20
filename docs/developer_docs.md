# OpenVR Advanced Settings Developer Docs

For things that don't directly belong a single place in the source code.

# New Patch Release Checklist

## Increment Version Numbers

The version number string is kept in the `./build_scripts/compile_version_string.txt` file. The file is read by at compile time by whatever needs the current version number. Currently it is being used in 3 locations.

1. `advancedSettings.pro` - `VERSION` variable and `APPLICATION_VERSION` define.
2. `build_scripts\win\common.py` - Variable named `VERSION_STRING`.
3. `installer\installer.nsi` - Variable named `VERSION_STRING`.

The string should be three digits separated by dots (periods), as described in [semantic versioning](https://semver.org/) (like "2.8.0"). Additional qualifiers may be appended with a dash (and only a dash). The `QMAKE` build script relies on there only being dot separated integers before the first dash. If there are non-integers (or letters) before the first dash the Windows build will fail.

The AppVeyor build server will append the last 8 digits of the commit hash. This is in order to make debugging various versions easier. If `-release` is found in the original version string, `-release` will be removed and a hash will not be appended.
