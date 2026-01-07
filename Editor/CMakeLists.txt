cmake_minimum_required(VERSION 3.16)

set(EDITOR_NAME RainfallEditor CACHE INTERNAL "Global Editor name")

# --- Collect source files for the executable ---
file(GLOB_RECURSE EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# --- Build executable ---
add_executable(${EDITOR_NAME} ${EDITOR_SOURCES})
add_dependencies(${EDITOR_NAME} ${API_NAME})

# --- OpenGL ---
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

# --- GLEW ---
find_package(GLEW REQUIRED)
if(GLEW_FOUND)
    message(STATUS "Found GLEW: ${GLEW_INCLUDE_DIRS}")
endif()

# --- GLFW ---
find_package(glfw3 3.3 REQUIRED)
if(GLFW3_FOUND)
    message(STATUS "Found GLFW")
endif()

# --- ImGui ---
# Nix usually installs only headers + static lib somewhere under /nix/store
# If a proper CMake config doesn't exist, you can do this:
find_path(IMGUI_INCLUDE_DIRS "imgui.h")
find_library(IMGUI_LIBRARIES NAMES imgui)
if(IMGUI_INCLUDE_DIRS AND IMGUI_LIBRARIES)
    message(STATUS "Found ImGui: ${IMGUI_LIBRARIES}")
else()
    message(FATAL_ERROR "ImGui not found")
endif()

# --- ImPlot ---
find_path(IMPLOT_INCLUDE_DIRS "implot.h")
find_library(IMPLOT_LIBRARIES NAMES implot)
if(IMPLOT_INCLUDE_DIRS AND IMPLOT_LIBRARIES)
    message(STATUS "Found ImPlot: ${IMPLOT_LIBRARIES}")
else()
    message(FATAL_ERROR "ImPlot not found")
endif()
# --- Include directories ---
target_include_directories(${EDITOR_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        ${IMGUI_INCLUDE_DIRS}
        ${IMPLOT_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}

)


# --- Link libraries ---
target_link_libraries(${EDITOR_NAME}
    PRIVATE
         ${API_NAME}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${IMGUI_LIBRARIES}
        ${IMPLOT_LIBRARIES}
        ${OpenGL_GL_LIBRARY}
)

# --- Optional definitions ---
target_compile_definitions(${EDITOR_NAME} PRIVATE "IMGUI_IMPL_OPENGL_LOADER_GLEW")
