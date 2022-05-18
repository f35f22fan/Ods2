message(STATUS ${CMAKE_CURRENT_LIST_FILE})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(${Ods2_SOURCE_DIR})
link_directories(${Ods2_BINARY_DIR})

# set all include directories for in and out of source builds
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${ZLIB_INCLUDE_DIRS}
)

file(GLOB_RECURSE src_files
	"*.h" "*.hh" "*.hpp" "*.hxx"
	"*.c" "*.cc" "*.cpp")

if (DEFINED QUAZIP_STATIC)
	set(lib_type STATIC)
else (DEFINED QUAZIP_STATIC)
	set(lib_type SHARED)
	add_definitions(-DQUAZIP_SHARED -DQUAZIP_BUILD)
endif (DEFINED QUAZIP_STATIC)

add_library(quazip ${lib_type} ${src_files})
get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)
target_link_libraries(quazip Qt${QT_VERSION_MAJOR}::Core ${zlib_libs})
if (QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(quazip Qt6::Core5Compat)
endif()

