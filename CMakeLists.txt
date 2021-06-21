cmake_minimum_required(VERSION 3.12)

project(tactile CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)

include(Utilities)

set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)
set(TACTILE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND CMAKE_PREFIX_PATH "$ENV{GLEW_INCLUDE_DIRS}")
list(APPEND CMAKE_PREFIX_PATH "$ENV{GLEW_LIBRARIES}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_header_only_lib(libNenya ${LIBRARY_DIR}/nenya ${LIBRARY_DIR}/nenya/nenya.hpp)

set(IMGUI_SOURCES
    ${LIBRARY_DIR}/imgui/imconfig.h
    ${LIBRARY_DIR}/imgui/imgui.cpp
    ${LIBRARY_DIR}/imgui/imgui.h
    ${LIBRARY_DIR}/imgui/imgui_demo.cpp
    ${LIBRARY_DIR}/imgui/imgui_draw.cpp
    ${LIBRARY_DIR}/imgui/imgui_impl_opengl3.cpp
    ${LIBRARY_DIR}/imgui/imgui_impl_opengl3.h
    ${LIBRARY_DIR}/imgui/imgui_impl_sdl.cpp
    ${LIBRARY_DIR}/imgui/imgui_impl_sdl.h
    ${LIBRARY_DIR}/imgui/imgui_internal.h
    ${LIBRARY_DIR}/imgui/imgui_tables.cpp
    ${LIBRARY_DIR}/imgui/imgui_widgets.cpp
    ${LIBRARY_DIR}/imgui/imstb_rectpack.h
    ${LIBRARY_DIR}/imgui/imstb_textedit.h
    ${LIBRARY_DIR}/imgui/imstb_truetype.h)

set(RUNE_SOURCES
    ${LIBRARY_DIR}/rune/vector_map.hpp
    ${LIBRARY_DIR}/rune/rune_error.hpp)
add_header_only_lib(libRune ${LIBRARY_DIR}/rune ${RUNE_SOURCES})

add_header_only_lib(libCenturion ${LIBRARY_DIR}/centurion ${LIBRARY_DIR}/centurion/centurion.hpp)
add_header_only_lib(libEntt ${LIBRARY_DIR}/entt ${LIBRARY_DIR}/entt/entt.hpp)
add_header_only_lib(libJson ${LIBRARY_DIR}/json ${LIBRARY_DIR}/json/json.hpp)

add_subdirectory(src)
add_subdirectory(test)