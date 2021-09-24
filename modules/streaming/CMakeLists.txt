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
SET(PROJ_NAME kpsr_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

ADD_DEFINITIONS(-DKPSR_LICENSE_FILE="")

# Source
#---------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
    )

FILE(GLOB_RECURSE ${PROJ_NAME}_HEADERS
    "include/*.h"
    )

# Create Library
#---------------------------------------------------#
ADD_LIBRARY(${PROJ_NAME} ${${PROJ_MAIN_NAME}_LIB_TYPE} ${${PROJ_NAME}_HEADERS} ${${PROJ_NAME}_SRC} )

# Link libraries with Project
#---------------------------------------------------#
TARGET_LINK_LIBRARIES(${PROJ_NAME}
  PUBLIC
  ${KLEPSYDRA_CORE_LIBRARIES}
  ${KPSR_ADMIN_CORE_LIBRARIES}
  PRIVATE
  pthreadpool
  )

TARGET_INCLUDE_DIRECTORIES(${PROJ_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Install
INSTALL(TARGETS ${PROJ_NAME}
    EXPORT KlepsydraStreamingTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
