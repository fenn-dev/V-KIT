cmake_minimum_required(VERSION 3.16)
project(VulpineCompiler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Dependency Management (FetchContent) ---
include(FetchContent)

# Fetch GLFW (Windowing)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Fetch ImGui (UI)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking 
)
FetchContent_MakeAvailable(imgui)

# --- Compiler Flags ---
if(MSVC)
    add_compile_options($<$<NOT:$<CONFIG:Debug>>:/O2>)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra $<$<NOT:$<CONFIG:Debug>>:-O3>)
endif()

# --- Files and Headers ---
# Include project headers + ImGui headers + ImGui Backend headers
include_directories(
    include 
    V-KIT/token 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)

set(SOURCES
    main.cpp
    V-KIT/token/Tokenizer.cpp
    
    # ImGui Core
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    
    # ImGui Backends (Matches our GLFW + OpenGL setup)
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# --- Executable and Linking ---
add_executable(vulpine ${SOURCES})

# We need OpenGL for ImGui to render
find_package(OpenGL REQUIRED)

# Link everything together
# opengl32 is standard on Windows; for Linux, use ${OPENGL_LIBRARIES}
target_link_libraries(vulpine PRIVATE 
    glfw 
    OpenGL::GL
)