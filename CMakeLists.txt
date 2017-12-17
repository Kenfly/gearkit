cmake_minimum_required(VERSION 2.8)

project(gearkit)
set(TARGET_NAME gearkit)

set(EXTLIB)
set(GEARKIT_DIR ${PROJECT_SOURCE_DIR})

add_gearkit_include(${GEARKIT_DIR}/)

add_subdirectory(base)
add_subdirectory(kit)
add_subdirectory(network)
if (NOT (${SCRIPT_TYPE} MATCHES "none"))
    add_subdirectory(script)
    list(APPEND EXTLIB libscript)
endif()

set(HEADER_FILE
    Application.h
)

Set(SOURCE_FILE
    Application.cpp
)

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${TARGET_NAME} libkit libnetwork ${EXTLIB})

