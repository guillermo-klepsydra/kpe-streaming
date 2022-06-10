#*************************************************************************
#
#                           Klepsydra STREAMING Modules
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
#*************************************************************************
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${GTEST_PATH}
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories("${GTEST_PATH}/googletest/include")

set(TEST_DATA ${CMAKE_CURRENT_SOURCE_DIR}/data )

# Configure modules header with variables configured, last position to work!
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/common/config.h.in"
                "${CMAKE_CURRENT_SOURCE_DIR}/common/config.h" )

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/data/streaming_conf.json.in"
                "${CMAKE_CURRENT_SOURCE_DIR}/data/streaming_conf.json" )

add_subdirectory(streaming_tests)
