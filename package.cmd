echo on

cd %project_dir%\bin\win64\

echo Copying project files for archival...
copy "%project_dir%\Readme.md" "AdvancedSettings\Readme.md"
copy "%project_dir%\LICENSE" "AdvancedSettings\LICENSE.txt"
xcopy /Y /E /I "%project_dir%\src\res" "AdvancedSettings\res\"
copy "%project_dir%\src\restartvrserver.bat" "AdvancedSettings\restartvrserver.bat"
copy "%project_dir%\src\startdesktopmode.bat" "AdvancedSettings\startdesktopmode.bat"
copy "%project_dir%\src\qt.conf" "AdvancedSettings\qt.conf"
copy "%project_dir%\src\manifest.vrmanifest" "AdvancedSettings\manifest.vrmanifest"
copy "%project_dir%\third-party\openvr\bin\win64\openvr_api.dll" "AdvancedSettings\openvr_api.dll"

echo Packaging portable archive...
7z a AdvancedSettings.zip AdvancedSettings

echo Creating installer...
cd %project_dir%/installer
makensis installer.nsi
