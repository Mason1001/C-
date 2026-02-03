cmake_minimum_required(VERSION 3.15)
project(MouseTracker VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find packages
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs/imgui
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends
    ${PROJECT_SOURCE_DIR}/libs/nlohmann
    ${PROJECT_SOURCE_DIR}/libs/glad/include
    ${PROJECT_SOURCE_DIR}/libs/glfw/include
)

# ImGui sources
set(IMGUI_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp
)

# GLAD sources
set(GLAD_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/glad/src/glad.c
)

# Application sources
set(APP_SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/Config.cpp
    ${PROJECT_SOURCE_DIR}/src/Logger.cpp
    ${PROJECT_SOURCE_DIR}/src/MouseTracker.cpp
    ${PROJECT_SOURCE_DIR}/src/UI.cpp
    ${PROJECT_SOURCE_DIR}/src/SplashScreen.cpp
)

# GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)

# Create executable
add_executable(${PROJECT_NAME} 
    ${APP_SOURCES}
    ${IMGUI_SOURCES}
    ${GLAD_SOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
)

# Platform specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} user32 gdi32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} dl pthread X11 Xtst)
endif()

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY assets DESTINATION .)
