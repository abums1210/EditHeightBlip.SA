@echo off
setlocal enabledelayedexpansion

set "MODE=%~1"

if "%MODE%"=="" (
    echo ========================================================
    echo  Build Options for EditHeightBlip [GTA-SA]:
    echo    [1] Local Bin - Personal - does not touch game dir
    echo    [2] Release   - Deploy to game scripts folder
    echo    [3] Debug     - Deploy to game scripts folder
    echo ========================================================
    set /p "CHOICE=Enter choice [1-3] default 1: "
    if "!CHOICE!"=="2" (
        set "MODE=release"
    ) else if "!CHOICE!"=="3" (
        set "MODE=debug"
    ) else (
        set "MODE=bin"
    )
)

if /i "%MODE%"=="1" set "MODE=bin"
if /i "%MODE%"=="2" set "MODE=release"
if /i "%MODE%"=="3" set "MODE=debug"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "tokens=*" %%i in ('"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe') do set MSBUILD=%%i

set "SIGNTOOL=C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x86\signtool.exe"
if not exist "%SIGNTOOL%" (
    for /d %%k in ("%ProgramFiles(x86)%\Windows Kits\10\bin\10.*") do (
        if exist "%%k\x86\signtool.exe" set "SIGNTOOL=%%k\x86\signtool.exe"
    )
)
set "CERT_FILE=%USERPROFILE%\CharlesVercetti.pfx"
set "CERT_PASS=CharlesVercetti2026"

if not exist "%~dp0..\logs" mkdir "%~dp0..\logs"
echo MSBuild Path: %MSBUILD%
echo Mode: %MODE%

if /i "%MODE%"=="bin" (
    echo Building Local Bin [game directories untouched]...
    "%MSBUILD%" "%~dp0EditHeightBlip.vcxproj" /p:Configuration="Release GTA-SA" /p:Platform="Win32" /p:OutDir="%~dp0bin\GTA-SA\Release\\" /m /fl /flp:logfile="%~dp0..\logs\editheightblip_sa_bin.log"
    call :sign_bin
) else if /i "%MODE%"=="release" (
    echo Building Release [deploying to game directories]...
    "%MSBUILD%" "%~dp0EditHeightBlip.vcxproj" /p:Configuration="Release GTA-SA" /p:Platform="Win32" /m /fl /flp:logfile="%~dp0..\logs\editheightblip_sa_release.log"
    call :sign_release
) else if /i "%MODE%"=="debug" (
    echo Building Debug [deploying to game directories]...
    "%MSBUILD%" "%~dp0EditHeightBlip.vcxproj" /p:Configuration="Debug GTA-SA" /p:Platform="Win32" /m /fl /flp:logfile="%~dp0..\logs\editheightblip_sa_debug.log"
) else (
    echo Unknown mode: %MODE%. Use bin, release, or debug.
    exit /b 1
)

exit /b 0

:sign_bin
if exist "%CERT_FILE%" if exist "%SIGNTOOL%" (
    echo Signing bin outputs with CharlesVercetti certificate...
    for %%f in ("%~dp0bin\GTA-SA\Release\*.asi") do (
        if exist "%%~f" (
            "%SIGNTOOL%" sign /f "%CERT_FILE%" /p "%CERT_PASS%" /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 "%%~f"
        )
    )
)
exit /b 0

:sign_release
if exist "%CERT_FILE%" if exist "%SIGNTOOL%" (
    echo Signing release outputs in game folders with CharlesVercetti certificate...
    for %%f in ("%GTA_SA_DIR%\scripts\EditHeightBlip.SA.asi") do (
        if exist "%%~f" (
            "%SIGNTOOL%" sign /f "%CERT_FILE%" /p "%CERT_PASS%" /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 "%%~f"
        )
    )
)
exit /b 0
