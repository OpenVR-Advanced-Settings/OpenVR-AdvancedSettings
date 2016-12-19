;--------------------------------
;Include Modern UI

	!include "MUI2.nsh"

;--------------------------------
;General

	!define BASEDIR "bin\win64"

	;Name and file
	Name "OpenVR Advanced Settings"
	OutFile "OpenVR-AdvancedSettings-Installer.exe"
	
	;Default installation folder
	InstallDir "$PROGRAMFILES64\OpenVR-AdvancedSettings"
	
	;Get installation folder from registry if available
	InstallDirRegKey HKLM "Software\OpenVR-AdvancedSettings" ""
	
	;Request application privileges for Windows Vista
	RequestExecutionLevel admin

;--------------------------------
;Interface Settings

	!define MUI_ABORTWARNING

;--------------------------------
;Pages

	!insertmacro MUI_PAGE_LICENSE "LICENSE"
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES
  
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
	!insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Macros

!macro TerminateOverlay
	DetailPrint "Terminating OpenVR Advanced Settings overlay..."
	nsExec::ExecToLog '"taskkill" /F /IM AdvancedSettings.exe'
	Sleep 2000 ; give 2 seconds for the application to finish exiting
!macroend

;--------------------------------

;Installer Sections

Section "Install" SecInstall

	IfSilent install
		!insertmacro TerminateOverlay
  
	install:

	SetOutPath "$INSTDIR"
  
	RMDir /r "$INSTDIR\"

	;ADD YOUR OWN FILES HERE...
	File "LICENSE"
	File "${BASEDIR}\*.exe"
	File "${BASEDIR}\*.dll"
	File "${BASEDIR}\*.bat"
	File "${BASEDIR}\*.vrmanifest"
	File "${BASEDIR}\*.conf"
	File /r "${BASEDIR}\res"
	File /r "${BASEDIR}\qtdata"

	; Install redistributable
	ExecWait '"$INSTDIR\vcredist_x64.exe" /install /quiet'

	; Install the vrmanifest
	nsExec::ExecToLog '"$INSTDIR\AdvancedSettings.exe" -installmanifest'
  
	;Store installation folder
	WriteRegStr HKCU "Software\OpenVR-AdvancedSettings" "" $INSTDIR
  
	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenVRAdvancedSettings" "DisplayName" "OpenVR Advanced Settings"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenVRAdvancedSettings" "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""

	; If SteamVR is already running, execute the dashboard as the user
	FindWindow $0 "Qt5QWindowIcon" "SteamVR Status"
	StrCmp $0 0 +2
	Exec '"$INSTDIR\AdvancedSettings.exe"'

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

	!insertmacro TerminateOverlay

	; Remove the vrmanifest
	nsExec::ExecToLog '"$INSTDIR\AdvancedSettings.exe" -removemanifest'

	RMDir /r "$INSTDIR"

	DeleteRegKey /ifempty HKCU "Software\OpenVR-AdvancedSettings"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenVRAdvancedSettings"

SectionEnd

