project(network)

set(TARGET_NAME libnetwork)
set(EXTLIB)

if(WINDOWS)
    set(suffix "win")
elseif(APPLE)
    set(suffix "linux")
    list(APPEND EXTLIB pthread)
elseif(LINUX)
    set(suffix "linux")
    list(APPEND EXTLIB pthread)
endif()

include_directories(${INCLUDE_DIR})

set(HEADER_FILE
    network.h
    SockAddr.h
    Packet.h
	Socket.h
    platform/${suffix}/SockAddr_${suffix}.h
    platform/${suffix}/Socket_${suffix}.h
)

Set(SOURCE_FILE
    SockAddr.cpp
    Packet.cpp
    Socket.cpp
    platform/${suffix}/SockAddr_${suffix}.cpp
    platform/${suffix}/Socket_${suffix}.cpp
)


if(BUILD_TYPE STREQUAL "server")
    list(APPEND HEADER_FILE Server.h)
    list(APPEND SOURCE_FILE Server.cpp)
    list(APPEND HEADER_FILE platform/${suffix}/Server_${suffix}.h)
    list(APPEND SOURCE_FILE platform/${suffix}/Server_${suffix}.cpp)
elseif(BUILD_TYPE STREQUAL "client")
    list(APPEND HEADER_FILE Client.h)
    list(APPEND SOURCE_FILE Client.cpp)
    list(APPEND HEADER_FILE platform/${suffix}/Client_${suffix}.h)
    list(APPEND SOURCE_FILE platform/${suffix}/Client_${suffix}.cpp)
endif()

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "network")
target_link_libraries(${TARGET_NAME} ${EXTLIB} libbase)
