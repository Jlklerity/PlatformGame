cmake_minimum_required(VERSION 3.10)
project(PlatformGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Include your own headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW (disable extras)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# FreeType
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
add_subdirectory(external/freetype)

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# Source files
set(SOURCES
    src/main.cpp
    src/Shader.cpp
    src/TextRenderer.cpp
    src/Collision.cpp
    src/orb.cpp
    src/Cannon.cpp
    src/Wall.cpp
    src/Texture.cpp
)

add_executable(PlatformGame ${SOURCES})

# Platform-specific OpenGL linking
if(APPLE)
    target_link_libraries(PlatformGame PRIVATE glad glfw freetype glm "-framework OpenGL")
elseif(WIN32)
    target_link_libraries(PlatformGame PRIVATE glad glfw freetype glm opengl32)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
