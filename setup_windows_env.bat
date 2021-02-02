@echo off
echo Setting up the Window$ Qt environment...


	rem ==> ENVIRONMENT: MinGW
set PATH=C:\Qt\5.15.2\mingw81_64\bin;%PATH%

set PATH=C:\Qt\Tools\mingw810_64\bin;%PATH%
	rem IMPORTANT: also rename mingw32-make.exe
	rem to make.exe from the folder above (...tools/mingw/bin)
	rem so that the system can find the make.exe tool.

	rem Dir with cmake.exe:
set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%
	rem <== ENVIRONMENT: MinGW



	rem ==> ENVIRONMENT: Visual Studio 2013 32bit
rem set PATH=C:\Qt\Qt5.3.2\5.3\msvc2013_opengl\bin;%PATH%

	rem Dir with cmake.exe:
rem set PATH=C:\Program Files\CMake\bin;%PATH%
	rem <== ENVIRONMENT: Visual Studio 2013 32bit



rem ==> ENVIRONMENT: Visual Studio 2019 64bit
rem Generate with: cmake -G "Visual Studio 16 2019" ..
rem set PATH=C:\Qt\5.15.2\msvc2019_64\bin;%PATH%

rem Dir with cmake.exe:
rem set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%
rem <== ENVIRONMENT: Visual Studio 2019 64bit



echo Done
