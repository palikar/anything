

set(inst_src_dir include/imgui_src)
set(root ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(srcs
    ${root}/imgui.cpp
    ${root}/imgui_impl_glfw.cpp
    ${root}/imgui_impl_opengl3.cpp
    ${root}/imgui_widgets.cpp
    ${root}/imgui_demo.cpp
    ${root}/imgui_draw.cpp
    
    )

set(inst_srcs ${inst_src_dir}/imgui.cpp)

add_library(imgui INTERFACE)



target_sources(imgui INTERFACE    
    $<BUILD_INTERFACE:${srcs}>
    $<INSTALL_INTERFACE:${inst_srcs}>)



target_include_directories(imgui INTERFACE ./imgui)
# set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)
