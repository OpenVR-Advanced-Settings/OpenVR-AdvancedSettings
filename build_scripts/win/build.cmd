@ECHO OFF
python build.py build
call test.bat
rm test.bat
PAUSE
