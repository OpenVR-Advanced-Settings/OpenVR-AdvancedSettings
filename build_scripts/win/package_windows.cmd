@echo off
REM Echo off to avoid spam.

REM Non zero is failure, 0 is success.
SET exit_code_success=0
SET exit_code_failure_build_apps=1
SET exit_code_failure_build_locations=2

SET output_dir="\bin\win64\"

REM Used for echos.
SET current_activity=PACKAGE

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

REM ZIP_LOC can't be called 7Z_LOC because of Windows limitations.
ECHO %current_activity%: Testing if all required build environment values are set:
IF NOT DEFINED ZIP_LOC (
    ECHO %current_activity%: ZIP_LOC not defined. Using default value.
    SET ZIP_LOC="C:\Program Files\7-Zip\7z.exe"
)
ECHO %current_activity%: ZIP_LOC set to '%ZIP_LOC%'.
IF EXIST %ZIP_LOC% (
    ECHO %current_activity%: ZIP_LOC exists.
) ELSE (
    ECHO %current_activity%: ZIP_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: Testing if all required build environment values are set:
IF NOT DEFINED NSIS_LOC (
    ECHO %current_activity%: NSIS_LOC not defined. Using default value.
    SET NSIS_LOC="C:\Program Files (x86)\NSIS\"
)
ECHO %current_activity%: NSIS_LOC set to '%NSIS_LOC%'.
IF EXIST %NSIS_LOC% (
    ECHO %current_activity%: NSIS_LOC exists.
) ELSE (
    ECHO %current_activity%: NSIS_LOC directory does not exist. Exiting.
    EXIT /B %exit_code_failure_build_locations%
)

ECHO %current_activity%: All required build environment values are set.

ECHO %current_activity%: Running 7zip:
%ZIP_LOC% a %project_dir%\%output_dir%\AdvancedSettings.zip %project_dir%\%output_dir%\AdvancedSettings\*
ECHO %current_activity%: 7zip done.

ECHO %current_activity%: Running NSIS:
REM Trying to launch makensis without CD into the dir does not work for some reason.
CD %NSIS_LOC%
REM /V2 is show warnings and errors, /WX is treat warnings as errors.
makensis /V2 /WX %project_dir%\installer\installer.nsi
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
ECHO %current_activity%: NSIS done.

CD %original_dir%

ECHO %current_activity%: %current_activity% DONE.