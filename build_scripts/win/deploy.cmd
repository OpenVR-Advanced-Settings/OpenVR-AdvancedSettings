@ECHO OFF
python build.py deploy
call test.bat
rm test.bat
PAUSE
