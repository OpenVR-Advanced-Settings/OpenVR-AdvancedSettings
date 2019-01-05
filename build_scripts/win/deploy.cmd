@ECHO OFF
python build.py deploy
IF ERRORLEVEL 1 EXIT /B 1
call current_build.bat
IF ERRORLEVEL 1 EXIT /B 1
del current_build.bat
IF ERRORLEVEL 1 EXIT /B 1
PAUSE
