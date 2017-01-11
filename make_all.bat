@echo off

echo ===============================
echo Building normal version
echo ===============================
cd source
"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
nmake /nologo %*
if errorlevel 1 goto error_exit
cd ..

