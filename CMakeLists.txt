cmake_minimum_required (VERSION 3.9.0)
project(opengl_intro LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)
include_directories(extern/glfw/include)

# Add GlAD
include_directories(extern/glad/include)
set(GLAD_SOURCES
    extern/glad/src/glad.c)

# Setup source code
include_directories(include)
set(MAIN_SOURCES
    src/main.cpp
    src/util.cpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${GLAD_SOURCES})
target_link_libraries(${PROJECT_NAME}
                      glfw ${GLFW_LIBRARIES})
