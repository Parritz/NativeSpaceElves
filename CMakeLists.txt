cmake_minimum_required(VERSION 3.10)
project(spaceelves)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files and assets
file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add GLAD source file
set(GLAD_SRC include/glad.c)

# Executable
add_executable(spaceelves ${SOURCES} ${GLAD_SRC})

# Link GLFW library
target_link_libraries(spaceelves ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
