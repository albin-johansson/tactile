cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    application.cpp
    application.hpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp
    main.cpp

    aliases/col.hpp
    aliases/czstring.hpp
    aliases/ints.hpp
    aliases/layer_id.hpp
    aliases/map_id.hpp
    aliases/maybe.hpp
    aliases/object_id.hpp
    aliases/object_ref.hpp
    aliases/row.hpp
    aliases/shared.hpp
    aliases/tile_id.hpp
    aliases/tileset_id.hpp
    aliases/unique.hpp

    core/document.hpp
    core/document_delegate.cpp
    core/document_delegate.hpp
    core/map_document.cpp
    core/map_document.hpp
    core/model.cpp
    core/model.hpp
    core/tactile_error.hpp

    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp
    core/algorithms/invoke_n.hpp

    core/commands/command_stack.cpp
    core/commands/command_stack.hpp
    core/commands/command.hpp
    core/commands/command.cpp

    core/events/add_map_event.hpp
    core/events/open_map_event.hpp
    core/events/quit_event.hpp
    core/events/redo_event.hpp
    core/events/undo_event.hpp

    core/map/map.cpp
    core/map/map.hpp
    core/map/map_position.hpp

    core/map/layers/layer.hpp
    core/map/layers/layer_delegate.cpp
    core/map/layers/layer_delegate.hpp
    core/map/layers/layer_type.hpp
    core/map/layers/layer_utils.cpp
    core/map/layers/layer_utils.hpp
    core/map/layers/object.cpp
    core/map/layers/object.hpp
    core/map/layers/object_layer.cpp
    core/map/layers/object_layer.hpp
    core/map/layers/object_type.hpp
    core/map/layers/tile_layer.cpp
    core/map/layers/tile_layer.hpp

    core/properties/property.cpp
    core/properties/property.hpp
    core/properties/property_context.hpp
    core/properties/property_delegate.cpp
    core/properties/property_delegate.hpp
    core/properties/property_type.hpp

    core/tileset/tileset.cpp
    core/tileset/tileset.hpp
    core/tileset/tileset_manager.cpp
    core/tileset/tileset_manager.hpp
    core/tileset/tileset_selection.hpp

    gui/imgui_wrapper.cpp
    gui/imgui_wrapper.hpp
    gui/show_about_tactile.cpp
    gui/show_about_tactile.hpp
    gui/show_file_dialog.cpp
    gui/show_file_dialog.hpp
    gui/show_grid.cpp
    gui/show_grid.hpp
    gui/show_gui.cpp
    gui/show_gui.hpp
    gui/show_layers.cpp
    gui/show_layers.hpp
    gui/show_map_viewport.cpp
    gui/show_map_viewport.hpp
    gui/show_menu_bar.cpp
    gui/show_menu_bar.hpp
    gui/show_properties.cpp
    gui/show_properties.hpp
    gui/show_settings.cpp
    gui/show_settings.hpp
    gui/show_tilesets.cpp
    gui/show_tilesets.hpp
    gui/show_tool_bar.cpp
    gui/show_tool_bar.hpp
    gui/themes.cpp
    gui/themes.hpp
    )

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})

add_library(libTactile ${TACTILE_SOURCE_FILES})

add_definitions(-DCENTURION_NO_SDL_TTF)
add_definitions(-DCENTURION_NO_SDL_MIXER)
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)
add_definitions(-DMSVC) # ImGuiFileDialog needs this to avoid unsafe C-functions

target_include_directories(libTactile
    PUBLIC ${TACTILE_ROOT_DIR}/src

    SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS}
    SYSTEM PUBLIC ${OPENGL_INCLUDE_DIRS}
    SYSTEM PUBLIC ${SDL2_INCLUDE_DIR}
    SYSTEM PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS}
    SYSTEM PUBLIC ${LIBRARY_DIR}/imgui)

target_link_libraries(libTactile
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC ${SDL2_IMAGE_LIBRARIES}
    PUBLIC libCenturion
    PUBLIC libNenya
    PUBLIC libJson
    PUBLIC libRune
    PUBLIC libEntt)

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

if (WIN32)
  copy_directory_post_build(Tactile
      ${TACTILE_ROOT_DIR}/bin
      ${CMAKE_CURRENT_BINARY_DIR})
endif ()

copy_directory_post_build(Tactile
    ${RESOURCES_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/resources)