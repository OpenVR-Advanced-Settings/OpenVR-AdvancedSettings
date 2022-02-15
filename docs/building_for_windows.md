# Building for Windows


 > * [Requirements](#requirements)
 > * [Locations and Environment Variables](#locs_and_envs)
 > * [Building](#features)
 > * [Contributing](#contributing)
 > * [Using with QtCreator](#qt_creator)
 > * [Using with Visual Studio](#visual_studio)

<a name="requirements"></a>
## Requirements

For compiling:

1. [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads/) (tested on 2017 Community)
2. [Qt Framework](https://www.qt.io/download) version equal to or greater than 5.12.
3. [Python 3](https://www.python.org/downloads/) (must be in `PATH` environment variable)

For pushing changes to the repo:


2. Clang format. Part of the [LLVM collection](https://releases.llvm.org/download.html).
 - **note** use version 9.0.1/9.0.0 of LLVM

Additionally, for fully building portable and installer release versions:

3. [7zip](https://www.7-zip.org/download.html)

4. [Nullsoft Scriptable Install System (NSIS)](https://sourceforge.net/projects/nsis/)


<a name="locs_and_envs"></a>
## Locations and Environment Variables

The following environmental variables are relevant for building the project.

| Environment Variable  | Purpose |
| --------------------  | ------------- |
| `QT_LOC`              | Path to your Qt `\bin\` folder.    |
| `VS_LOC`              | Path to your Visual Studio `vcvarsall.bat` file.  |
| `JOM_LOC`             | Path to an `.exe` of `jom.exe`. Usually installed with Qt. `jom` is a wrapper around `nmake` allowing more threads to be used. It is not required, but using `nmake`is slower. If `JOM_LOC` isn't defined, or the `.exe` can't be found `nmake` will be used by default. To force using `nmake`, set `JOM_LOC` to a non-valid path like `1`.  |
| `ZIP_LOC`             | Path to your 7zip `.exe`. Both portable and installer will work.  |
| `NSIS_LOC`            | Path to your installation of NSIS containing the `makensis` file. Both portable and installer will work. |
| `LLVM_LOC`            | Path to your LLVM bin directory containing  `clang-format.exe`. Not necessary for building with the MSVC compiler, but necessary for pushing changes to the repo. Leave as empty string if your `clang-format.exe` and `clang-cl.exe` are in the path. |
| `PYTHON_LOC`          | A non-error-checked way of referring to a specific python installation. Must end on a backslash or be undefined. If undefined `python` must be in the path. |

If an environment variable isn't set a default value will be provided. The default values are shown in the table below.

| Environment Variable  | Default Value |
| --------------------  | ------------- |
| `QT_LOC`              | No default value. Will traverse the filesystem looking for "DRIVE_LETTER:\Qt\VERSION\msvc2017\bin\". Will use the highest value found unless this variable is set. |
| `VS_LOC`              | `"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"`  |
| `JOM_LOC`             | `"C:\Qt\Tools\QtCreator\bin\jom.exe"`|
| `ZIP_LOC`             | `"C:\Program Files\7-Zip\7z.exe"`    |
| `NSIS_LOC`            | `"C:\Program Files (x86)\NSIS\"`     |
| `LLVM_LOC`            | `"C:\Program Files\LLVM\bin\"`       |
| `PYTHON_LOC`            | NONE SUPPLIED. It is expected that `python` is in the path if `PYTHON_LOC` is defined.|

Additionally, these optional environment variables are supported.

| Environment Variable  | Purpose |
| --------------------  | ------------- |
| `BUILD_DEBUG`         | If set, a debug version will be built. The packaging script will not run with this var set.   |
| `BUILD_VSPROJ`        | If set, Visual Studio project files will be built.  |
| `BUILD_PACKAGE`       | If set, the script will create a zipped portable version and installer `.exe`. Unset by default since only the build server really needs to create zips and installer versions. |

Remember that there's a difference between an unset variable and a variable set to an empty string. `SET BUILD_DEBUG=""` does not unset the variable. You need to write `SET BUILD_DEBUG=`.

The `.dll` files necessary for a release build differ from those needed by a debug build. Therefore you need to make sure you run all scripts with `BUILD_DEBUG` either defined or undefined, depending on your needs.

<a name="building"></a>
## Building

With the programs above installed and environment variables set, go into the root folder of the repository and run `build_windows.cmd`. Compiled binaries along with everything needed to run portable will output to `.\bin\win64\AdvancedSettings\`.

<a name="contributing"></a>
## Contributing

For full details, see [CONTRIBUTING.md](CONTRIBUTING.md).

<a name="qt_creator"></a>
## Using with QtCreator

Run `build_windows.cmd` once and fully build (packaging not necessary) the application. Open the `.pro` file in Qt Creator. A build and toolchain option should be marked in Qt Creator, with all others unmarked. If it isn't, set the directory to be the root repo directory. You should now be able to compile and run in Qt Creator without issues.

Notice that you may need to redeploy library files if you make large changes (run `build_scripts\win\deploy.cmd`).

Additionally, you can set Qt Creator to run `build_scripts\win\deploy.cmd` on compilation in order to ensure that the correct libraries are present. Remember to set or unset `BUILD_DEBUG` depending on your needs.

<a name="visual_studio"></a>
## Using with Visual Studio

Run `build_windows.cmd` once and fully build (packaging not necessary) the application with the `BUILD_VSPROJ` environment variable set. Open the newly created project file. If you compiled a release build (`BUILD_DEBUG` not set) change the build type to release in Visual Studio. You should be able to compile and run with Visual Studio without issues.

Notice that any new files you add or compiler settings you change will not be backported to the `.pro` file. Hence it is necessary to manually change the `.pro` file.

Notice that you may need to redeploy library files if you make large changes (run `build_scripts\win\deploy.cmd`).

Additionally, you can set Visual Studio to run `build_scripts\win\deploy.cmd` on compilation in order to ensure that the correct libraries are present. Remember to set or unset `BUILD_DEBUG` depending on your needs.