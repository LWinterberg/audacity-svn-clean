; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Audacity 1.1
AppVerName=Audacity 1.1.3
AppPublisherURL=http://audacity.sourceforge.net
AppSupportURL=http://audacity.sourceforge.net
AppUpdatesURL=http://audacity.sourceforge.net
DefaultDirName={pf}\Audacity 1.1
DefaultGroupName=Audacity 1.1
AlwaysCreateUninstallIcon=yes
LicenseFile=E:\sfw_dev\audacity\LICENSE.txt
InfoBeforeFile=E:\sfw_dev\audacity\README.txt
; uncomment the following line if you want your installation to run on NT 3.51 too.
; MinVersion=4,3.51

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4

[Files]
Source: "E:\sfw_dev\audacity\win\Release\audacity.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\audacity-help.htb"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\bg\audacity.mo"; DestDir: "{app}\Languages\bg"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\ca\audacity.mo"; DestDir: "{app}\Languages\ca"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\da\audacity.mo"; DestDir: "{app}\Languages\da"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\da\wxstd.mo"; DestDir: "{app}\Languages\da"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\de\audacity.mo"; DestDir: "{app}\Languages\de"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\de\wxstd.mo"; DestDir: "{app}\Languages\de"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\es\audacity.mo"; DestDir: "{app}\Languages\es"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\es\wxstd.mo"; DestDir: "{app}\Languages\es"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\fr\audacity.mo"; DestDir: "{app}\Languages\fr"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\fr\wxstd.mo"; DestDir: "{app}\Languages\fr"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\hu\audacity.mo"; DestDir: "{app}\Languages\hu"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\hu\wxstd.mo"; DestDir: "{app}\Languages\hu"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\it\audacity.mo"; DestDir: "{app}\Languages\it"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\it\wxstd.mo"; DestDir: "{app}\Languages\it"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\ja\audacity.mo"; DestDir: "{app}\Languages\ja"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\nl\audacity.mo"; DestDir: "{app}\Languages\nl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\nl\wxstd.mo"; DestDir: "{app}\Languages\nl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\pl\audacity.mo"; DestDir: "{app}\Languages\pl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\pl\wxstd.mo"; DestDir: "{app}\Languages\pl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\pt\audacity.mo"; DestDir: "{app}\Languages\pt"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\ru\audacity.mo"; DestDir: "{app}\Languages\ru"; CopyMode: alwaysoverwrite
; removed file   Source: "E:\sfw_dev\audacity\win\Release\Languages\ru\wxstd.mo"; DestDir: "{app}\Languages\ru"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\sl\audacity.mo"; DestDir: "{app}\Languages\sl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Languages\sl\wxstd.mo"; DestDir: "{app}\Languages\sl"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\README.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\LICENSE.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\bug.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\dspprims.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\evalenv.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\follow.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\init.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\misc.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\nyinit.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\nyqmisc.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\nyquist.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\printrec.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\profile.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\seq.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\seqfnint.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\seqmidi.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\sndfnint.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\system.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\test.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Nyquist\xlinit.lsp"; DestDir: "{app}\Nyquist"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\analyze.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
; problematic so omit for now:   Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\beat.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\delay.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\fadein.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\fadeout.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\Freeverb2.dll"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\Freeverb-readme.txt"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\highpass.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\lowpass.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\pluck.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite
Source: "E:\sfw_dev\audacity\win\Release\Plug-Ins\tremolo.ny"; DestDir: "{app}\Plug-Ins"; CopyMode: alwaysoverwrite

[Icons]
Name: "{group}\Audacity 1.1"; Filename: "{app}\audacity.exe"
Name: "{userdesktop}\Audacity 1.1"; Filename: "{app}\audacity.exe"; MinVersion: 4,4; Tasks: desktopicon

[Run]
Filename: "{app}\audacity.exe"; Description: "Launch Audacity"; Flags: nowait postinstall skipifsilent

