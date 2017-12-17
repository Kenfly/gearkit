project(lua)

set(TARGET_NAME liblua)

set(EXTLIB)

if (LINUX)
    list(APPEND EXTLIB readline)
    list(APPEND EXTLIB dl)
    add_definitions(-DLUA_USE_LINUX)
elseif (APPLE)
    list(APPEND EXTLIB readline)
endif()

aux_source_directory(. LUA_SRC)

add_library(${TARGET_NAME} ${LUA_SRC})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "lua")
target_link_libraries(${TARGET_NAME} ${EXTLIB})

