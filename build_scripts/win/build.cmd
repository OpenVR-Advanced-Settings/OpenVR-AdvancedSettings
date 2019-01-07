@ECHO OFF
python build.py build
IF ERRORLEVEL 1 EXIT /B 1
call current_build.bat
IF ERRORLEVEL 1 EXIT /B 1
del current_build.bat
IF ERRORLEVEL 1 EXIT /B 1
PAUSE
