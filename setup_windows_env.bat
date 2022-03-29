@echo off
echo Setting up the MS Windows Qt environment...

set qt_path=C:\Qt\Qt5.12.12
set cmake_path="C:\Program Files\CMake\bin"

rem ==> ENVIRONMENT: CMake, MinGW
rem set PATH=%cmake_path%;%qt_path%\5.12.12\mingw73_64\bin;%qt_path%\Tools\mingw730_64\bin;%PATH%
rem <== ENVIRONMENT: MinGW

rem ==> ENVIRONMENT: Visual Studio
rem Generate with: cmake -G "Visual Studio 16 2019"
rem or: cmake -G "Visual Studio 15 2017"
set PATH=%cmake_path%;%qt_path%\5.12.12\msvc2017_64\bin;%PATH%
rem <== ENVIRONMENT: Visual Studio

echo Done
