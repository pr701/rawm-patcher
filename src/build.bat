@echo off
setlocal

:: Find vswhere
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo ERROR: vswhere not found. Is Visual Studio installed?
    exit /b 1
)

:: Get VS install path
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -property installationPath`) do set "VSDIR=%%i"
if not defined VSDIR (
    echo ERROR: Visual Studio installation not found.
    exit /b 1
)

:: Init dev environment
call "%VSDIR%\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul 2>&1

:: Build
echo Building RAWMPatcher (Release x64)...
msbuild RAWMPatcher.sln /p:Configuration=Release /p:Platform=x64 /v:minimal
if errorlevel 1 (
    echo BUILD FAILED
    exit /b 1
)

echo.
echo Output: Build\Release\RAWMPatcher-x64.exe
