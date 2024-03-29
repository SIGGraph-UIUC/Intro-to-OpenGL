cmake_minimum_required (VERSION 3.9.0)
project(opengl_intro)
set(CMAKE_CXX_STANDARD 17)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)
include_directories(extern/glfw/include)

# Add GLAD
include_directories(extern/glad/include)
set(GLAD_SOURCES
    extern/glad/src/glad.c)

# Add GLM
include_directories(extern/glm)

# Add tinyobjloader
include_directories(extern/tinyobjloader)

# Add stb_image
include_directories(extern/stb_image)

# Setup source code
include_directories(include)
set(MAIN_SOURCES
    src/main.cpp
    src/shader_util.cpp
    src/geometry_util.cpp
    src/camera_util.cpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${GLAD_SOURCES})
target_link_libraries(${PROJECT_NAME}
                      glfw ${GLFW_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC -DSHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")
target_compile_definitions(${PROJECT_NAME} PUBLIC -DRESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources")
