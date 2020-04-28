cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
project(anything LANGUAGES CXX)
include(safeguards)

include(GNUInstallDirs)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)
add_library(project_defines INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)


option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()


option(ENABLE_ASAN "Enable address sanitizer" FALSE)
if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
endif()


target_compile_options(project_warnings
    INTERFACE
    -Wall
    -Wextra
    -Wshadow
    )

option(ALL_WARNINGS "Enable all waring that the compiler supports" FALSE)
if(${ALL_WARNINGS})
    target_compile_options(project_warnings
        INTERFACE
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2)
endif()


option(LIBCPP "Build with libc++" FALSE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND ${ALL_WARNINGS})
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.3)
        target_compile_options(project_warnings INTERFACE
            -Wduplicated-branches)
    endif()
    target_compile_options(project_warnings INTERFACE
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
        -Wuseless-cast)
    target_link_libraries(project_options INTERFACE stdc++fs)
else()
    if(LIBCPP)
        target_compile_options(project_options INTERFACE -stdlib=libc++)
    else()
        target_link_libraries(project_options INTERFACE stdc++fs)
    endif()
endif()


option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()


option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK
            ${CPPCHECK}
            --suppress=syntaxError
            --enable=all
            --inconclusive)
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

# Project setup
file(READ "${PROJECT_SOURCE_DIR}/version" VERSION NEWLINE_CONSUME)
string(REPLACE "\n" "" VERSION "${VERSION}")
string(REPLACE "." ";" VERSION_LIST ${VERSION})
list(GET VERSION_LIST 0 VERSION_MAJOR)
list(GET VERSION_LIST 1 VERSION_MINOR)
list(GET VERSION_LIST 2 VERSION_PATCH)

target_compile_definitions(project_defines INTERFACE AY_VERSION_MAJOR=${VERSION_MAJOR})
target_compile_definitions(project_defines INTERFACE AY_VERSION_MINOR=${VERSION_MINOR})
target_compile_definitions(project_defines INTERFACE AY_VERSION_PATCH=${VERSION_PATCH})


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_include_directories(project_options INTERFACE include)

add_subdirectory(libs)
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/libs/include")

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)



option(GLCALLS_CHECKS "Check all OpenGL calls for errors" TRUE)
if (${GLCALLS_CHECKS})
    target_compile_definitions(project_defines INTERFACE OPENGL_CHECKS)
endif()

option(DEBUG_LOGGING "Debug logging" TRUE)
if (${GLCALLS_CHECKS})
    target_compile_definitions(project_defines INTERFACE DEBUG_LOGGING)
endif()


include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

# The testing example
add_library(anything_engine

    ./include/rendering/renderer.cpp


    ./include/rendering/texture.cpp
    ./include/rendering/frame_buffer.cpp
    ./include/rendering/depth_texture.cpp
    ./include/rendering/cube_texture.cpp
    ./include/rendering/renderer3D.cpp
    ./include/rendering/shaders.cpp
    ./include/rendering/vertex_array.cpp
    ./include/rendering/texture_binder.cpp

    ./include/graphics/materials/phong.cpp
    ./include/graphics/materials/solid_color.cpp
    ./include/graphics/materials/textured.cpp
    ./include/graphics/mesh_factory.cpp
    ./include/graphics/geometry.cpp
    ./include/graphics/geometry_factory.cpp

    ./include/application/resource_loader.cpp
    ./include/application/application.cpp
    ./include/application/window.cpp

    ./include/engine/game_base.cpp
    ./include/engine/engine.cpp
    ./include/engine/scene.cpp
    ./include/engine/raycaster.cpp
    ./include/engine/texture_library.cpp
    ./include/engine/shader_library.cpp

    ./include/engine/entities/axis_helper.cpp
    ./include/engine/entities/camera_helper.cpp
    ./include/engine/entities/grid_helper.cpp
    ./include/engine/entities/plane_helper.cpp
    ./include/engine/entities/axis_helper.cpp
    ./include/engine/entities/box_helper.cpp
    ./include/engine/entities/pointlight_helper.cpp
    ./include/engine/entities/spotlight_helper.cpp

    ./include/engine/components/orbital_camera.cpp

    ./include/util/gl_helpers.cpp
    ./include/util/files.cpp
    ./include/util/logging.cpp


    )

target_include_directories(anything_engine PUBLIC ./include)

target_link_libraries(anything_engine PUBLIC project_options project_warnings project_defines
    PUBLIC glfw OpenGL::GL GLEW::GLEW imgui assimp imguizmo fmt stb)

if(${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(anything_engine
        PUBLIC ./include/std_header.hpp ./include/glm_header.hpp)
endif()


add_executable(balls_scene ./examples/ball_scene.cpp)
target_link_libraries(balls_scene PUBLIC anything_engine)

add_executable(mat_scene ./examples/mat_scene.cpp)
target_link_libraries(mat_scene PUBLIC anything_engine)



file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS balls_scene
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})

install(DIRECTORY ./include/ DESTINATION include)

set(CPACK_PACKAGE_VENDOR "Stanislav Arnaudov")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Game engine written from scratch targeting linux and opengl4.")
set(CPACK_PACKAGE_VERSION_MAJOR ${CTG_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CTG_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CTG_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /.github
    /dist
    /.*build.*
    /\\\\.DS_Store)
include(CPack)
