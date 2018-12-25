# Building for Windows


 > * [Requirements](#requirements)
 > * [Locations and Environment Variables](#locs_and_envs)
 > * [Building](#features)

<a name="requirements"></a>
## Requirements

For compiling:

1. Microsoft Visual Studio (tested on 2017 Community)
2. Qt Framework (tested on 5.10 and 5.11)

Additionally, for fully building portable and installer release versions:
3. 7zip Command Line
4. Nullsoft Scriptable Install System (NSIS)

<a name="locs_and_envs"></a>
## Locations and Environment Variables

Visual Studio must be installed in the default location of `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community`.

The `QT` environment variable must be set to the location of your Qt installation (for example `C:\Qt\5.11.1\msvc2017_64`).

The `PLATFORM` environment variable must be set to `amd64`.

For building the portable and installer release versions, the locations of 7zip Command Line (`7z.exe`) and NSIS (`makensis.exe`) must be in the `PATH` environment variable.

<a name="building"></a>
## Building

With the programs above installed and environment variables set, go into the root folder of the repository and first run `build_windows.cmd` and then `package.cmd`. Compiled binaries along with everything needed to run portable will output to `.\bin\win64\AdvancedSettings\`.
