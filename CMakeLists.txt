cmake_minimum_required(VERSION 3.21)
project(hkedengine)

set(CMAKE_CXX_STANDARD 17)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES main.cpp dep/GLAD/src/glad.c)
set(HEADER_FILES engine/dep/obj_loader.h engine/shader.h engine/dep/stb_image.h engine/input.h engine/mesh.cpp engine/mesh.h engine/material.cpp engine/material.h engine/scene.cpp engine/scene.h engine/pipeline.cpp engine/pipeline.h engine/ui.cpp engine/ui.h)

file(COPY resources DESTINATION ${dir}/cmake-build-debug)
file(COPY shaders DESTINATION ${dir}/cmake-build-debug)

add_subdirectory(${PROJECT_SOURCE_DIR}/dep/reactphysics3d)

include_directories(${PROJECT_SOURCE_DIR}/dep/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/reactphysics3d/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/GLAD/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/spdlog/include)

link_directories(${PROJECT_SOURCE_DIR}/dep/glfw/lib-mingw-w64)
link_directories(${PROJECT_SOURCE_DIR}/dep/reactphysics3d/build)

add_executable(hkedengine ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(hkedengine ReactPhysics3D glfw3 gdi32 opengl32)
