project(luacore)

set(TARGET_NAME libluacore)

set(HEADER_FILE
    LuaCore.h
    LuaBinding.h
)

Set(SOURCE_FILE
    LuaCore.cpp
    LuaBinding.cpp
)

set(LUA_SRC ${PROJECT_SOURCE_DIR}/lua-5.3.4/src)
set(LUA_BRIDGE_SRC ${PROJECT_SOURCE_DIR}/LuaBridge/Source)
add_subdirectory(${LUA_SRC})
include_directories(${LUA_SRC})
include_directories(${LUA_BRIDGE_SRC})


add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "luacore")
target_link_libraries(${TARGET_NAME} libkit libnetwork liblua)

