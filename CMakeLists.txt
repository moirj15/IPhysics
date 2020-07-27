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
        ${SRC_PHYSICS_ENGINE_FILES}
        ${SRC_PHYSICS_SIMULATION_FILES}
        ${CMAKE_SOURCE_DIR}/glad/src/glad.c
        )

set(SOURCE_LIST_VOXEL_GENERATOR
        ${IMGUI_FILES}
        ${SRC_VOXEL_GENERATOR}
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

    set(CMAKE_CXX_FLAGS "/std:c++17 /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
    set(CMAKE_CXX_FLAGS_DEBUG "/std:c++17 /MTd /MP /TP /W3 /DEBUG -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
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
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE -DIMGUI_IMPL_OPENGL_LOADER_GLAD")
    include_directories(
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/sharedLibrary/include
    )
    link_directories(
            ${CMAKE_SOURCE_DIR}/libs/linux
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
            pthread
    )
    target_link_libraries(
            VoxelGenerator
            sharedLibrary
            ${OPENGL_LIBRARY}
            libglfw3.a
            dl
            X11
            pthread
    )
endif ()







