project(script)

set(TARGET_NAME libscript)
set(EXTLIB)

set(HEADER_FILE
    ScriptSupport.h
)

Set(SOURCE_FILE
    ScriptSupport.cpp
)


if(SCRIPT_TYPE STREQUAL "javascript")
    add_subdirectory(js)
elseif(SCRIPT_TYPE STREQUAL "lua")
    add_subdirectory(lua)
    list(APPEND EXTLIB libluacore)
elseif(SCRIPT_TYPE STREQUAL "python")
endif()

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "script")
target_link_libraries(${TARGET_NAME} ${EXTLIB})

