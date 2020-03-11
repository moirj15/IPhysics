cmake_minimum_required(VERSION 3.10)
project(IPhysics)

file(GLOB IMGUI_FILES "imgui/*")
file(GLOB SRC_FILES "src/*")
file(GLOB SRC_UTILS_FILES "src/utils/*")
file(GLOB SRC_RENDERER_FILES "src/renderer/*")
file(GLOB SRC_PHYSICS_ENGINE_FILES "src/PhysicsEngine/*")
file(GLOB SRC_VOXEL_GENERATOR "src/voxelGenerator/*")
file(GLOB SRC_INPUT_FILES "src/Input/*")
file(GLOB SRC_PHYSICS_SIMULATION_FILES "src/PhysicsSimulation/*")
file(GLOB GLEW_FILES "glew/*")


set(SOURCE_LIST_MAIN_PROJECT
	${IMGUI_FILES}
  ${GLEW_FILES}
	${SRC_UTILS_FILES}
	${SRC_RENDERER_FILES}
	${SRC_PHYSICS_ENGINE_FILES}
	${SRC_INPUT_FILES}
  ${SRC_PHYSICS_SIMULATION_FILES}

)

set(SOURCE_LIST_VOXEL_GENERATOR
	${IMGUI_FILES}
  ${GLEW_FILES}
  ${SRC_UTILS_FILES}
  ${SRC_VOXEL_GENERATOR}
	${SRC_RENDERER_FILES}
	${SRC_INPUT_FILES}
)


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
    D:/libraries/glfw/include;
    D:/libraries/glew-2.1.0/include;
    D:/libraries/ReactPhysics3D/src
    D:/libraries/bullet/include;
  )
  set(CMAKE_CXX_FLAGS "/std:c++17 /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLEW_STATIC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  target_link_libraries(
    IPhysics
    ${OPENGL_LIBRARY}
    D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
    D:/libraries/glfw/release/glfw3.lib
    #D:/libraries/ReactPhysics3D/Debug/reactphysics3d.lib
    D:/libraries/bullet/Debug/BulletDynamics_Debug.lib
    D:/libraries/bullet/Debug/BulletCollision_Debug.lib
    #D:/libraries/bullet/Release/Bullet3Common.lib
    D:/libraries/bullet/Debug/LinearMath_Debug.lib
    opengl32.lib
    gdi32.lib
    user32.lib
    kernel32.lib

  )
  message(${CMAKE_CXX_FLAGS})
  target_link_libraries(
    VoxelGenerator
    ${OPENGL_LIBRARY}
    D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
    D:/libraries/glfw/release/glfw3.lib
    D:/libraries/ReactPhysics3D/Debug/reactphysics3d.lib
    #D:/libraries/bullet/Release/Bullet3Dynamics.lib
    #D:/libraries/bullet/Release/Bullet3Collision.lib
    #D:/libraries/bullet/Release/Bullet3Common.lib
    #D:/libraries/bullet/Release/LinearMath.lib
    opengl32.lib
    gdi32.lib
    user32.lib
    kernel32.lib
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







