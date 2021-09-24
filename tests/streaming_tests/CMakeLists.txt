#*************************************************************************
#
#                           Klepsydra Streaming Modules
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

SET(PROJ_NAME kpsr_${PROJ_MAIN_NAME}_tests)
PROJECT(${PROJ_NAME})

# Source
#---------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
)

# Create Library
#---------------------------------------------------#
ADD_EXECUTABLE(${PROJ_NAME} ${${PROJ_NAME}_SRC} )

# Link libraries with Project
#---------------------------------------------------#

TARGET_LINK_LIBRARIES(${PROJ_NAME}
  kpsr_${PROJ_MAIN_NAME}
  gtest_main
  )

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../common
  )

add_test(NAME ${PROJ_NAME} COMMAND ${PROJ_NAME})

