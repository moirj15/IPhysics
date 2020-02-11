cmake_minimum_required(VERSION 3.10)
project(IPhysics)

file(GLOB IMGUI_FILES "imgui/*")
file(GLOB SRC_FILES "src/*")
file(GLOB SRC_UTILS_FILES "src/utils/*")
file(GLOB SRC_RENDERER_FILES "src/renderer/*")
file(GLOB SRC_PHYSICS_FILES "src/physics/*")
file(GLOB SRC_VOXEL_GENERATOR "src/voxelGenerator/*")


set(SOURCE_LIST_MAIN_PROJECT
	${SRC_UTILS_FILES}
	${SRC_RENDERER_FILES}
	${SRC_PHYSICS_FILES}
	${IMGUI_FILES}
)

set(SOURCE_LIST_VOXEL_GENERATOR
    ${SRC_UTILS_FILES}
    ${SRC_VOXEL_GENERATOR}
	${SRC_RENDERER_FILES}
	${IMGUI_FILES}
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

add_executable(
    IPhysics
    ${SOURCE_LIST_MAIN_PROJECT}
)

add_executable(
    VoxelGenerator
    ${SOURCE_LIST_VOXEL_GENERATOR}
)
find_package(OpenGL REQUIRED)
if (MSYS OR MINGW)
    set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
    include_directories(
            D:/libraries/glm;
            D:/libraries/glfw-3.2.1.bin.WIN64/include;
            D:/libraries/glew-2.1.0/include
            D:/libraries/ReactPhysics3D/src
    )
    target_link_libraries(
            IPhysics
            ${OPENGL_LIBRARY}
            D:/libraries/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3.a
            D:/libraries/glew-2.1.0/lib/Release/x64/glew32.dll
            D:/libraries/ReactPhysics3D/lib/libreactphysics3d.a
    )
    target_link_libraries(
            VoxelGenerator
            ${OPENGL_LIBRARY}
            D:/libraries/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3.a
            D:/libraries/glew-2.1.0/lib/Release/x64/glew32.dll
            D:/libraries/ReactPhysics3D/lib/libreactphysics3d.a
    )


elseif (WIN32)
    message("WINDOWS")
    include_directories(
        D:/libraries/glm;
        D:/libraries/glfw-3.2.1.bin.WIN64/include;
        D:/libraries/glew-2.1.0/include;
        D:/libraries/ReactPhysics3D/src
    )
  set(CMAKE_CXX_FLAGS "/std:c++17  /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
	#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
#    #add_definitions(-DGLEW_STATIC=1)
    target_link_libraries(
            IPhysics
        ${OPENGL_LIBRARY}
        D:/libraries/glew-2.1.0/lib/Release/x64/glew32.lib
        D:/libraries/glfw-3.3.bin.WIN64/lib-vc2019/glfw3.lib
        D:/libraries/ReactPhysics3D/Debug/reactphysics3d.lib
        )
    target_link_libraries(
            VoxelGenerator
        ${OPENGL_LIBRARY}
        D:/libraries/glew-2.1.0/lib/Release/x64/glew32.lib
        D:/libraries/glfw-3.3.bin.WIN64/lib-vc2019/glfw3.lib
        D:/libraries/ReactPhysics3D/Debug/reactphysics3d.lib
        )
else()
message("not windows hit me")
set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
include_directories(
        ~/libraries/reactphysics3d/src
)
target_link_libraries(
        IPhysics
        GL
        GLEW
        glfw
        ~/libraries/reactphysics3d/lib/reactphysics3d.a
)
endif ()







