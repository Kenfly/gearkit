#notice:
#1.you need include this .cmakefile to build your project
#2.you need call add_gearkit_include add include dir


#use c++11
add_compile_options(-std=c++11)

set(BUILD_TYPE "server" CACHE STRING "Set Building Type")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS "server;client")

#compiler
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    set(COMPILER "gcc")
elseif(MSVC)
    set(COMPILER "msvc")
endif()

message("===================================")
message("cmake platform:${CMAKE_SYSTEM_NAME}")
message("building type:${BUILD_TYPE}")
message("compilor:${COMPILER}")
message("===================================")

#platforme define
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DPLATFORM_WINDOWS)
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DPLATFORM_LINUX)
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DPLATFORM_LINUX)
    set(APPLE TRUE)
endif()

set(INCLUDE_DIR
    ./
    base
    network
)

function(add_gearkit_include prefix)
    foreach(var ${INCLUDE_DIR})
        include_directories(${prefix}${var})
        message("include:${prefix}${var}")
    endforeach()
endfunction()
    
