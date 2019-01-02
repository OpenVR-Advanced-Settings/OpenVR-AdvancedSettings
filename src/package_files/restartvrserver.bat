@echo off

REM First (optional) parameter is the vr runtime path
REM When no parameter given use default steam install location
IF [%1] == [] (
	SET "steamVRPath=C:\Program Files (x86)\Steam\steamapps\common\SteamVR\"
) ELSE (
	SET "steamVRPath=%~1"
)
echo VR Runtime Path: %steamVRPath%
echo.

REM Kill vrmonitor.exe and give SteamVR some time for a graceful shutdown
REM When no process was killed then exit (helps to prevent the taskkill clone bug)
(taskkill /im vrmonitor.exe /f >nul 2>nul && echo Give SteamVR some time for a graceful shutdown ... && timeout /t 20) || exit

REM Forcefully kill all remaining processes
taskkill /im vrcompositor.exe /f >nul 2>nul
taskkill /im vrdashboard.exe /f >nul 2>nul
taskkill /im vrserver.exe /f >nul 2>nul

REM Start vrmonitor.exe
REM start "" "%steamVRPath%\bin\win32\vrmonitor.exe"
start "" "%steamVRPath%\bin\win32\vrstartup.exe"
