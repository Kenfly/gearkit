project(libbase)

set(HEADER_FILE
    Singleton.h
    Logger.h
)

Set(SOURCE_FILE
    Logger.cpp
)

#list(APPEND SOURCE_FILE ${SRC_DIR}/Socket.cpp)

add_library(libbase ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(libbase PROPERTIES OUTPUT_NAME "base")
