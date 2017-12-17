project(network)

set(TARGET_NAME libnetwork)

set(suffix ${PLATFORM_NAME})

#add_gearkit_include(${GEARKIT_DIR}/)

set(HEADER_FILE
    netsys.h
    SockAddr.h
    Packet.h
    Session.h
	Socket.h
    Terminal.h
    network.h
    platform/${suffix}/SockAddr_${suffix}.h
    platform/${suffix}/Socket_${suffix}.h
)

Set(SOURCE_FILE
    SockAddr.cpp
    Socket.cpp
    Packet.cpp
    Session.cpp
    Terminal.cpp
    platform/${suffix}/SockAddr_${suffix}.cpp
    platform/${suffix}/Socket_${suffix}.cpp
)


if(USE_SERVER)
    list(APPEND HEADER_FILE Server.h)
    list(APPEND SOURCE_FILE Server.cpp)
    list(APPEND HEADER_FILE platform/${suffix}/Server_${suffix}.h)
    list(APPEND SOURCE_FILE platform/${suffix}/Server_${suffix}.cpp)
endif(USE_SERVER)

if(USE_CLIENT)
    list(APPEND HEADER_FILE Client.h)
    list(APPEND SOURCE_FILE Client.cpp)
    list(APPEND HEADER_FILE platform/${suffix}/Client_${suffix}.h)
    list(APPEND SOURCE_FILE platform/${suffix}/Client_${suffix}.cpp)
endif(USE_CLIENT)

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "network")
target_link_libraries(${TARGET_NAME} libkit)

