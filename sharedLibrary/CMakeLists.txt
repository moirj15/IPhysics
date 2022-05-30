cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(sharedLibrary)

# the source files for the shared libraries
set(SRC_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SHARED_LIBRARY_SOURCE
        ${SRC_FILE_DIR}/Utils/Serialization.cpp
        ${SRC_FILE_DIR}/Renderer/Camera.cpp
        ${SRC_FILE_DIR}/Renderer/DebugDrawer.cpp
        ${SRC_FILE_DIR}/Renderer/Renderer.cpp
        ${SRC_FILE_DIR}/Renderer/Shader.cpp
        ${SRC_FILE_DIR}/Renderer/ShaderLibrary.cpp
        ${SRC_FILE_DIR}/Renderer/Window.cpp
        ${SRC_FILE_DIR}/VoxelObjects/MeshManager.cpp
        ${SRC_FILE_DIR}/Renderer/Gfx.cpp
        ${SRC_FILE_DIR}/Renderer/glad.c
         )

# set the include files
set(HEADER_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHARED_LIBRARY_HEADERS
        ${HEADER_FILE_DIR}/Utils/Serialization.h
        ${HEADER_FILE_DIR}/Utils/MathCasts.h
        ${HEADER_FILE_DIR}/Utils/VoxelSerializer.h
        ${HEADER_FILE_DIR}/Renderer/Camera.h
        ${HEADER_FILE_DIR}/Renderer/DebugDrawer.h
        ${HEADER_FILE_DIR}/Renderer/Renderer.h
        ${HEADER_FILE_DIR}/Renderer/Shader.h
        ${HEADER_FILE_DIR}/Renderer/ShaderLibrary.h
        ${HEADER_FILE_DIR}/Renderer/Window.h
        ${HEADER_FILE_DIR}/VoxelObjects/MeshManager.h
        ${HEADER_FILE_DIR}/VoxelObjects/Objects.h
         ${HEADER_FILE_DIR}/Renderer/Gfx.h
         ${HEADER_FILE_DIR}/Renderer/glad.h
         )


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/include 
    #${CMAKE_SOURCE_DIR}/glad/include
    #${CMAKE_SOURCE_DIR}/focus/pub_include
    )

add_library(sharedLibrary STATIC ${SHARED_LIBRARY_HEADERS} ${SHARED_LIBRARY_SOURCE})

target_include_directories(sharedLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
if (WIN32)
    set_property(TARGET sharedLibrary PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:DEBUG>:Debug>")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /TP /W3 /DEBUG -D_CRT_SECURE_NO_WARNINGS  -DGLM_ENABLE_EXPERIMENTAL")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /TP /W3 /DEBUG -D_DEBUG -D_CRT_SECURE_NO_WARNINGS  -DGLM_ENABLE_EXPERIMENTAL")
    link_directories(${CMAKE_SOURCE_DIR}/libs/windows)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_libraries(
                sharedLibrary
                #${CMAKE_SOURCE_DIR}/focus/lib/focus.lib
				focus
                ${CMAKE_SOURCE_DIR}/libs/windows/SDL2-static.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/SDL2main.lib
                #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletSoftBody_Debug.lib
                #focus
                #                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
                #msvcrtd.lib
                winmm.lib
                imm32.lib
                version.lib
                d3d11.lib
                d3d12.lib
                libcmt.lib
                dxgi.lib
                dxguid.lib
                d3dcompiler.lib
                winmm.lib
                gdi32
                opengl32
                Setupapi.lib
        )
    else()
        target_link_libraries(
                sharedLibrary
                ${CMAKE_SOURCE_DIR}/focus/lib/focus.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/SDL2-static.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/SDL2main.lib
                #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletSoftBody.lib
                #focus
                #                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
                #msvcrtd.lib
                winmm.lib
                imm32.lib
                version.lib
                d3d11.lib
                d3d12.lib
                libcmt.lib
                dxgi.lib
                dxguid.lib
                d3dcompiler.lib
                winmm.lib
                gdi32
                opengl32
                Setupapi.lib
        )
    endif()

else ()
    set(CMAKE_CXX_FLAGS "-std=c++20 -g -Wall -Wextra -pedantic -D_DEBUG")
    link_directories(${CMAKE_SOURCE_DIR}/libs/linux)
endif ()
