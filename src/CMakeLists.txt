# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

set(BIN_NAME ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE INCLUDE_FILES
    "../include/DAudioManager"
    "../include/dtkaudiomanager_global.h"
    "../include/daudioport.h"
    "../include/daudiocard.h"
    "../include/daudiomanager.h"
)
file(GLOB_RECURSE INCLUDE_PRIVATE_FILES
    "./daudio_p.h"
    "./dpulseaudio.h"
)
file(GLOB_RECURSE SRCS
    "./daudioport.cpp"
    "./daudiocard.cpp"
    "./daudiomanager.cpp"
    "./dpulseaudio.cpp"
)

add_library(${BIN_NAME} SHARED
    ${INCLUDE_FILES}
    ${INCLUDE_PRIVATE_FILES}
    ${SRCS}
)

include_directories(../include)

add_definitions(-DLIBDTKAUDIOMANAGER_LIBRARY)

set_target_properties(${BIN_NAME} PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

target_compile_definitions(${BIN_NAME} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")

find_package(PulseAudio)

target_include_directories(${BIN_NAME} PUBLIC
    ${PULSEAUDIO_INCLUDE_DIR}
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    ${PULSEAUDIO_LIBRARY}
)

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${BIN_NAME})
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
