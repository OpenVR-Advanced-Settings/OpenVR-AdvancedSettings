@ECHO OFF
python build.py deploy
call test.bat
del test.bat
PAUSE
