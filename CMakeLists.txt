cmake_minimum_required(VERSION 3.10)
project(IPhysics)
#set(CMAKE_UNITY_BUILD ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB THIRD_PARTY_SRC CONFIGURE_DEPENDS
        "src/third_party/imgui/*"
        "src/third_party/imgui/backends/imgui_impl_opengl3.h"
        "src/third_party/imgui/backends/imgui_impl_opengl3.cpp"
        "src/third_party/imgui/backends/imgui_impl_sdl.h"
        "src/third_party/imgui/backends/imgui_impl_sdl.cpp"
        )


# the source files for the voxel generator app
set(VOXEL_GENERATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/voxelizer)
set(VOXEL_GENERATOR_SOURCE
        ${VOXEL_GENERATOR_SOURCE_DIR}/main.cpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/Obj.cpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/System.cpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/Voxelizer.cpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/VoxelizerUI.cpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/tiny_obj_loader.cpp
        )
set(VOXEL_GENERATOR_HEADERS
        ${VOXEL_GENERATOR_SOURCE_DIR}/FileUtils.hpp
        ${VOXEL_GENERATOR_SOURCE_DIR}/Obj.h
        ${VOXEL_GENERATOR_SOURCE_DIR}/System.h
        ${VOXEL_GENERATOR_SOURCE_DIR}/Voxelizer.h
        ${VOXEL_GENERATOR_SOURCE_DIR}/VoxelizerParameters.h
        )
#file(GLOB SRC_VOXEL_GENERATOR CONFIGURE_DEPENDS "src/voxelizer/*")

# the source files for the physics simulation app
#file(GLOB SRC_PHYSICS_SIMULATION_FILES CONFIGURE_DEPENDS "src/physics-sim/app/*")
#file(GLOB SRC_PHYSICS_ENGINE_FILES CONFIGURE_DEPENDS "src/physics-sim/PhysicsEngine/*")

set(PHYSICS_SIM_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/physics-sim)

set(PHYSICS_SIM_SOURCE
        ${PHYSICS_SIM_SOURCE_DIR}/main.cpp
        ${PHYSICS_SIM_SOURCE_DIR}/IphysicsUI.cpp
        ${PHYSICS_SIM_SOURCE_DIR}/Physics.cpp
        ${PHYSICS_SIM_SOURCE_DIR}/PhysicsSimulationApp.cpp
        ${PHYSICS_SIM_SOURCE_DIR}/DebugRenderer.cpp
        ${PHYSICS_SIM_SOURCE_DIR}/Scene.cpp
        )
set(PHYSICS_SIM_HEADERS
        ${PHYSICS_SIM_SOURCE_DIR}/IphysicsUI.h
        ${PHYSICS_SIM_SOURCE_DIR}/Physics.h
        ${PHYSICS_SIM_SOURCE_DIR}/PhysicsSimulationApp.h
        ${PHYSICS_SIM_SOURCE_DIR}/collidable.h
        ${PHYSICS_SIM_SOURCE_DIR}/settings.h
        ${PHYSICS_SIM_SOURCE_DIR}/DebugRenderer.h
        ${PHYSICS_SIM_SOURCE_DIR}/Scene.h
        )



set(SOURCE_LIST_PHYSICS_SIM
        ${THIRD_PARTY_SRC}
        ${PHYSICS_SIM_SOURCE}
        ${PHYSICS_SIM_HEADERS}
        )

set(SOURCE_LIST_VOXEL_GENERATOR
        ${THIRD_PARTY_SRC}
        ${VOXEL_GENERATOR_SOURCE}
        ${VOXEL_GENERATOR_HEADERS}
        )


find_package(OpenGL REQUIRED)
add_subdirectory(sharedLibrary)
include_directories(
        ${CMAKE_SOURCE_DIR}/include;
        ${CMAKE_SOURCE_DIR}/sharedLibrary/include;
        ${CMAKE_SOURCE_DIR}/glad/include;
        ${CMAKE_SOURCE_DIR}/focus/pub_include;
        ${CMAKE_SOURCE_DIR}/src/third_party/imgui;
        # TODO: this is just a temp fix for imgui requiring sdl headers to be included
        ${CMAKE_SOURCE_DIR}/focus/include/SDL2
)
if (WIN32)
    message("WINDOWS")
    link_directories(
            ${CMAKE_SOURCE_DIR}/libs/windows
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet
            ${CMAKE_BINARY_DIR}/sharedLibrary
            ${CMAKE_SOURCE_DIR}/focus/lib;
    )

    set(CMAKE_CXX_FLAGS "/std:c++latest /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:WinMainCRTStartup")
    set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            )
    foreach (CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()
    add_executable(
        PhysicsSim
        ${SOURCE_LIST_PHYSICS_SIM}
    )

    add_executable(
            VoxelGenerator
            ${SOURCE_LIST_VOXEL_GENERATOR}
    )
    message(${CMAKE_BUILD_TYPE})
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message("DEBUG YOU FOOL!!!")
        #target_link_libraries(
        #        IPhysics
        #        ${OPENGL_LIBRARY}
        #        BulletDynamics_Debug
        #        BulletCollision_Debug
        #        LinearMath_Debug
        #        sharedLibrary
        #        focus
        #        glfw3
        #        opengl32.lib
        #        gdi32.lib
        #        user32.lib
        #        kernel32.lib
        #)
        target_link_libraries(
                VoxelGenerator
                ${OPENGL_LIBRARY}
                # TODO: hacky solution for linking in sdl and focus
                ${CMAKE_SOURCE_DIR}/focus/lib/focus.lib
                ${CMAKE_SOURCE_DIR}/focus/libs/windows/SDL2-static.lib
                ${CMAKE_SOURCE_DIR}/focus/libs/windows/SDL2main.lib
                #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath_Debug.lib
                sharedLibrary
                #focus
                #                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
                msvcrtd.lib
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
        target_link_libraries(
                PhysicsSim
                BulletDynamics_Debug
                BulletCollision_Debug
                LinearMath_Debug
                ${OPENGL_LIBRARY}
                # TODO: hacky solution for linking in sdl and focus
                ${CMAKE_SOURCE_DIR}/focus/lib/focus.lib
                ${CMAKE_SOURCE_DIR}/focus/libs/windows/SDL2-static.lib
                ${CMAKE_SOURCE_DIR}/focus/libs/windows/SDL2main.lib
                #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision_Debug.lib
                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath_Debug.lib
                sharedLibrary
                #focus
                #                glfw3
                opengl32.lib
                gdi32.lib
                user32.lib
                kernel32.lib
                msvcrtd.lib
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
    else ()
        #target_link_libraries(
        #IPhysics
        #${OPENGL_LIBRARY}
        #BulletDynamics.lib
        #         BulletCollision.lib
        #         LinearMath.lib
        #
        #         glfw3.lib
        #        opengl32.lib
        ##        gdi32.lib
        ##        user32.lib
        ##        kernel32.lib
        #)
    endif ()
    set_target_properties(VoxelGenerator PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
else ()
    message("not windows hit me")
    set(CMAKE_CXX_FLAGS "-std=c++2a -g -Wall -Wextra -pedantic -D_DEBUG -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE -DIMGUI_IMPL_OPENGL_LOADER_GLAD")
    link_directories(
            ${CMAKE_SOURCE_DIR}/libs/linux
            ${CMAKE_BINARY_DIR}/sharedLibrary
            ${CMAKE_SOURCE_DIR}/focus/lib;
    )
    #target_link_libraries(
    #         IPhysics
    #         sharedLibrary
    #         ${OPENGL_LIBRARY}
    #         focus
    #         libglfw3.a
    #         dl
    #         X11
    #         pthread
    # )
    target_link_libraries(
            VoxelGenerator
            sharedLibrary
            ${OPENGL_LIBRARY}
            # TODO: figure out a better way to do this, don't feel like fighting with cmake atm
            ${CMAKE_SOURCE_DIR}/focus/lib/libfocus.a
            /usr/local/lib/libSDL2.so
            dl
            X11
            pthread
    )
endif ()







