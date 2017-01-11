@echo off

echo ===============================
echo Building normal version
echo ===============================
cd source
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
nmake /nologo clean
nmake /nologo
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
nmake /nologo
if errorlevel 1 goto error_exit
cd ..

pause