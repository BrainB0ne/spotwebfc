; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Spotweb Filter Creator"
#define MyAppVersion "0.6.0"
#define MyAppPublisher "SpotwebFC"
#define MyAppURL "https://www.brainbytez.nl/spotweb-filter-creator/"
#define MyAppExeName "spotwebfc.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EB933651-A7FA-4E49-B606-62CAF2B4650D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=license.txt
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=spotwebfc-{#MyAppVersion}-win32-setup
SetupIconFile=logo.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "logo.ico"; DestDir: "{app}"; Flags: ignoreversion

Source: "Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Xml.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libGLESv2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "iconengines\qsvgicon.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion

Source: "imageformats\qgif.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qicns.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qico.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qsvg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qtga.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qtiff.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qwbmp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "imageformats\qwebp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion

Source: "platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion

Source: "styles\qwindowsvistastyle.dll"; DestDir: "{app}\styles"; Flags: ignoreversion

Source: "spotwebfc.cxml"; DestDir: "{app}"; Flags: ignoreversion
Source: "spotwebfc.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "spotwebfc.lng"; DestDir: "{app}"; Flags: ignoreversion
Source: "spotwebfc_de.qm"; DestDir: "{app}"; Flags: ignoreversion
Source: "spotwebfc_fr.qm"; DestDir: "{app}"; Flags: ignoreversion
Source: "spotwebfc_nl.qm"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

