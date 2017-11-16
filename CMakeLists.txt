cmake_minimum_required(VERSION 2.8)

project(gearkit)
set(TARGET_NAME gearkit)

set(GEARKIT_DIR ${PROJECT_SOURCE_DIR})

set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/base
    ${PROJECT_SOURCE_DIR}/network
)

include_directories(${INCLUDE_DIR})

add_subdirectory(base)
add_subdirectory(network)

set(HEADER_FILE
    Application.h
)

Set(SOURCE_FILE
    Application.cpp
)

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${TARGET_NAME} libbase libnetwork)

