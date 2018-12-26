@echo off
REM Echo off to avoid spam.

REM Used for echos.
SET top_level_activity=MASTER

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
    SET project_dir=%folder_path%
)

ECHO %top_level_activity%: Calling build script.
CALL %project_dir%\build_scripts\win\build_windows.cmd
IF ERRORLEVEL 1 EXIT /B 1

ECHO %top_level_activity%: Calling deployment script.
CALL %project_dir%\build_scripts\win\deploy_windows.cmd
IF ERRORLEVEL 1 EXIT /B 1

ECHO %top_level_activity%: Calling packaging script.
CALL %project_dir%\build_scripts\win\package_windows.cmd
IF ERRORLEVEL 1 EXIT /B 1

CD %original_dir%

ECHO %top_level_activity%: MASTER DONE.