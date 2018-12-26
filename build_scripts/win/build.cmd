@echo off
REM Echo off to avoid spam.

REM Non zero is failure, 0 is success.
SET exit_code_success=0
SET exit_code_failure_build_apps=1
SET exit_code_failure_build_locations=2

REM Used for echos.
SET current_activity=BUILD

REM If scripts are chain called the original dirs should not be overwritten.
IF NOT DEFINED original_dir (
    SET original_dir=%CD%
)
IF NOT DEFINED project_dir (
    REM Arg 0 (called %0) is the full path to the file.
    REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
    REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
    SET folder_path=%~dp0
    REM Project dir is up two dirs from the file.
    SET project_dir=%folder_path%\..\..\
)

REM Change dir to the project dir, otherwise qmake doesn't know where it is.
CD %project_dir%

ECHO %current_activity%: Testing if all required build environment values are set:
IF NOT DEFINED QT_LOC (
    ECHO %current_activity%: QT_LOC not defined. Using default value.
    SET QT_LOC="C:\Qt\5.11.1\msvc2017_64\bin\"
)
ECHO %current_activity%: QT_LOC set to '%QT_LOC%'.
IF EXIST %QT_LOC% (
    ECHO %current_activity%: QT_LOC exists.
) ELSE (
    ECHO %current_activity%: QT_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

IF NOT DEFINED VS_LOC (
    ECHO %current_activity%: VS_LOC not defined. Using default value.
    SET VS_LOC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
)
ECHO %current_activity%: VS_LOC set to '%VS_LOC%'.
IF EXIST %VS_LOC% (
    ECHO %current_activity%: VS_LOC exists.
) ELSE (
    ECHO %current_activity%: VS_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: All required build environment values are set.

IF DEFINED BUILD_DEBUG (
    ECHO %current_activity%: BUILD_DEBUG var set. Compiling for debug.
    SET compile_level=debug
) ELSE (
    ECHO %current_activity%: BUILD_DEBUG var not set. Compiling for release.
    SET compile_level=release
)

ECHO %current_activity%: Setting up environment with Visual Studio 'vcvarsall.bat'.
REM Currently only this platform is supported.
SET target_platform=AMD64
REM Set up environment for qmake.
CALL %VS_LOC% %target_platform%

IF DEFINED BUILD_VSPROJ (
    ECHO %current_activity%: BUILD_VSPROJ defined. Building Visual Studio project files.
    %QT_LOC%\qmake -tp vc
    ECHO %current_activity%: Building Visual Studio project files done.
) ELSE (
    ECHO %current_activity%: BUILD_VSPROJ not defined. Not building Visual Studio project files.
)

ECHO %current_activity%: Running qmake for %compile_level%:
%QT_LOC%\qmake -spec win32-msvc CONFIG+=x86_64 CONFIG+=%compile_level%
ECHO %current_activity%: qmake done.

IF NOT DEFINED JOM_LOC (
    ECHO %current_activity%: JOM_LOC not defined. Using default value.
    SET JOM_LOC="C:\Qt\Tools\QtCreator\bin\jom.exe"
)
ECHO %current_activity%: JOM_LOC set to '%JOM_LOC%'.

IF EXIST %JOM_LOC% (
    ECHO %current_activity%: JOM_LOC exists.
    ECHO %current_activity%: Running jom: 
    %JOM_LOC%
    REM jom will not exit with an error code. Leading to a passing build even with errors.
    IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
    ECHO %current_activity%: jom done. 
) ELSE (
    ECHO JOM_LOC directory does not exist. Using nmake.
    ECHO %current_activity%: Running nmake: 
    nmake
    REM nmake will not exit with an error code. Leading to a passing build even with errors.
    IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
    ECHO %current_activity%: nmake done.
)

CD %original_dir%

ECHO %current_activity%: %current_activity% DONE.