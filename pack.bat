@echo off
if "%1"=="" goto usage
setlocal DISABLEDELAYEDEXPANSION

:: 7z (official version) or 7-zip32 (undll + common archiver) can be used
if "%SEVENZIP%"=="" set SEVENZIP=7-zip32

:: Normal version
set execfiles=tcdll.tclock tclock.exe tcprop.exe
set pkgfiles=readme-kt.txt config-kt.txt format-kt.txt readme.html
set srcfiles=source pack.bat make_all.bat

if exist pkg rd /s /q pkg
mkdir pkg

rem "%SEVENZIP" a -mx=9 -m0=PPMd source.7z %srcfiles% -xr!out -xr!out64 -xr!work -xr!work64 -xr!*.bak -xr!*.old -xr!*.sw? -xr!*~ -xr!*.aps -xr!tags
"%SEVENZIP%" a -m0=PPMd:o=31:mem=25 source.7z %srcfiles% -xr!out -xr!out64 -xr!work -xr!work64 -xr!*.bak -xr!*.old -xr!*.sw? -xr!*~ -xr!*.aps -xr!tags
move /y source.7z pkg > nul

robocopy source\out   pkg\x86 %execfiles% > nul
robocopy source\out64 pkg\x64 %execfiles% > nul
robocopy .    pkg %pkgfiles% > nul

cd pkg
"%SEVENZIP%" a -mx=9 tclocklight-%1.zip %pkgfiles% x86 x64 source.7z
cd ..

goto end

:usage
echo.
echo usage: pack ^<version^>
echo.
echo ^<version^>: ex. kt090627

:end
