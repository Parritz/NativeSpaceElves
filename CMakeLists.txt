cmake_minimum_required(VERSION 3.10)
project(spaceelves)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES src/*.cpp src/*.hpp)

# Add GLAD source file
set(GLAD_SRC include/glad.c)

# Executable
add_executable(spaceelves ${SOURCES} ${GLAD_SRC})

# Link GLFW library
target_link_libraries(spaceelves ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
