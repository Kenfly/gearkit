project(libdebug)

set(TARGET_NAME libdebug)

set(HEADER_FILE
    debugfunc.h
    DebugInput.h
)

Set(SOURCE_FILE
    DebugInput.cpp
)

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "debug")

