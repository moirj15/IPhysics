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


add_executable(
  IPhysics
  ${SOURCE_LIST_MAIN_PROJECT}
)

add_executable(
  VoxelGenerator
  ${SOURCE_LIST_VOXEL_GENERATOR}
)
find_package(OpenGL REQUIRED)
if (WIN32)
  message("WINDOWS")
  include_directories(
    D:/libraries/glm;
    G:/libraries/glfw-clang/include;
    G:/libraries/include/;
    D:/libraries/bullet/include;
    ${CMAKE_SOURCE_DIR}/sharedLibrary/include
  )
  add_subdirectory(sharedLibrary)

  set(CMAKE_CXX_FLAGS "/std:c++17 /MTd /MP /TP /W3 -D_CRT_SECURE_NO_WARNINGS -DGLEW_STATIC -DGLM_ENABLE_EXPERIMENTAL")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  message(${CMAKE_BUILD_TYPE})
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("DEBUG YOU FOOL!!!")
    target_link_libraries(
      IPhysics
      ${OPENGL_LIBRARY}
      G:/libraries/glfw-clang/glfw3.lib
      D:/libraries/bullet/Debug/BulletDynamics_Debug.lib
      D:/libraries/bullet/Debug/BulletCollision_Debug.lib
      D:/libraries/bullet/Debug/LinearMath_Debug.lib
      opengl32.lib
      gdi32.lib
      user32.lib
      kernel32.lib
    )
  else()
    target_link_libraries(
      IPhysics
      ${OPENGL_LIBRARY}
      D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
#      D:/libraries/glfw/release/glfw3.lib
            G:/libraries/glfw-clang/glfw3.lib
      D:/libraries/bullet/Release/BulletDynamics.lib
      D:/libraries/bullet/Release/BulletCollision.lib
      D:/libraries/bullet/Release/LinearMath.lib
      opengl32.lib
      gdi32.lib
      user32.lib
      kernel32.lib
    )
  endif()
elseif (MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG -DGLM_ENABLE_EXPERIMENTAL")
  include_directories(
    D:/libraries/glm;
    D:/libraries/glfw-3.2.1.bin.WIN64/include;
    D:/libraries/glew-2.1.0/include
    D:/libraries/ReactPhysics3D/src
  F:/Programs/msys2/mingw64/include/bullet;
#    D:/libraries/bullet/include;
  )
  target_link_libraries(
    IPhysics
    ${OPENGL_LIBRARY}
    D:/libraries/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3.a
    D:/libraries/glew-2.1.0/lib/Release/x64/glew32.dll
    #D:/libraries/ReactPhysics3D/lib/libreactphysics3d.a
          F:/Programs/msys2/mingw64/lib/libBulletDynamics_d.dll.a
    F:/Programs/msys2/mingw64/lib/libBulletCollision_d.dll.a
    F:/Programs/msys2/mingw64/lib/libLinearMath_d.dll.a
  )
  target_link_libraries(
    VoxelGenerator
    ${OPENGL_LIBRARY}
    D:/libraries/glfw-3.2.1.bin.WIN64/lib-mingw-w64/libglfw3.a
    D:/libraries/glew-2.1.0/lib/Release/x64/glew32.dll
    #D:/libraries/ReactPhysics3D/lib/libreactphysics3d.a
  )

  message(${CMAKE_CXX_FLAGS})
  target_link_libraries(
    VoxelGenerator
    ${OPENGL_LIBRARY}
    D:/libraries/glew-2.1.0/lib/Release/x64/glew32s.lib
#    D:/libraries/glfw/release/glfw3.lib
          G:/libraries/glfw-clang/glfw3.lib
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







