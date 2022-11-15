# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

PROJECT(dtkaudiomanager-example)
file(GLOB_RECURSE SRCS "main.cpp"
    "../src/*.cpp"
)

set(EXECUTABLE_OUTPUT_PATH ../)
include_directories("../include")
add_executable(dtkaudiomanager-example
    ${SRCS}
)

find_package(PulseAudio)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${BIN_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    ${PULSEAUDIO_LIBRARY}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PULSEAUDIO_INCLUDE_DIR}
)
