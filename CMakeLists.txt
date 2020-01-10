#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_CXX_COMPILER_WORKS 1)
#set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
#set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
cmake_minimum_required(VERSION 3.10)
project(DView)

file(GLOB IMGUI_FILES "imgui/*")
file(GLOB SRC_FILES "src/*")
file(GLOB SRC_RENDERER_FILES "src/renderer/*")


set(SOURCE_LIST 
	${SRC_FILES}
  ${SRC_RENDERER_FILES}
	${IMGUI_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

add_executable(
  DView
    ${SOURCE_LIST}
)
find_package(OpenGL REQUIRED)

if (WIN32)
    message("WINDOWS")
    include_directories(
        D:/libraries/glm;
        D:/libraries/glfw-3.2.1.bin.WIN64/include;
        D:/libraries/glew-2.1.0/include
    )
  set(CMAKE_CXX_FLAGS "/std:c++17  /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
	#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
#    #add_definitions(-DGLEW_STATIC=1)
    target_link_libraries(
       DView 
        ${OPENGL_LIBRARY}
        D:/libraries/glew-2.1.0/lib/Release/x64/glew32.lib
        D:/libraries/glfw-3.3.bin.WIN64/lib-vc2019/glfw3.lib
        )
else()
message("not windows hit me")
	set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
    target_link_libraries(
      DView

        GL
        GLEW
        glfw
	)
endif ()







