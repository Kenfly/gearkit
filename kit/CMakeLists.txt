project(libkit)

set(TARGET_NAME libkit)
set(EXTLIB)
set(suffix ${PLATFORM_NAME})

if(LINUX OR APPLE)
    list(APPEND EXTLIB pthread)
endif()

set(HEADER_FILE
    kitsys.h
    Ref.h
    RefPool.h
    Buffer.h
    BufferPool.h
    Logger.h
    Timer.h
    Mutex.h
    Thread.h
    Functions.h
    State.h
    SyncQueue.h
    platform/${suffix}/Mutex_${suffix}.h
    platform/${suffix}/Thread_${suffix}.h
)

Set(SOURCE_FILE
    kitsys.cpp
    Ref.cpp
    RefPool.cpp
    Buffer.cpp
    BufferPool.cpp
    Logger.cpp
    Timer.cpp
    Mutex.cpp
    Thread.cpp
    Functions.cpp
    State.cpp
    platform/${suffix}/Mutex_${suffix}.cpp
    platform/${suffix}/Thread_${suffix}.cpp
)

add_library(${TARGET_NAME} ${HEADER_FILE} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "kit")
target_link_libraries(${TARGET_NAME} ${EXTLIB} )
