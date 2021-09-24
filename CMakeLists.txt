#****************************************************************************
#
#                           Klepsydra Core Modules
#              Copyright (C) 2020-2022  Klepsydra Technologies GmbH
#                            All Rights Reserved.
#
#  This file is subject to the terms and conditions defined in
#  file 'LICENSE.md', which is part of this source code package.
#
#  NOTICE:  All information contained herein is, and remains the property of Klepsydra
#  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
#  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
#  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
#  trade secret or copyright law. Dissemination of this information or reproduction of
#  this material is strictly forbidden unless prior written permission is obtained from
#  Klepsydra Technologies GmbH.
#
#****************************************************************************

# Main header: Project name & Configuration
# ------------------------------------------------------------------------------------------------------
CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

if(DEFINED KPSR_BUILD_PATH)
else()
    SET(KPSR_BUILD_PATH ${CMAKE_SOURCE_DIR}/kpsr-build)
endif()
message(STATUS "kpsr-build path: " ${KPSR_BUILD_PATH})

if (KPSR_WITH_FREERTOS)
   include(${KPSR_BUILD_PATH}/freeRTOS/cmake/compiler.cmake)
endif()

SET(PROJ_MAIN_NAME "streaming")

PROJECT(${PROJ_MAIN_NAME})

set(KPSR_STREAMING_MAJOR_VERSION 1)
set(KPSR_STREAMING_MINOR_VERSION 0)
set(KPSR_STREAMING_PATCH_VERSION 0)
set(KPSR_STREAMING_VERSION
    ${KPSR_STREAMING_MAJOR_VERSION}.${KPSR_STREAMING_MINOR_VERSION}.${KPSR_STREAMING_PATCH_VERSION})

# Configuration types
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

IF(DEFINED KPSR_PRODUCTION)
  add_definitions(-DKPSR_DLL_EXPORTS)
  add_definitions(-DKPSR_DLL)
  cmake_policy(SET CMP0063 NEW)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
ENDIF()

IF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
  SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
ENDIF()

#Install binaries
IF(DEFINED KPSR_INSTALL_PATH)
  SET(CMAKE_INSTALL_PREFIX ${KPSR_INSTALL_PATH})
ELSE()
  SET(KPSR_INSTALL_PATH ${CMAKE_INSTALL_PREFIX})
ENDIF()
message(STATUS "Klepsydra install path: " ${KPSR_INSTALL_PATH})

IF(DEFINED THIRDPARTIES_PATH)
ELSE()
  SET(THIRDPARTIES_PATH ${CMAKE_SOURCE_DIR}/thirdparties )
ENDIF()
message(STATUS "Thirdparties install path: " ${THIRDPARTIES_PATH})

IF(DEFINED GTEST_PATH)
ELSE()
  SET(GTEST_PATH ${THIRDPARTIES_PATH}/googletest) #default google test path
ENDIF()
message(STATUS "Google test path: " ${GTEST_PATH})

# Configure variables
# ------------------------------------------------------------------------------------------------------

# Paths
SET(${PROJ_MAIN_NAME}_PATH_MAIN  ${CMAKE_SOURCE_DIR}                         CACHE PATH "This directory contains initial Path")
SET(${PROJ_MAIN_NAME}_PATH_INSTALL       ${KPSR_INSTALL_PATH})
SET(CMAKE_MODULE_PATH            ${KPSR_BUILD_PATH}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET(LIBRARY_OUTPUT_PATH          ${PROJECT_BINARY_DIR}/bin                   CACHE PATH "Library output" FORCE)
SET(EXECUTABLE_OUTPUT_PATH       ${PROJECT_BINARY_DIR}/bin                   CACHE PATH "Executable output" FORCE)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)

IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "leon3")
  enable_language(ASM)
  # -Dsun is a hack in order to make cereal believe that we are on the Solaris
  # platform, and disable the BSD extensions
  SET(BUILD_SHARED_LIBS "OFF" FORCE)
  SET(ARCH_FLAGS "-mcpu=leon3 -qbsp=leon3 -Dsun -D_GNU_SOURCE")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAGS}")
  SET(CMAKE_EXE_LINKER_FLAGS "--entry=__wrap_start")
ELSEIF(KPSR_WITH_FREERTOS)
  SET(BUILD_SHARED_LIBS "OFF" CACHE PATH "Static libraries" FORCE)
  include(${KPSR_BUILD_PATH}/freeRTOS/cmake/zynq.cmake)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -faligned-new")

ELSE()
  find_package(Threads REQUIRED)
  SET(BUILD_SHARED_LIBS "ON" FORCE)
ENDIF()

set (CMAKE_CXX_STANDARD 14)

if (NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
    check_cxx_compiler_flag(-Wl,--no-undefined HAS_NO_UNDEFINED)
    if (HAS_NO_UNDEFINED)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    endif()
endif()

check_cxx_compiler_flag(-pedantic HAS_PEDANTIC)
if (HAS_PEDANTIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

check_cxx_compiler_flag(-Wall HAS_WALL)
if (HAS_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_include_file_cxx(sys/epoll.h HAS_EPOLL)
if (HAS_EPOLL)
    add_definitions(-DHAS_EPOLL)
else()
    add_definitions(-DUSE_GET_WRITE_QUEUE)
endif()

IF((NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "leon3") AND (NOT KPSR_WITH_FREERTOS))
  enable_testing()
ENDIF()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.5)
    else()
        add_definitions(-faligned-new)
    endif()
endif()

add_definitions(-fno-strict-overflow)

if (KPSR_WITH_CODE_METRICS)
   # Code Coverage
   IF (CMAKE_BUILD_TYPE MATCHES Debug)
       add_definitions(--coverage)
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
       SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
       include(FindPythonInterp)
       include(CodeCoverage)
       setup_target_for_all_tests_coverage()
       setup_target_for_all_tests_coverage_cobertura(coverage)
   ENDIF()

   # Code style check
   include(CppcheckTargets)
   include(Findcppcheck)
ENDIF()

if (DEFINED KPSR_WITH_MARCH)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${KPSR_WITH_MARCH}")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(KlepsydraBuild)

# Configuration current project
# ------------------------------------------------------------------------------------------------------

# Project Logic
# ------------------------------------------------------------------------------------------------------

find_package(Klepsydra REQUIRED)
find_package(KlepsydraAdmin REQUIRED)
######################################################################################
# ADD SUB FOLDERS
######################################################################################
LINK_DIRECTORIES(${CMAKE_INSTALL_FULL_LIBDIR})

ADD_SUBDIRECTORY(modules)
IF((NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "leon3") AND (NOT KPSR_WITH_FREERTOS))
  ADD_SUBDIRECTORY(tests)
ENDIF()

if (KPSR_RUNTIME)
else()
   # add a target to generate API documentation with Doxygen
   if(KPSR_WITH_DOXYGEN)
       find_package(Doxygen REQUIRED)
       set(DOC_PROJECT_NAME "Klepsydra Streaming")
       set(DOC_PROJECT_BRIEF "Klepsydra Streaming API")

       file(GLOB_RECURSE DOC_DIRS "*/modules/group_definitions.h")
       string(REGEX REPLACE "/group_definitions.h[;]?" " " DOC_DIRS "${DOC_DIRS}")
       string(REGEX REPLACE ";" " " DOC_DIRS "${DOC_DIRS}")

       configure_file(${KPSR_BUILD_PATH}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
       add_custom_target(doc
           ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
           COMMAND moxygen --templates ${KPSR_BUILD_PATH}/docs/moxygen/templates --groups --output api-%s.md ./xml
           WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
           COMMENT "Generating API documentation with Doxygen" VERBATIM
       )
   endif(KPSR_WITH_DOXYGEN)
ENDIF()


set(DEF_INSTALL_CMAKE_DIR lib/cmake/klepsydrastreaming)
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

list (APPEND EXPORT_INCLUDE_DIRS ${CMAKE_INSTALL_FULL_INCLUDEDIR})
message ("CORE_EXPORT_TARGETS: " ${CORE_EXPORT_TARGETS})
message ("EXPORT_INCLUDE_DIRS: " ${EXPORT_INCLUDE_DIRS})
export(TARGETS ${CORE_EXPORT_TARGETS} FILE "${PROJECT_BINARY_DIR}/KlepsydraStreamingTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE streaming)

# Create the KlepsydraStreamingConfig.cmake and KlepsydraStreamingConfigVersion files
configure_file(KlepsydraStreamingConfig.cmake.in
  "${PROJECT_BINARY_DIR}/KlepsydraStreamingConfig.cmake" @ONLY)

configure_file(KlepsydraStreamingConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/KlepsydraStreamingConfig.cmake" @ONLY)
# ... for both
configure_file(KlepsydraStreamingConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/KlepsydraStreamingConfigVersion.cmake" @ONLY)

# Install the KlepsydraStreamingConfig.cmake and KlepsydraStreamingConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/KlepsydraStreamingConfig.cmake"
  "${PROJECT_BINARY_DIR}/KlepsydraStreamingConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT KlepsydraStreamingTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

