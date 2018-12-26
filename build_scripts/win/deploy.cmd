@echo off
REM Echo off to avoid spam.

REM Non zero is failure, 0 is success.
SET exit_code_success=0
SET exit_code_failure_build_apps=1
SET exit_code_failure_build_locations=2

SET output_dir="\bin\win64\"

REM Used for echos.
SET current_activity=DEPLOY

REM If scripts are chain called the original dirs should not be overwritten.
IF NOT DEFINED original_dir (
    SET original_dir=%CD%
)
IF NOT DEFINED project_dir (
    REM Arg 0 (called %0) is the full path to the file.
    REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
    REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
    REM Project dir is up two dirs from the file.
    SET project_dir=%~dp0\..\..\
)

ECHO %current_activity%: Testing if all required build environment values are set:
IF NOT DEFINED QT_LOC (
    ECHO %current_activity%: QT_LOC not defined. Using default value.
    SET QT_LOC="C:\Qt\5.11.1\msvc2017_64\bin\"
)
REM QT_LOC is a directory, it can't be in the path, no reason to check using WHERE.
ECHO %current_activity%: QT_LOC set to '%QT_LOC%'.
IF EXIST %QT_LOC% (
    ECHO %current_activity%: QT_LOC exists.
) ELSE (
    ECHO %current_activity%: QT_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: All required build environment values are set.

IF NOT EXIST %project_dir%\%output_dir% (
    ECHO %current_activity%: Output dir '%output_dir%' does not exist. Creating.
    mkdir %project_dir%\%output_dir%
    ECHO %current_activity%: Output dir created.
)

cd %project_dir%\%output_dir%

IF DEFINED BUILD_DEBUG (
    ECHO %current_activity%: BUILD_DEBUG var set. Compiling for debug.
    SET compile_level=debug
) ELSE (
    ECHO %current_activity%: BUILD_DEBUG var not set. Compiling for release.
    SET compile_level=release
)

ECHO %current_activity%: Running windeployqt:

REM windeployqt requires the QT\bin location to be in the path.
REM Changing the PATH var is not ideal, but windeployqt does not work otherwise.
set original_path=%PATH%
set PATH=%QT_LOC%;%PATH%

REM Caret newline (^CFLR) splits a command over several lines.
REM This is done to avoid one ultra long line.

windeployqt --dir AdvancedSettings\qtdata ^
--libdir AdvancedSettings ^
--plugindir AdvancedSettings\qtdata\plugins ^
--no-system-d3d-compiler ^
--no-opengl-sw ^
--%compile_level% ^
--qmldir ..\..\src\res\qml\ ^
AdvancedSettings\AdvancedSettings.exe

REM windeployqt does not exit on error by itself.
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
REM Return the PATH to original as quickly as possible.
set PATH=%original_path%
ECHO %current_activity%: windeployqt done.

REM Switches used for copy:
REM /V Verifies that files have been copied correctly.
REM /Q Suppresses xcopy spam.
REM /Y Skips the overwrite yes prompt.
REM /E Copies all subdirs, even if they are empty.
REM /I "If Source is a directory or contains wildcards and Destination does not exist,
REM     xcopy assumes Destination specifies a directory name and creates a new directory.
REM     Then, xcopy copies all specified files into the new directory. By default,
REM     xcopy prompts you to specify whether Destination is a file or a directory."

ECHO %current_activity%: Copying necessary files.
copy /V /Y "%project_dir%\Readme.md" "%project_dir%\%output_dir%\AdvancedSettings\Readme.md"
ECHO %current_activity%: README copied.

REM All three licenses must be included in binary releases, as per the terms.
copy /V /Y "%project_dir%\LICENSE" "%project_dir%\%output_dir%\AdvancedSettings\LICENSE-GPL.txt"
copy /V /Y "%project_dir%\third-party\openvr\LICENSE" "%project_dir%\%output_dir%\AdvancedSettings\LICENSE-VALVE.txt"
copy /V /Y "%project_dir%\third-party\easylogging++\LICENSE" "%project_dir%\%output_dir%\AdvancedSettings\LICENSE-MIT.txt"
ECHO %current_activity%: LICENSE files copied.

xcopy /Q /Y /E /I /V "%project_dir%\src\res" "%project_dir%\%output_dir%\AdvancedSettings\res\"
ECHO %current_activity%: Resource files copied.

copy /V /Y "%project_dir%\src\restartvrserver.bat" "%project_dir%\%output_dir%\AdvancedSettings\restartvrserver.bat"
copy /V /Y "%project_dir%\src\startdesktopmode.bat" "%project_dir%\%output_dir%\AdvancedSettings\startdesktopmode.bat"
ECHO %current_activity%: Extra batch files copied.

copy /V /Y "%project_dir%\src\qt.conf" "%project_dir%\%output_dir%\AdvancedSettings\qt.conf"
copy /V /Y "%project_dir%\src\manifest.vrmanifest" "%project_dir%\%output_dir%\AdvancedSettings\manifest.vrmanifest"
ECHO %current_activity%: VR specific configuration files copied.

copy /V /Y "%project_dir%\third-party\openvr\bin\win64\openvr_api.dll" "%project_dir%\%output_dir%\AdvancedSettings\openvr_api.dll"
ECHO %current_activity%: OpenVR .dll copied.

cd %original_dir%

ECHO %current_activity%: %current_activity% DONE.