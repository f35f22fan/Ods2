# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.17.0") 
    # Disable automoc for .hh files:
    #cmake_policy(SET CMP0100 OLD)
endif()

include_directories(${Ods2_SOURCE_DIR})
link_directories(${CMAKE_BINARY_DIR}/lib)

# Set some Win32 Specific Settings
if(WIN32)
	set(GUI_TYPE "") #or WIN32 for no console output
endif (WIN32)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(hh_files
	examples1.hh
	examples2.hh
	util.hh
)

foreach(f IN LISTS hh_files)
	set_property(SOURCE ${f} PROPERTY SKIP_AUTOMOC ON)
endforeach()

set(src_files
	examples1.cc #examples1.hh
	examples2.cc #examples2.hh
	Invoice.cpp Invoice.hpp
	InvoiceItem.cpp InvoiceItem.hpp
	main.cpp
	util.cc #util.hh
)

list(APPEND ${src_files} ${hh_files})

foreach(f IN LISTS src_files)
	get_filename_component(b ${f} NAME)
	set_source_files_properties(${f} PROPERTIES
		COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

add_definitions(-DODS2_TEST_DIR="${Ods2_SOURCE_DIR}")
set (exe_name "app")
add_executable(${exe_name} ${GUI_TYPE} ${src_files})
#get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)
target_link_libraries(${exe_name} ods Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
) #${zlib_libs}

## enable_testing() seems to not make a difference
## since cmake builds the tests regardless
#enable_testing(true)
## As moc files are generated in the binary dir, tell CMake
## to always look for includes there:
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#macro(add_test testname testsrc)
#set(test_${testname}_SRCS ${testsrc})
#add_executable(test_${testname} ${test_${testname}_SRCS})
#target_link_libraries(test_${testname} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test ods)
#endmacro(add_test)

#include_directories(test)
#add_test(0_read_cells test/ReadCells.cpp test/ReadCells.hpp)
#add_test(1_write_out test/WriteOut.cpp test/WriteOut.hpp)

