cmake_minimum_required(VERSION 2.8)

project(testkit)
set(TARGET_NAME testkit)

include(../Config.cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/../ out)

set(GEARKIT_DIR ${PROJECT_SOURCE_DIR}/../)
add_gearkit_include(${GEARKIT_DIR})

set(APP_HED
    app_${BUILD_TYPE}.h
)
set(APP_SRC
    app_${BUILD_TYPE}.cpp
    main_${BUILD_TYPE}.cpp
)

add_executable(${TARGET_NAME} ${APP_HED} ${APP_SRC})
target_link_libraries(${TARGET_NAME} gearkit)

