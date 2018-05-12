echo on

SET project_dir="%cd%"

echo Set up environment...
set original_path=%PATH%
set PATH=%QT%\bin\;C:\Qt\Tools\QtCreator\bin\;C:\Qt\QtIFW2.0.1\bin\;C:\Program Files (x86)\NSIS\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %PLATFORM%

cd "%project_dir%
echo Building AdvancedSettings...
qmake -spec win32-msvc CONFIG+=x86_64 CONFIG-=debug CONFIG+=release
nmake

echo Packaging...
mkdir %project_dir%\bin\win64\
cd %project_dir%\bin\win64\
windeployqt --dir AdvancedSettings\qtdata --libdir AdvancedSettings --plugindir AdvancedSettings\qtdata\plugins --no-system-d3d-compiler --no-opengl-sw --release --qmldir ..\..\src\res\qml\ AdvancedSettings\AdvancedSettings.exe

rd /s /q AdvancedSettings\moc\
rd /s /q AdvancedSettings\obj\
rd /s /q AdvancedSettings\qrc\

set PATH=%original_path%
cd %project_dir%
