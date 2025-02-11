@echo off

set qt_path=C:/Qt
set cmake_path="C:/Program Files/CMake/bin"
set PATH=%cmake_path%;%PATH%

rem [START] ENVIRONMENT: CMake, MinGW
rem How to install "make" on windows:
rem On windows 10 or 11, you can run the command "winget install ezwinports.make" in the
rem command line or powershell to quickly install it, then restart the command line or powershell.
rem Then you can run "make"!

rem echo Setting up Windows Qt environment for CMake and MinGW
set QT_DIR=C:/Qt/6.8.2/llvm-mingw_64/lib/cmake/Qt6
set PATH=C:/Qt/6.8.2/llvm-mingw_64/bin;C:/Qt/Tools/llvm-mingw1706_64/bin;%PATH%
rem [END]



rem [START] ENVIRONMENT: Microsoft Visual Studio
rem Generate with: cmake -G "Visual Studio 17 2022"
rem echo Setting up Windows Qt environment for MSVC
rem set QT_DIR=C:/Qt/6.8.2/msvc2022_64/lib/cmake/Qt6
rem set PATH=C:/Qt/6.8.2/msvc2022_64/bin;%PATH%
rem [END]

echo Done
