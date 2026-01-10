set(API_NAME RainfallAPI CACHE INTERNAL "Global API name")



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
        ${portaudio_SOURCE_DIR}/include
)

# --- Link libraries ---
target_link_libraries(${API_NAME}
    PUBLIC
        portaudio
        BrassUtils::BrassUtils
        Taskflow::Taskflow
        Boost::headers
        Boost::bimap
        Boost::math
        Boost::static_string
)

# --- Optional compile flags from pkg-config ---
target_compile_options(${API_NAME} PRIVATE ${PORTAUDIO_CFLAGS_OTHER})
