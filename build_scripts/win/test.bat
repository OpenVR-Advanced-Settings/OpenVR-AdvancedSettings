@ECHO Starting 7zip:
"C:\Program Files\7-Zip\7z.exe" a -t7z Z:\Programming\Cpp\2018-09-12_OpenVR_AdvancedSettings\OpenVR-AdvancedSettings\\\bin\\win64\\AdvancedSettings\AdvancedSettings-2-8-0.7z Z:\Programming\Cpp\2018-09-12_OpenVR_AdvancedSettings\OpenVR-AdvancedSettings\\bin\\win64\\AdvancedSettings\*
IF ERRORLEVEL 1 EXIT /B 1
@ECHO 7zip done.
@ECHO Starting NSIS:
cd Z:\Programming\Cpp\2018-09-12_OpenVR_AdvancedSettings\NSISPortable\App\NSIS
makensis /V2 /WX Z:\Programming\Cpp\2018-09-12_OpenVR_AdvancedSettings\OpenVR-AdvancedSettings\installer\installer.nsi
IF ERRORLEVEL 1 EXIT /B 1
@ECHO NSIS done.
