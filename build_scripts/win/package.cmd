@ECHO OFF
python build.py package
call test.bat
rm test.bat
PAUSE
