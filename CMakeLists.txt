# @brief Required for building Belledonne

cmake_minimum_required(VERSION 2.8)

project(belledonne)

# This is required for CMake to find our custom .cmake files
set (CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  ${CMAKE_MODULE_PATH}
)

# Use debug version of libclang
# set(LIBCLANG_INCLUDE_PATH
    # "D:/llvm/tools/clang/include")
# set(LIBCLANG_LIBRARY_PATH
    # "D:/llvm/build/Debug/lib")
find_package(libclang 3.8.0 REQUIRED)
# Lmdb is being manually integrated in the project, see below
# find_package(lmdb REQUIRED)

# Paths definitions
set(BELLEDONNE_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Default parameters

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Various detection, to be used as shortcuts below

# Compilers
set(COMPILER_IS_CLANG
    0)
set(COMPILER_IS_GCC
    0)
set(COMPILER_IS_MSVC
    0)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(COMPILER_IS_CLANG
      1)
  message("Compiler detected as Clang")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(COMPILER_IS_GCC
      1)
  message("Compiler detected as gcc")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(COMPILER_IS_MSVC
      1)
  message("Compiler detected as MSVC")
else()
  message(SEND_ERROR "Could not deduce compiler")
endif()

# Systems
set(SYSTEM_IS_WINDOWS
    0)
set(SYSTEM_IS_MACOSX
    0)
set(SYSTEM_IS_LINUX
    0)
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(SYSTEM_IS_WINDOWS
      1)
  message("System detected as Windows")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(SYSTEM_IS_MACOSX
      1)
  message("System detected as Mac OS X")
else()
  # ${CMAKE_SYSTEM_NAME} MATCHES "Linux" may be true on Mac!
  set(SYSTEM_IS_LINUX
      1)
  message("System detected as Linux")
endif()

# Build configuration
set(BUILD_IS_DEBUG
    0)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(BUILD_IS_DEBUG
      1)
  message("Build configuration set to Debug")
else()
  message("Build configuration set to Release")
endif()

# Functions

# @brief For the specified target, append compiler flags to the existing ones
#
# @param _TARGET_NAME           target name
# @param _FLAGS                 flags to set
function(add_compiler_flags _TARGET_NAME _FLAGS)
  set_property(TARGET ${_TARGET_NAME}
               APPEND_STRING PROPERTY COMPILE_FLAGS ${_FLAGS}
               )
endfunction(add_compiler_flags)

# @brief For the specified target, append linker flags to the existing ones
#
# @param _TARGET_NAME           target name
# @param _FLAGS                 flags to set
function(add_linker_flags _TARGET_NAME _FLAGS)
  set_property(TARGET ${_TARGET_NAME}
               APPEND PROPERTY LINK_FLAGS ${_FLAGS}
               )
endfunction(add_linker_flags)

# @brief For the whole project, append debug compiler flags
#
# @param _FLAGS                 flags to set
function(add_debug_flags _FLAGS)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_debug_flags)

# @brief For the whole project, append release compiler flags
#
# @param _FLAGS                 flags to set
function(add_release_flags _FLAGS)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${_FLAGS}"
      PARENT_SCOPE)
  # Same for the "symbol" configuration
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_release_flags)

# @brief For the whole project, append release linker flags
#
# @param _FLAGS                 flags to set
function(add_release_linker_flags _FLAGS)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${_FLAGS}"
      PARENT_SCOPE)
  # Same for the "symbol" configuration
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_release_linker_flags)

# @brief For the given list of targets, disable all warnings
#
# @param _TARGETS      List of targets
function(disable_warnings_list _TARGETS)
 if (COMPILER_IS_GCC OR COMPILER_IS_CLANG)
   set(FLAG
       "-w")
 else()
   set(FLAG
       "/W0")
 endif()
 SET_SOURCE_FILES_PROPERTIES(${_TARGETS}
                             PROPERTIES COMPILE_FLAGS ${FLAG}
                             )
endfunction(disable_warnings_list)

# @brief (MSVC only) Set one specific target to use MT runtime library,
# or MTd in debug configuration
#
# @param _TARGET_NAME           target name
function(set_target_mt _TARGET_NAME)
  if(COMPILER_IS_MSVC)
    # Cannot use "add_CONFIG_flags" function here due to variable scoping
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT"
        PARENT_SCOPE)
  endif(COMPILER_IS_MSVC)
endfunction(set_target_mt)

# @brief (MSVC only) Set a platform toolset for one specific target
#
# @param _TARGET_NAME           target name
# @param _PLATFORM_TOOLSET      string, usually v120, v140 etc.
function(set_target_platformtoolset _TARGET_NAME _PLATFORM_TOOLSET)
  if(COMPILER_IS_MSVC)
    set(CMAKE_GENERATOR_TOOLSET ${_PLATFORM_TOOLSET} CACHE STRING "Platform Toolset" FORCE)
  endif()
endfunction(set_target_platformtoolset)

option(BUILD_TEST "Use GTest framework." OFF)
message(STATUS "GTest framework: ${BUILD_TEST}")

if(COMPILER_IS_MSVC)
option(BUILD_VSIX "Use Visual Studio SDK." OFF)
message(STATUS "Visual Studio SDK: ${BUILD_VSIX}")
endif()

# Project-wide various options
if (COMPILER_IS_MSVC)
  # Multithreaded build
  add_definitions("/MP")
endif (COMPILER_IS_MSVC)

# Project-wide warning options
if(COMPILER_IS_GCC OR COMPILER_IS_CLANG)
  add_definitions(-pedantic)
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wpointer-arith)
  add_definitions(-Wcast-qual)
  add_definitions(-Weffc++)
  add_definitions(-ggdb)
elseif(COMPILER_IS_MSVC)
  add_definitions(-W4)
  # This disable useless msvc warnings about checked iterators
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(COMPILER_IS_GCC OR COMPILER_IS_CLANG)

# Release-only options
if(COMPILER_IS_GCC OR COMPILER_IS_CLANG)
  add_release_flags("-Ofast")
elseif(COMPILER_IS_MSVC)
  add_release_linker_flags("/LTCG")
  add_release_flags("/Ox")
  add_release_flags("/Ob2")
  add_release_flags("/Oi")
  add_release_flags("/Os")
  add_release_flags("/GL")
endif(COMPILER_IS_GCC OR COMPILER_IS_CLANG)

# C++11 related options:
# If using Clang or gcc we have to add the flag "std=c++11"
# Nothing special with msvc
if (COMPILER_IS_CLANG)
  add_definitions(-std=c++11)
elseif (COMPILER_IS_GCC)
  add_definitions(-std=c++11)
endif()


# Various includes


# Integrating lmdb in the whole project as it is a pain to build separately and debug on Windows
include(lmdb_CmakeLists.txt)
# Keeping those for other platforms maybe
# set(LMDB_INCLUDE_DIRS
    # $ENV{LMDB_ROOT}
# )
# find_library(LMDB_LIBRARIES
             # NAMES liblmdb
             # HINTS ${LMDB_INCLUDE_DIRS})

# Google Test framework
if (BUILD_TEST)
  set(BUILD_GMOCK
      OFF CACHE INTERNAL ""
  )
  set(BUILD_GTEST
      ON CACHE INTERNAL ""
  )
  # GTest path may be defined from an outside CMake file,
  # if this is used as an external library
  if(NOT EXISTS ${GTEST_DIR})
    set(GTEST_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/externals/gtest
    )
    set(GTEST_BUILD_DIR
      ${CMAKE_CURRENT_BINARY_DIR}/gtest
    )
    set(GTEST_INCLUDE_DIRS
      ${GTEST_DIR}/googletest/include
    )
  endif(NOT EXISTS ${GTEST_DIR})
  # MSVC-specific issues
  if (MSVC AND MSVC_VERSION EQUAL 1700)
    # Known GTest/MSVC 2011 issue: VS2012 doesn't support correctly the tuples yet
    add_definitions(-D _VARIADIC_MAX=10)
  endif(MSVC AND MSVC_VERSION EQUAL 1700)

  # Not displaying gtest warnings
  add_subdirectory(${GTEST_DIR} ${GTEST_BUILD_DIR})
  add_compiler_flags(gtest " -w")
endif (BUILD_TEST)

add_subdirectory(belledonne)
