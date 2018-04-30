#cmake -G "MinGW Makefiles"
cmake_minimum_required(VERSION 2.8.12)
Project(Ods2)

message("Source dir: " ${Ods2_SOURCE_DIR})

#only build zlib on windows because unix has it by default
IF(WIN32)
	include_directories(${Ods2_SOURCE_DIR}/zlib)
	link_directories(${Ods2_BINARY_DIR}/lib)
	set_property(GLOBAL PROPERTY zlib_libs_property "zlib")
ELSE (WIN32)
	find_package(ZLIB REQUIRED)
	if (ZLIB_FOUND)
    	include_directories(${ZLIB_INCLUDE_DIRS})
		set_property(GLOBAL PROPERTY zlib_libs_property "${ZLIB_LIBRARIES}")
	endif(ZLIB_FOUND)
ENDIF (WIN32)

#if(WIN32)
#    set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.7.1\\5.7\\msvc2015_64\\")
#endif (WIN32)

set(LIBRARY_OUTPUT_PATH "${Ods2_BINARY_DIR}/lib")

#set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(ODS_STATIC "1")
	set(QUAZIP_STATIC "1")
	set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

if (WIN32)
	add_subdirectory(zlib)
endif(WIN32)

add_subdirectory(quazip)
add_subdirectory(ods)
add_subdirectory(sample)
