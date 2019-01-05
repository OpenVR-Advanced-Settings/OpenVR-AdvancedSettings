@ECHO OFF
python build.py build
call test.bat
del test.bat
PAUSE
