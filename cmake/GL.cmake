cmake_minimum_required(VERSION 3.10.0)

include(FetchContent)
# ---------------- GLFW ----------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.10
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)

FetchContent_MakeAvailable(glfw)

# ---------------- GLEW ----------------
set (GLEW_VERSION "2.1.0")
set (GLEW_URL "https://download.sourceforge.net/sourceforge/project/glew/glew/${GLEW_VERSION}/glew-${GLEW_VERSION}.zip")
set (GLEW_HASH "SHA256=2700383d4de2455f06114fbaf872684f15529d4bdc5cdea69b5fb0e9aa7763f1")

FetchContent_Declare(glew
	URL "${GLEW_URL}"
	URL_HASH "${GLEW_HASH}"
	SOURCE_SUBDIR "build/cmake"
)

FetchContent_MakeAvailable(glew)

# ---------------- OpenGL -----w3-----------
# Modern CMake provides a built-in OpenGL package

# --- OpenGL ---
cmake_policy(SET CMP0072 NEW) # Prefer GLVND
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()
