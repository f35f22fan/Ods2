@echo off
echo Setting up the MS Windows Qt environment...

set qt_path=C:\Qt
set cmake_path="C:\Program Files\CMake\bin"

rem ==> ENVIRONMENT: CMake, MinGW
rem set QT_DIR=%qt_path%/6.8.2/mingw_64/lib/cmake/Qt6
rem set PATH=%cmake_path%;%qt_path%/6.8.2/mingw_64/bin;%qt_path%Tools/mingw1310_64/bin;%PATH%
rem <== ENVIRONMENT: MinGW

rem ==> ENVIRONMENT: Visual Studio
set QT_DIR=%qt_path%/6.8.2/msvc2022_64/lib/cmake/Qt6
rem Generate with: cmake -G "Visual Studio 17 2022"
set PATH=%cmake_path%;%qt_path%/6.8.2/msvc2022_64/bin;%PATH%
rem <== ENVIRONMENT: Visual Studio

echo Done
