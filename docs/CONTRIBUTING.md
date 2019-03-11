<a name="top"></a>
# How to contribute to OpenVR Advanced Settings

 > * [You found a bug or have a suggestion for improvement](#bug_improvement)
 > * [You wrote a patch](#patch)
 >   * [Building](#building)
 >   * [Formatting](#formatting)
 >   * [Style guide](#style)
 > * [Additional License Requirements](#License)

<a name="bug_improvement"></a>
## You found a bug or have a suggestion for improvement

First, update to the newest release.

If the issue persists, look through the [issues page](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/issues) and see if there are any open issues relevant to yours. If there are, describe your issue in detail and why you think it's relevant to the already open issue. 

If there aren't any open issues relevant to yours, create a new issue and give as much detail as possible.

For bug reports include what you expect to happen, what happened, version number of the application, operating system, SteamVR version number (if possible) and any other information you find relevant.

For improvement requests include what you're expecting to do, why you want the feature this way, the problem that you're solving and any other information that you find relevant.

Include logs if possible. Logs can be found in `%APPDATA%\AdvancedSettings-Team\AdvancedSettings.log` on Windows.

<a name="patch"></a>
## You want to submit changes

<a name="building"></a>
### Building

If you haven't already, read the instructions for how to build on [Windows](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/tree/master#building_on_windows) or [Linux](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/tree/master#building_on_linux). Your patch must build on all platforms without errors (warnings are treated as errors). Notice that different compilers have different warning messages, being able to successfully compile on one does not mean that all of them will pass. The pull request will be built on both Windows (msvc and clang-msvc) and Linux (gcc and clang) CI servers.

<a name="formatting"></a>
### Formatting

This project uses `clang-format` (part of the [LLVM collection](https://releases.llvm.org/download.html)) on all `*.cpp`and `*.h` files in the `src` directory, using the supplied `.clang-format` file in the root of the repository. Correct formatting is checked by the CI servers.

The `.\build_scripts\win\format.cmd` (for Windows) and `./build_scripts/linux/format` (for Linux) scripts will automatically run `clang-format` on all required files.

The Windows script requires that either your `clang-format.exe` is in `C:\Program Files\LLVM\bin\clang-format.exe` or the `LLVM_LOC` environment variable is set to the LLVM bin directory.

The Linux script requires that `clang-format` is in your `path`.

There are `clang-format` plugins for many IDEs, including  [Qt Creator](https://doc.qt.io/qtcreator/creator-beautifier.html) and [Visual Studio](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).

To verify that your patch has been formatted correctly, run `.\build_scripts\win\verify_formatting.cmd` on Windows or `./build_scripts/linux/verify_formatting` on Linux. If there are formatting changes needed the script will output a diff showing which changes are needed, otherwise it will output `FORMATTED CORRECTLY.`. The verification script requires `python` in the `path` for Linux and Windows. On Windows the verification script will either use the `LLVM_LOC` environment variable or the default location.

<a name="style"></a>
### Style guide

Currently, there are no directives for style other than `clang-format`.

<a name="License"></a>
## Licensing

By submitting a Pull Request to this Repo you are also granting a [GNU Lesser General Public License 3.0](https://opensource.org/licenses/lgpl-3.0.html) for your contributions to the project for the github user https://github.com/ykeara/ for the purpose of distributing on platforms such as Steam. If you have any questions or concerns, about this clause please email ykeara1@gmail.com BEFORE you submit your pull request.
