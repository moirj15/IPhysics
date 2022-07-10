cmake_minimum_required(VERSION 3.20)
project(IPhysics)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(IPhysics
        src/voxelizer/main.cpp
        src/voxelizer/HeadSystem.cpp
        src/voxelizer/UISystem.cpp
        src/voxelizer/tiny_obj_loader.cpp
        src/voxelizer/InputSystem.cpp
        src/voxelizer/RenderSystem.cpp
        src/voxelizer/PhysicsSystem.cpp
        src/voxelizer/ResourceSystem.cpp
        src/voxelizer/glad.c
        src/third_party/imgui/imgui.cpp
        src/third_party/imgui/imgui_draw.cpp
        src/third_party/imgui/imgui_tables.cpp
        src/third_party/imgui/imgui_widgets.cpp
        src/third_party/imgui/imgui_internal.h
        src/third_party/imgui/backends/imgui_impl_opengl3.h
        src/third_party/imgui/backends/imgui_impl_opengl3.cpp
        src/third_party/imgui/backends/imgui_impl_sdl.h
        src/third_party/imgui/backends/imgui_impl_sdl.cpp
        )

target_include_directories(IPhysics PRIVATE
        ${CMAKE_SOURCE_DIR}/include;
        ${CMAKE_SOURCE_DIR}/glad/include;
        ${CMAKE_SOURCE_DIR}/src/third_party/imgui;
        # TODO: this is just a temp fix for imgui requiring sdl headers to be included
        ${CMAKE_SOURCE_DIR}/include/SDL2
        )

link_directories(
        ${CMAKE_SOURCE_DIR}/libs/windows
        ${CMAKE_SOURCE_DIR}/libs/windows/bullet
        ${CMAKE_BINARY_DIR}/sharedLibrary
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
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

set_property(TARGET IPhysics PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:DEBUG>:Debug>")
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("DEBUG YOU FOOL!!!")
    target_link_libraries(
            IPhysics
            ${OPENGL_LIBRARY}
            # TODO: hacky solution for linking in sdl and focus
            ${CMAKE_SOURCE_DIR}/libs/windows/SDL2-static.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/SDL2main.lib
            #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision_Debug.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath_Debug.lib
            opengl32.lib
            gdi32.lib
            user32.lib
            kernel32.lib
            msvcrtd.lib
            winmm.lib
            imm32.lib
            version.lib
            libcmt.lib
            dxgi.lib
            dxguid.lib
            winmm.lib
            gdi32
            opengl32
            Setupapi.lib
    )

else ()
    message("RELEASE YOU FOOL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
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
    target_link_libraries(
            IPhysics
            ${OPENGL_LIBRARY}
            # TODO: hacky solution for linking in sdl and focus
            ${CMAKE_SOURCE_DIR}/libs/windows/SDL2-static.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/SDL2main.lib
            #                ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletDynamics_Debug.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet/BulletCollision.lib
            ${CMAKE_SOURCE_DIR}/libs/windows/bullet/LinearMath.lib
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
endif ()
