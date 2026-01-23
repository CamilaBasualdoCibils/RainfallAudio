set(EDITOR_NAME RainfallEditor CACHE INTERNAL "Global Editor name")

# --- Collect source files for the executable ---
file(GLOB_RECURSE EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
# --- Build executable ---
add_executable(${EDITOR_NAME} ${EDITOR_SOURCES})
add_dependencies(${EDITOR_NAME} ${API_NAME})


# --- Include directories ---
target_include_directories(${EDITOR_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        # ${imgui_SOURCE_DIR}
        # ${implot_SOURCE_DIR}
        ${glfw_SOURCE_DIR}/include
        ${glew_SOURCE_DIR}/include
        #${IMNODEFLOW_DIR}/include
        ${imnodeflow_SOURCE_DIR}/include

)
# --- Link libraries ---
target_link_libraries(${EDITOR_NAME}
    PRIVATE
        ${API_NAME}
        glfw
        glew_s
        imgui_lib
        implot_lib
        Boost::stacktrace_addr2line
        OpenGL::GL
        Qt6::Widgets
)
target_compile_definitions(${EDITOR_NAME} PRIVATE GLEW_STATIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
