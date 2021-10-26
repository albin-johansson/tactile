cmake_minimum_required(VERSION 3.16)

project(tactile CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARY_DIR ${ROOT_DIR}/lib)
set(RESOURCE_DIR ${ROOT_DIR}/resources)

# Target names
set(TACTILE_PROTO prototactile)
set(TACTILE_LIB libtactile)
set(TACTILE_EXE Tactile)
set(TACTILE_TEST TactileTests)

set(TACTILE_IO_IMPORT_LIBRARY "${CMAKE_BINARY_DIR}/modules/tactile-io/TactileIO${CMAKE_IMPORT_LIBRARY_SUFFIX}")

list(APPEND CMAKE_PREFIX_PATH
    "$ENV{GLEW_LIBRARIES}"
    "${LIBRARY_DIR}/glew")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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

set(PUGIXML_SOURCES
    ${LIBRARY_DIR}/pugixml/pugiconfig.hpp
    ${LIBRARY_DIR}/pugixml/pugixml.cpp
    ${LIBRARY_DIR}/pugixml/pugixml.hpp)

include(Utilities)
include(TactileDependencies)

add_subdirectory(modules/tactile-proto)
add_subdirectory(modules/tactile-io)
add_subdirectory(modules/tactile)
add_subdirectory(test)