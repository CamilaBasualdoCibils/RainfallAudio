include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
#ImNodeFlow
#FetchContent_Declare(ImNodeFlow
#     GIT_REPOSITORY "https://github.com/Fattorino/ImNodeFlow.git"
#    GIT_PROGRESS TRUE
#)
##FetchContent_Populate(ImNodeFlow)
#FetchContent_MakeAvailable(ImNodeFlow)


# ImGui
add_compile_definitions(IMGUI_DISABLE_OBSOLETE_FUNCTIONS IMGUI_DEFINE_MATH_OPERATORS)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.5-docking
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(imgui)

# ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.17
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(implot)

# Add ImGui + ImPlot sources manually
add_library(imgui_lib
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
)

target_link_libraries(imgui_lib PUBLIC glfw OpenGL::GL)

add_library(implot_lib
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
)

target_include_directories(implot_lib PUBLIC
    ${implot_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(implot_lib PUBLIC imgui_lib)