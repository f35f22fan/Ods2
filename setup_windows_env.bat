@echo off
echo Setting up the Window$ Qt environment...


rem ==> ENVIRONMENT: MinGW
set PATH=C:\Qt\Qt5.12.9\5.12.9\mingw73_64\bin;%PATH%

set PATH=C:\Qt\Qt5.12.9\Tools\mingw730_64\bin;%PATH%
rem IMPORTANT: also rename mingw32-make.exe
rem to make.exe from the folder above (...tools/mingw/bin)
rem so that the system can find the make.exe tool.

rem Dir with cmake.exe:
set PATH=C:\Program Files\CMake\bin;%PATH%
rem <== ENVIRONMENT: MinGW



rem ==> ENVIRONMENT: Visual Studio 2013 32bit
rem set PATH=C:\Qt\Qt5.3.2\5.3\msvc2013_opengl\bin;%PATH%

rem Dir with cmake.exe:
rem set PATH=C:\Program Files\CMake\bin;%PATH%
rem <== ENVIRONMENT: Visual Studio 2013 32bit



rem ==> ENVIRONMENT: Visual Studio 2015 64bit
rem set PATH=C:\Qt\Qt5.7.1\5.7\msvc2015_64\bin;%PATH%

rem Dir with cmake.exe:
rem set PATH=C:\Program Files\CMake\bin;%PATH%
rem <== ENVIRONMENT: Visual Studio 2013 64bit



echo Done
