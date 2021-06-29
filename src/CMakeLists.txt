cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    application.cpp
    application.hpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp

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

    core/events/add_column_event.hpp
    core/events/add_map_event.hpp
    core/events/add_row_event.hpp
    core/events/add_tileset_event.hpp
    core/events/center_viewport_event.hpp
    core/events/close_map_event.hpp
    core/events/open_map_event.hpp
    core/events/quit_event.hpp
    core/events/redo_event.hpp
    core/events/remove_column_event.hpp
    core/events/remove_row_event.hpp
    core/events/remove_tileset_event.hpp
    core/events/select_layer_event.hpp
    core/events/select_map_event.hpp
    core/events/select_tileset_event.hpp
    core/events/set_property_value_event.hpp
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

    filesystem/get_persistent_file_path.cpp
    filesystem/get_persistent_file_path.hpp

    gui/canvas_info.hpp
    gui/canvas_info.cpp
    gui/cursors.cpp
    gui/cursors.hpp
    gui/get_texture_id.cpp
    gui/get_texture_id.hpp
    gui/imgui_context.cpp
    gui/imgui_context.hpp
    gui/show_grid.cpp
    gui/show_grid.hpp
    gui/style.cpp
    gui/style.hpp
    gui/update_gui.cpp
    gui/update_gui.hpp
    gui/themes.cpp
    gui/themes.hpp

    gui/layout/dock_space.cpp
    gui/layout/dock_space.hpp
    gui/layout/load_default_layout.cpp
    gui/layout/load_default_layout.hpp

    gui/widgets/button_ex.cpp
    gui/widgets/button_ex.hpp
    gui/widgets/file_dialog.cpp
    gui/widgets/file_dialog.hpp
    gui/widgets/help_marker.cpp
    gui/widgets/help_marker.hpp
    gui/widgets/mouse_tracker.cpp
    gui/widgets/mouse_tracker.hpp

    gui/widgets/dialogs/about_dialog.cpp
    gui/widgets/dialogs/about_dialog.hpp
    gui/widgets/dialogs/credits_dialog.cpp
    gui/widgets/dialogs/credits_dialog.hpp
    gui/widgets/dialogs/settings_dialog.cpp
    gui/widgets/dialogs/settings_dialog.hpp
    gui/widgets/dialogs/tileset_dialog.cpp
    gui/widgets/dialogs/tileset_dialog.hpp

    gui/widgets/layers/layer_widget.cpp
    gui/widgets/layers/layer_widget.hpp

    gui/widgets/menus/menu_bar_widget.cpp
    gui/widgets/menus/menu_bar_widget.hpp

    gui/widgets/properties/bool_property_widget.cpp
    gui/widgets/properties/bool_property_widget.hpp
    gui/widgets/properties/color_property_widget.cpp
    gui/widgets/properties/color_property_widget.hpp
    gui/widgets/properties/file_property_widget.hpp
    gui/widgets/properties/file_property_widget.cpp
    gui/widgets/properties/float_property_widget.cpp
    gui/widgets/properties/float_property_widget.hpp
    gui/widgets/properties/int_property_widget.cpp
    gui/widgets/properties/int_property_widget.hpp
    gui/widgets/properties/object_property_widget.cpp
    gui/widgets/properties/object_property_widget.hpp
    gui/widgets/properties/properties_widget.cpp
    gui/widgets/properties/properties_widget.hpp
    gui/widgets/properties/string_property_widget.cpp
    gui/widgets/properties/string_property_widget.hpp

    gui/widgets/tilesets/tileset_content_widget.cpp
    gui/widgets/tilesets/tileset_content_widget.hpp
    gui/widgets/tilesets/tileset_viewport_widget.cpp
    gui/widgets/tilesets/tileset_viewport_widget.hpp
    gui/widgets/tilesets/tileset_widget.cpp
    gui/widgets/tilesets/tileset_widget.hpp

    gui/widgets/toolbar/tool_bar.cpp
    gui/widgets/toolbar/tool_bar.hpp

    gui/widgets/viewport/map_content_viewport.cpp
    gui/widgets/viewport/map_content_viewport.hpp
    gui/widgets/viewport/no_content_viewport_widget.cpp
    gui/widgets/viewport/no_content_viewport_widget.hpp
    gui/widgets/viewport/render_map.cpp
    gui/widgets/viewport/render_map.hpp
    gui/widgets/viewport/viewport_widget.cpp
    gui/widgets/viewport/viewport_widget.hpp

    utils/buffer_utils.hpp
    utils/color_utils.hpp
    utils/load_texture.cpp
    utils/load_texture.hpp
    utils/scope_id.hpp
    utils/texture_info.hpp
    )

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})

add_library(libTactile ${TACTILE_SOURCE_FILES})

add_definitions(-DCENTURION_NO_SDL_TTF)
add_definitions(-DCENTURION_NO_SDL_MIXER)
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

if (WIN32)
  add_definitions(-DMSVC) # ImGuiFileDialog needs this to avoid unsafe C-functions
endif ()

target_include_directories(libTactile
    PUBLIC ${TACTILE_ROOT_DIR}/src
    SYSTEM PUBLIC ${LIBRARY_DIR}/glew
    SYSTEM PUBLIC ${LIBRARY_DIR}/imgui
    SYSTEM PUBLIC ${SDL2_INCLUDE_DIR}
    SYSTEM PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(libTactile
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC ${SDL2_IMAGE_LIBRARIES}
    PUBLIC libCenturion
    PUBLIC libNenya
    PUBLIC libJson
    PUBLIC libRune
    PUBLIC libEntt
    PUBLIC libFontAwesome
    PUBLIC libStbImage)

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