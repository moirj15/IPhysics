cmake_minimum_required(VERSION 3.10)
project(sharedLibrary)
if (WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++17 /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS  -DGLM_ENABLE_EXPERIMENTAL")
    set(CMAKE_CXX_FLAGS_DEBUG "/std:c++17 /MTd /MP /TP /W3 /DEBUG -D_DEBUG -D_CRT_SECURE_NO_WARNINGS  -DGLM_ENABLE_EXPERIMENTAL")
else ()
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
endif ()

# the source files for the shared libraries
file(GLOB SRC_UTILS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/*")
file(GLOB SRC_RENDERER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*")
file(GLOB SRC_VOXEL_OBJECTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/VoxelObjects/*")
file(GLOB SRC_INPUT_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/Input/*")

# set the include files
file(GLOB INCLUDE_UTILS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/Utils/*")
file(GLOB INCLUDE_RENDERER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/Renderer/*")
file(GLOB INCLUDE_VOXEL_OBJECTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/VoxelObjects/*")
file(GLOB INCLUDE_INPUT_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/Input/*")

set(SHARED_LIBRARY_SOURCES
        ${SRC_UTILS_FILES}
        ${SRC_RENDERER_FILES}
        ${SRC_VOXEL_OBJECTS}
        ${SRC_INPUT_FILES}
        ${CMAKE_SOURCE_DIR}/glad/src/glad.c
        )

set(SHARED_LIBRARY_INCLUDE
        ${INCLUDE_UTILS_FILES}
        ${INCLUDE_RENDERER_FILES}
        ${INCLUDE_VOXEL_OBJECTS}
        ${INCLUDE_INPUT_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Common.h
        )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/glad/include)

add_library(sharedLibrary STATIC ${SHARED_LIBRARY_INCLUDE} ${SHARED_LIBRARY_SOURCES})

target_include_directories(sharedLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/libs/windows)
else ()
    link_directories(${CMAKE_SOURCE_DIR}/libs/linux)
endif ()
