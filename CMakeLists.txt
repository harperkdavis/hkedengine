cmake_minimum_required(VERSION 3.21)
project(hkedengine)

set(CMAKE_CXX_STANDARD 17)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES main.cpp dep/GLAD/src/glad.c)
set(HEADER_FILES engine/shader.h engine/graphics.cpp engine/graphics.h dep/stb_image.h engine/input.cpp engine/input.h)

file(COPY resources DESTINATION ${dir}/cmake-build-debug)
file(COPY shaders DESTINATION ${dir}/cmake-build-debug)

include_directories(${PROJECT_SOURCE_DIR}/dep/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/dep/GLAD/include)

link_directories(${PROJECT_SOURCE_DIR}/dep/glfw/lib-mingw-w64)

add_executable(hkedengine ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(hkedengine glfw3 gdi32 opengl32)
