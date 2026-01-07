cmake_minimum_required(VERSION 3.16)

set(API_NAME RainfallAPI CACHE INTERNAL "Global API name")

# --- Boost ---
find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS}")
endif()

# --- PortAudio via pkg-config ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
message(STATUS "PortAudio include dirs: ${PORTAUDIO_INCLUDE_DIRS}")
message(STATUS "PortAudio libs: ${PORTAUDIO_LIBRARIES}")

# --- Collect source files ---
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# --- Option to build shared or static library ---
option(BUILD_SHARED_LIBS "Build RainfallAPI as a shared library" ON)

if(BUILD_SHARED_LIBS)
    add_library(${API_NAME} SHARED ${SOURCES})
    target_compile_definitions(${API_NAME} PRIVATE RAINFALLAPI_EXPORTS)
    message(STATUS "Building ${API_NAME} as a shared library")
else()
    add_library(${API_NAME} STATIC ${SOURCES})
    target_compile_definitions(${API_NAME} PUBLIC RAINFALLAPI_STATIC)
    message(STATUS "Building ${API_NAME} as a static library")
endif()

# --- Include directories ---
target_include_directories(${API_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        ${Boost_INCLUDE_DIRS}
        ${PORTAUDIO_INCLUDE_DIRS}
)

# --- Link libraries ---
target_link_libraries(${API_NAME}
    PUBLIC
        ${Boost_LIBRARIES}
        ${PORTAUDIO_LIBRARIES}
)

# --- Optional compile flags from pkg-config ---
target_compile_options(${API_NAME} PRIVATE ${PORTAUDIO_CFLAGS_OTHER})

# --- Message about library type ---
if(BUILD_SHARED_LIBS)
    message(STATUS "Building ${API_NAME} as a shared library")
else()
    message(STATUS "Building ${API_NAME} as a static library")
endif()
