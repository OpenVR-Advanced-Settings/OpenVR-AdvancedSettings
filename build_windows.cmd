echo on

SET project_dir="%cd%"

echo Set up environment...
set original_path=%PATH%
set PATH=%QT%\bin\;C:\Qt\Tools\QtCreator\bin\;C:\Qt\QtIFW2.0.1\bin\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %PLATFORM%

cd "%project_dir%
echo Building AdvancedSettings...
qmake -spec win32-msvc CONFIG+=x86_64 CONFIG-=debug CONFIG+=release
nmake

echo Packaging...
cd %project_dir%\bin\win64\
windeployqt --dir AdvancedSettings\qtdata --libdir AdvancedSettings --plugindir AdvancedSettings\qtdata\plugins --no-system-d3d-compiler --no-opengl-sw --release --qmldir ..\..\src\res\qml\ AdvancedSettings\AdvancedSettings.exe

rd /s /q AdvancedSettings\moc\
rd /s /q AdvancedSettings\obj\
rd /s /q AdvancedSettings\qrc\

echo Copying project files for archival...
copy "%project_dir%\Readme.md" "AdvancedSettings\Readme.md"
copy "%project_dir%\LICENSE" "AdvancedSettings\LICENSE.txt"
xcopy /E /I "%project_dir%\src\res" "AdvancedSettings\res\"
copy "%project_dir%\src\restartvrserver.bat" "AdvancedSettings\restartvrserver.bat"
copy "%project_dir%\src\startdesktopmode.bat" "AdvancedSettings\startdesktopmode.bat"
copy "%project_dir%\src\qt.conf" "AdvancedSettings\qt.conf"
copy "%project_dir%\src\manifest.vrmanifest" "AdvancedSettings\manifest.vrmanifest"
copy "%project_dir%\third-party\openvr\bin\win64\openvr_api.dll" "AdvancedSettings\openvr_api.dll"

echo Packaging portable archive...
7z a AdvancedSettings.zip AdvancedSettings
set PATH=%original_path%
cd %project_dir%
