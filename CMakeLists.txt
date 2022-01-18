cmake_minimum_required(VERSION 3.21)
project(hkedengine)

set(CMAKE_CXX_STANDARD 17)

include_directories(hkedengine ${PROJECT_SOURCE_DIR}/dep/glfw/include ${PROJECT_SOURCE_DIR}/dep/glew/include)
link_directories(hkedengine ${PROJECT_SOURCE_DIR}/dep/glfw/lib-mingw-w64 ${PROJECT_SOURCE_DIR}/dep/glew/lib)

add_executable(hkedengine main.cpp)

target_link_libraries(hkedengine glfw3 gdi32 opengl32)
