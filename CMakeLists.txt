CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(Learning)

set (CMAKE_BUILD_TYPE Debug) 
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} modules/)

if(NOT MSVC)
    set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
endif(NOT MSVC)

if(NOT MSVC)
    option(Warn "Compile using all warnings")
endif(NOT MSVC)

if(Warn)
    set (CMAKE_CXX_FLAGS "-Wall -Werror -Wno-padded ${CMAKE_CXX_FLAGS}")
endif(Warn)

set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)
file(COPY res shaders DESTINATION ${CMAKE_BINARY_DIR})
