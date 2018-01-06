#notice:
#1.you need include this .cmakefile to build your project
#2.you need call add_gearkit_include add include dir


#use c++11
if(CMAKE_COMPILER_IS_GNUCXX)
    #add_compile_options(-std=c++11)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

# options
option(DEBUG_MODE "Debug or Release?" ON)
option(USE_SERVER "Use server module" ON)
option(USE_CLIENT "Use client module" ON)

#script type
set(SCRIPT_TYPE "lua" CACHE STRING "Choose Script Type")
set_property(CACHE SCRIPT_TYPE PROPERTY STRINGS "none;javascript;lua;python")

if(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE DEBUG)
    add_definitions(-DKIT_DEBUG_MODE)
else(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE RELEASE)
    add_definitions(-DKIT_RELEASE)
endif(DEBUG_MODE)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#compiler
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    set(COMPILER "gcc")
elseif(MSVC)
    set(COMPILER "msvc")
endif()

message("===================================")
message("cmake platform:${CMAKE_SYSTEM_NAME}")
message("compilor:${COMPILER}")
message("debug:${DEBUG_MODE}")
message("script:${SCRIPT_TYPE}")
message("server:${USE_SERVER}")
message("client:${USE_CLIENT}")
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
    debug
)

function(add_gearkit_include prefix)
    foreach(var ${INCLUDE_DIR})
        include_directories(${prefix}${var})
        message("include:${prefix}${var}")
    endforeach()
endfunction()
    
