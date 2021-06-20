cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    main.cpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp
    imgui_wrapper.cpp
    imgui_wrapper.hpp
    )

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})

add_library(libTactile ${TACTILE_SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_FOREACH)
add_definitions(-DCENTURION_NO_SDL_TTF)
add_definitions(-DCENTURION_NO_SDL_MIXER)
add_definitions(-DCENTURION_NO_SDL_IMAGE)

target_include_directories(libTactile
    PUBLIC ${TACTILE_ROOT_DIR}/src
    PUBLIC commands
    PUBLIC commands/layers
    PUBLIC commands/maps
    PUBLIC commands/properties
    PUBLIC commands/tilesets
    PUBLIC commands/tools
    PUBLIC core
    PUBLIC core/algorithms
    PUBLIC core/aliases
    PUBLIC core/concepts
    PUBLIC core/layers
    PUBLIC core/map
    PUBLIC core/property
    PUBLIC core/tile
    PUBLIC core/tileset
    PUBLIC core/viewmodels
    PUBLIC core/viewmodels/layer
    PUBLIC core/viewmodels/property
    PUBLIC export
    PUBLIC export/map
    PUBLIC export/theme
    PUBLIC gui
    PUBLIC gui/base
    PUBLIC gui/dialog
    PUBLIC gui/dialog/about
    PUBLIC gui/dialog/add-property
    PUBLIC gui/dialog/change-property-name
    PUBLIC gui/dialog/export-theme
    PUBLIC gui/dialog/import-theme
    PUBLIC gui/dialog/open-map
    PUBLIC gui/dialog/open-tileset
    PUBLIC gui/dialog/resize-map
    PUBLIC gui/dialog/save
    PUBLIC gui/dialog/select-file
    PUBLIC gui/editor
    PUBLIC gui/layer
    PUBLIC gui/properties
    PUBLIC gui/rendering
    PUBLIC gui/settings
    PUBLIC gui/startup
    PUBLIC gui/status
    PUBLIC gui/tileset
    PUBLIC gui/tileset/tabs
    PUBLIC gui/tileset/tabs/contents
    PUBLIC gui/tools
    PUBLIC gui/window
    PUBLIC mouse-tools
    PUBLIC parsing
    PUBLIC parsing/map
    PUBLIC preferences
    PUBLIC util

    SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS}
    SYSTEM PUBLIC ${OPENGL_INCLUDE_DIRS}
    SYSTEM PUBLIC ${SDL2_INCLUDE_DIR}
    SYSTEM PUBLIC ${LIBRARY_DIR}/imgui
    )

target_link_libraries(libTactile
    PUBLIC Qt6::Core
    PUBLIC Qt6::Widgets
    PUBLIC Qt6::OpenGLWidgets
    PUBLIC Qt6::Gui
    PUBLIC Qt6::Xml
    PUBLIC libNenya
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC libCenturion
    )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libTactile PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wdouble-promotion
      -Wswitch-default
      -Wswitch-enum
      -Wunused
      -Wuninitialized
      -Wsuggest-final-types
      -Wsuggest-final-methods
      -Wsuggest-override
      -Wduplicated-cond
      -Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(libTactile PRIVATE
      /EHsc
      /MP
      /W3)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(libTactile PRIVATE
      -Xclang
      -Wall
      -Wextra
      -Wpedantic
      -Wdouble-promotion
      -Wswitch-default
      -Wswitch-enum
      -Wunused
      -Wuninitialized
      -Wconversion
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-unused-template
      -Wno-unused-macros
      -Wno-newline-eof
      -Wno-documentation-unknown-command)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(Tactile WIN32
      ${TACTILE_SOURCE_FILES}
      main.cpp)
else ()
  add_executable(Tactile
      ${TACTILE_SOURCE_FILES}
      main.cpp)
endif ()


add_dependencies(Tactile libTactile)

target_link_libraries(Tactile PUBLIC libTactile)

copy_directory_post_build(Tactile
    ${TACTILE_ROOT_DIR}/bin
    ${CMAKE_CURRENT_BINARY_DIR})