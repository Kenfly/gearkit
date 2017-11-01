project(network)
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
    set(SRC_DIR "platform/win")
elseif(APPLE)
	add_definitions(-DPLATFORM_MAC)
    set(SRC_DIR "platform/linux")
elseif(UNIX)
	add_definitions(-DPLATFORM_LINUX)
    set(SRC_DIR "platform/linux")
endif()

set(HEADER_FILE
    netbase.h
    network.h
	Socket.h
    SockAddr.h
)

Set(SOURCE_FILE
)

list(APPEND SOURCE_FILE ${SRC_DIR}/Socket.cpp)
list(APPEND SOURCE_FILE ${SRC_DIR}/SockAddr.cpp)

if(BUILD_TYPE STREQUAL "server")
    list(APPEND HEADER_FILE Server.h)
    list(APPEND SOURCE_FILE ${SRC_DIR}/Server.cpp)
elseif(BUILD_TYPE STREQUAL "client")
    list(APPEND HEADER_FILE Client.h)
    list(APPEND SOURCE_FILE ${SRC_DIR}/Client.cpp)
endif()

add_library(libnetwork ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(libnetwork PROPERTIES OUTPUT_NAME "network")
