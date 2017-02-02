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
add_executable (tds2pdb ${PROJECT_SRC_FILES})

# Find and link boost components.
find_package (Boost 1.55 COMPONENTS filesystem REQUIRED)
include_directories (${BOOST_INCLUDE_DIR})
target_link_libraries (tds2pdb ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# C++ flags.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

# Minium C++11 standard required.
set_property (TARGET tds2pdb PROPERTY CXX_STANDARD 11)
set_property (TARGET tds2pdb PROPERTY CXX_STANDARD_REQUIRED ON)


#-------------------
# Test
#-------------------

enable_testing ()
set (PROJECT_TEST_NAME ${PROJECT_NAME}_test)

file (GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable (${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS})

target_link_libraries (${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
add_test (test1 ${PROJECT_TEST_NAME})