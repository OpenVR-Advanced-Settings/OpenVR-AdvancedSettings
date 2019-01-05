@ECHO OFF
python build.py package
call test.bat
del test.bat
PAUSE
