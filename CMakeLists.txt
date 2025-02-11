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

if(MSVC)
	# This flag allows MSVC to compile variadic function arguments when they're omitted
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:preprocessor")
endif()

find_package(QT NAMES Qt6 COMPONENTS Core Gui REQUIRED)
find_package(Qt6 COMPONENTS Core Gui REQUIRED) #5.15
# if (QT_VERSION_MAJOR EQUAL 6)
#     find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
# endif()

set(LIBRARY_OUTPUT_PATH "${Ods2_BINARY_DIR}/lib")

#set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(ODS_STATIC "1")
	#set(QUAZIP_STATIC "1")
	#set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

#if (WIN32)
	add_subdirectory(zlib SYSTEM)
#endif()
add_subdirectory(libzip)
add_subdirectory(ods)
add_subdirectory(examples)
