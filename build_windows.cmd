@echo off
REM Echo off to avoid spam.

SET exit_code_success=0
SET exit_code_failure_build_apps=1
SET exit_code_failure_build_locations=2

REM Used for echos.
SET top_level_activity=MASTER

REM If scripts are chain called the original dirs should not be overwritten.

SET original_dir=%CD%


REM Arg 0 (called %0) is the full path to the file.
REM https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/call#batch-parameters
REM As per above link, %~dp0 expands the zeroth arg to a drive letter and path only.
SET folder_path=%~dp0
REM Project dir is up two dirs from the file.
SET project_dir=%folder_path%


ECHO %top_level_activity%: Calling build script.
%PYTHON_LOC%python %project_dir%\build_scripts\win\build.py build
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
call %project_dir%\build_scripts\win\current_build.bat
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%

REM The average dev doesn't need to package and zip it, just build it and test.
IF NOT DEFINED BUILD_PACKAGE (
    ECHO %top_level_activity%: BUILD_PACKAGE not set. Not building installer and portable zip versions.
    ECHO %top_level_activity%: MASTER DONE.
    EXIT /B %exit_code_success%
)
ECHO %top_level_activity%: Calling packaging script.
%PYTHON_LOC%python %project_dir%\build_scripts\win\build.py package
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%
call %project_dir%\build_scripts\win\current_build.bat
IF ERRORLEVEL 1 EXIT /B %exit_code_failure_build_apps%

CD %original_dir%

ECHO %top_level_activity%: MASTER DONE.
