cmake_minimum_required (VERSION 3.5)
set (PROJECT_NAME tds2pdb)
project (${PROJECT_NAME} CXX)

#------------------
# Project
#------------------

# Starting version.
set (TDS2PDB_VERSION_MAJOR 1)
set (TDS2PDB_VERSION_MINOR 0)

# Generated header.
configure_file (
  "${PROJECT_SOURCE_DIR}/src/tds2pdb.hpp.in"
  "${PROJECT_BINARY_DIR}/tds2pdb.hpp")

# Add the binary tree to the search path for include files.
include_directories("${PROJECT_BINARY_DIR}")

# Source of the main project.
file (GLOB PROJECT_SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable (${PROJECT_NAME} ${PROJECT_SRC_FILES})

# Find and link boost components.
find_package (Boost 1.55 COMPONENTS filesystem program_options iostreams REQUIRED)
include_directories (${Boost_INCLUDE_DIR})
target_link_libraries (${PROJECT_NAME} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

# C++ flags.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O3")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

# Minium C++11 standard required.
set_property (TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property (TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# Strip release binaries.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")


#-------------------
# Test
#-------------------

enable_testing ()
set (PROJECT_TEST_NAME ${PROJECT_NAME}_test)

file (GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable (${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src ${Boost_INCLUDE_DIR})
target_link_libraries (${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
add_test (test1 ${PROJECT_TEST_NAME})

# Minium C++11 standard required.
set_property (TARGET ${PROJECT_TEST_NAME} PROPERTY CXX_STANDARD 11)
set_property (TARGET ${PROJECT_TEST_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

#------------------
# Doxygen
#------------------
find_package (Doxygen)

if (DOXYGEN_FOUND)
  configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxygen.in
                  ${CMAKE_CURRENT_BINARY_DIR}/Doxygen @ONLY)
  add_custom_target (doc
                     ${DOXYGEN_EXECUTABLE}
                     ${CMAKE_CURRENT_BINARY_DIR}/Doxygen
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
                     COMMENT "Generating project documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)
