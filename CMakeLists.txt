cmake_minimum_required(VERSION 3.60)
project(OpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Dependencies/spdlog)
add_subdirectory(Dependencies/imgui-cmake)

add_subdirectory(OpenGL)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OpenGL)

target_link_libraries(OpenGL PRIVATE spdlog imgui)