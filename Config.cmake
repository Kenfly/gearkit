#notice:
#1.you need include this .cmakefile to build your project
#2.you need call add_gearkit_include add include dir


#use c++11
add_compile_options(-std=c++11)

#build type
set(BUILD_TYPE "server" CACHE STRING "Set Building Type")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS "server;client")

#script type
set(SCRIPT_TYPE "none" CACHE STRING "Choose Script Type")
set_property(CACHE SCRIPT_TYPE PROPERTY STRINGS "none;javascript;lua;python")

#compiler
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    set(COMPILER "gcc")
elseif(MSVC)
    set(COMPILER "msvc")
endif()

message("===================================")
message("cmake platform:${CMAKE_SYSTEM_NAME}")
message("building type:${BUILD_TYPE}")
message("script type:${SCRIPT_TYPE}")
message("compilor:${COMPILER}")
message("===================================")

#platforme define
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DPLATFORM_WINDOWS)
    set(WINDOWS TRUE)
    set(PLATFORM_NAME "windows")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DPLATFORM_LINUX)
    set(LINUX TRUE)
    set(PLATFORM_NAME "linux")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DPLATFORM_LINUX)
    set(APPLE TRUE)
    set(PLATFORM_NAME "linux")
endif()

set(INCLUDE_DIR
    ./
    base
    kit
    network
    script
)

function(add_gearkit_include prefix)
    foreach(var ${INCLUDE_DIR})
        include_directories(${prefix}${var})
        message("include:${prefix}${var}")
    endforeach()
endfunction()
    
