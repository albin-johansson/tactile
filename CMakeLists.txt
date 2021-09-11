cmake_minimum_required(VERSION 3.18)

project(tactile CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 23)

include(Utilities)

set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)
set(TACTILE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/resources)

list(APPEND CMAKE_PREFIX_PATH "$ENV{GLEW_LIBRARIES}")
list(APPEND CMAKE_PREFIX_PATH "${LIBRARY_DIR}/glew")

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
    ${LIBRARY_DIR}/imgui/imstb_truetype.h

    ${LIBRARY_DIR}/imgui/ImGuiFileDialog.cpp
    ${LIBRARY_DIR}/imgui/ImGuiFileDialog.h
    ${LIBRARY_DIR}/imgui/ImGuiFileDialogConfig.h)

set(PUGIXML_SOURCES
    ${LIBRARY_DIR}/pugixml/pugiconfig.hpp
    ${LIBRARY_DIR}/pugixml/pugixml.cpp
    ${LIBRARY_DIR}/pugixml/pugixml.hpp)

add_header_only_lib(libNenya ${LIBRARY_DIR}/nenya ${LIBRARY_DIR}/nenya/nenya.hpp)
add_header_only_lib(libStbImage ${LIBRARY_DIR}/stbimage ${LIBRARY_DIR}/stbimage/stb_image.h)
add_header_only_lib(libCenturion ${LIBRARY_DIR}/centurion ${LIBRARY_DIR}/centurion/centurion.hpp)
add_header_only_lib(libEntt ${LIBRARY_DIR}/entt ${LIBRARY_DIR}/entt/entt.hpp)
add_header_only_lib(libJson ${LIBRARY_DIR}/json ${LIBRARY_DIR}/json/json.hpp)
add_header_only_lib(libFontAwesome ${LIBRARY_DIR}/fontawesome ${LIBRARY_DIR}/fontawesome/IconsFontAwesome5.h)
add_header_only_lib(libPFD ${LIBRARY_DIR}/pfd ${LIBRARY_DIR}/pfd/portable-file-dialogs.h)

add_subdirectory(src)
add_subdirectory(test)