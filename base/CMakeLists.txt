project(libbase)

set(HEADER_FILE
    kitsys.h
    Singleton.h
    Ref.h
    RefPool.h
    Logger.h
    HashList.h
    Buffer.h
    BufferPool.h
)

Set(SOURCE_FILE
    kitsys.cpp
    Ref.cpp
    RefPool.cpp
    Logger.cpp
    HashList.cpp
    Buffer.cpp
    BufferPool.cpp
)

add_library(libbase ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(libbase PROPERTIES OUTPUT_NAME "base")
