project(luacore)

set(TARGET_NAME libluacore)

set(EXTLIB)
set(LUA_SRC ${PROJECT_SOURCE_DIR}/lua-5.3.4/src)
set(LUA_INTF_SRC ${PROJECT_SOURCE_DIR}/lua-intf/)

set(HEADER_FILE
    LuaCore.h
    LuaBuffer.h
    LuaProtocol.h
    LuaBinding.h
)

Set(SOURCE_FILE
    LuaCore.cpp
    LuaBuffer.cpp
    LuaProtocol.cpp
    LuaBinding.cpp
)

if(DEBUG_MODE)
    list(APPEND HEADER_FILE 
        LuaDebug.h
    )
    list(APPEND SOURCE_FILE
        LuaDebug.cpp
    )
    list(APPEND EXTLIB libdebug)
endif(DEBUG_MODE)

add_subdirectory(${LUA_SRC})

#aux_source_directory(${LUA_INTF_SRC}/LuaIntf/src LUAINF)

#add_definitions(-DLUAINTF_HEADERS_ONLY)

include_directories(${LUA_SRC})
include_directories(${LUA_INTF_SRC})


add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "luacore")
target_link_libraries(${TARGET_NAME} libkit libnetwork liblua ${EXTLIB})

