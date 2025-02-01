#cmake -G "MinGW Makefiles"
cmake_minimum_required(VERSION 3.15)
Project(Ods2)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
endif()

find_package(QT NAMES Qt6 COMPONENTS Core Gui REQUIRED)
find_package(Qt6 COMPONENTS Core Gui REQUIRED) #5.15
# if (QT_VERSION_MAJOR EQUAL 6)
#     find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
# endif()

#only build zlib on windows because unix has it by default
#IF(WIN32)
#	include_directories(${Ods2_SOURCE_DIR}/zlib)
#	link_directories(${Ods2_BINARY_DIR}/lib)
#	set_property(GLOBAL PROPERTY zlib_libs_property "zlib")
#ELSE (WIN32)
#	find_package(ZLIB REQUIRED)
#	if (ZLIB_FOUND)
#    	include_directories(${ZLIB_INCLUDE_DIRS})
#		set_property(GLOBAL PROPERTY zlib_libs_property "${ZLIB_LIBRARIES}")
#	endif(ZLIB_FOUND)
#ENDIF (WIN32)

if(WIN32)
	set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.12.9\\5.12.9\\mingw73_64\\")
#    set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.7.1\\5.7\\msvc2015_64\\")
endif (WIN32)

set(LIBRARY_OUTPUT_PATH "${Ods2_BINARY_DIR}/lib")

#set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(ODS_STATIC "1")
	#set(QUAZIP_STATIC "1")
	set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

#if (WIN32)
	add_subdirectory(zlib)
#endif(WIN32)

add_subdirectory(quazip)
add_subdirectory(ods)
add_subdirectory(examples)
