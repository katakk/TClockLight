@echo off

cd source
nmake /nologo %*
if errorlevel 1 goto error_exit
cd ..
