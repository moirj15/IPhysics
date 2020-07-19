cmake_minimum_required(VERSION 3.10)
project(IPhysics)

file(GLOB IMGUI_FILES CONFIGURE_DEPENDS "imgui/*")
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*")


# the source files for the voxel generator app
file(GLOB SRC_VOXEL_GENERATOR CONFIGURE_DEPENDS "src/voxelizer/voxelGenerator/*")

# the source files for the physics simulation app
file(GLOB SRC_PHYSICS_SIMULATION_FILES CONFIGURE_DEPENDS "src/physics-sim/app/*")
file(GLOB SRC_PHYSICS_ENGINE_FILES CONFIGURE_DEPENDS "src/physics-sim/PhysicsEngine/*")


set(SOURCE_LIST_MAIN_PROJECT
        ${IMGUI_FILES}
        #${SRC_UTILS_FILES}
        #${SRC_RENDERER_FILES}
        ${SRC_PHYSICS_ENGINE_FILES}
        #${SRC_INPUT_FILES}
        ${SRC_PHYSICS_SIMULATION_FILES}
        ${CMAKE_SOURCE_DIR}/glad/src/glad.c
        #${SRC_VOXEL_OBJECTS}
        )

set(SOURCE_LIST_VOXEL_GENERATOR
        ${IMGUI_FILES}
        #${SRC_UTILS_FILES}
        ${SRC_VOXEL_GENERATOR}
        #${SRC_RENDERER_FILES}
        #${SRC_INPUT_FILES}
        #${SRC_VOXEL_OBJECTS}
        #${SRC_PHYSICS_ENGINE_FILES}
        )


find_package(OpenGL REQUIRED)
add_subdirectory(sharedLibrary)
if (WIN32)
    message("WINDOWS")
    include_directories(
            ${CMAKE_SOURCE_DIR}/include;
            ${CMAKE_SOURCE_DIR}/sharedLibrary/include;
            ${CMAKE_SOURCE_DIR}/glad/include
    )
    link_directories(
            ${CMAKE_SOURCE_DIR}/libs/windows
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet
            ${CMAKE_BINARY_DIR}/sharedLibrary
            ${CMAKE_BINARY_DIR}/sharedLibrary
    )
    add_executable(
            IPhysics
            ${SOURCE_LIST_MAIN_PROJECT}
    )

    add_executable(
            VoxelGenerator
            ${SOURCE_LIST_VOXEL_GENERATOR}
    )

    set(CMAKE_CXX_FLAGS "/std:c++17 /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLEW_STATIC -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
    set(CMAKE_CXX_FLAGS_DEBUG "/std:c++17 /MTd /MP /TP /W3 /DEBUG -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DGLEW_STATIC -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    message(${CMAKE_BUILD_TYPE})
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message("DEBUG YOU FOOL!!!")
        target_link_libraries(
                IPhysics
                ${OPENGL_LIBRARY}
                BulletDynamics_Debug
                BulletCollision_Debug
                LinearMath_Debug
                sharedLibrary
                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
        )
        target_link_libraries(
                VoxelGenerator
                ${OPENGL_LIBRARY}
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
                BulletCollision_Debug
                LinearMath_Debug
                sharedLibrary
                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
        )
    else ()
        target_link_libraries(
                IPhysics
                ${OPENGL_LIBRARY}
                #                D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
                #      D:/libraries/glfw/release/glfw3.lib
                #                G:/libraries/glfw-clang/glfw3.lib
                #                D:/libraries/bullet/Release/BulletDynamics.lib
                #                D:/libraries/bullet/Release/BulletCollision.lib
                #                D:/libraries/bullet/Release/LinearMath.lib
                BulletDynamics.lib
                BulletCollision.lib
                LinearMath.lib

                glfw3.lib
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
        )
    endif ()
else ()
    message("not windows hit me")
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
    include_directories(
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/sharedLibrary/include
    )
    link_directories(
            ${CMAKE_SOURCE_DIR}/libs
            ${CMAKE_BINARY_DIR}/sharedLibrary
    )
    add_executable(
            IPhysics
            ${SOURCE_LIST_MAIN_PROJECT}
    )

    add_executable(
            VoxelGenerator
            ${SOURCE_LIST_VOXEL_GENERATOR}
    )
    target_link_libraries(
            IPhysics
            sharedLibrary
            ${OPENGL_LIBRARY}
            libglfw3.a
            dl
            X11
    )
    target_link_libraries(
            VoxelGenerator
            sharedLibrary
            ${OPENGL_LIBRARY}
            libglfw3.a
            dl
            X11
    )
endif ()







