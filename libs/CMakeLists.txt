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

add_library(imgui ${srcs})
target_include_directories(imgui PUBLIC ./imgui)
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)


add_library(imguizmo
    ./imguizmo/ImCurveEdit.cpp
    ./imguizmo/ImGradient.cpp
    ./imguizmo/ImGuizmo.cpp
    ./imguizmo/ImSequencer.cpp)

target_link_libraries(imguizmo PUBLIC imgui)

target_include_directories(imguizmo PUBLIC ./imguizmo )
set_property(TARGET imguizmo PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(fmt)


add_library(stb ./src/stb.cpp)
target_include_directories(stb PUBLIC ./include)
set_property(TARGET stb PROPERTY POSITION_INDEPENDENT_CODE ON)

