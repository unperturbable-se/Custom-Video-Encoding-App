cmake_minimum_required(VERSION 3.10)
project(ImGuiApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main 
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    implot/implot.cpp
    implot/implot_items.cpp
    globals.cpp
    gui_abstraction.cpp
    main.cpp
)

# Add BOTH directories - the root AND the imgui folder
target_include_directories(main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(main PRIVATE glfw OpenGL::GL)