cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    application.cpp
    application.hpp
    application_events.cpp
    application_events.hpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp
    shortcuts.cpp
    shortcuts.hpp

    aliases/col.hpp
    aliases/czstring.hpp
    aliases/ints.hpp
    aliases/layer_id.hpp
    aliases/map_id.hpp
    aliases/maybe.hpp
    aliases/not_null.hpp
    aliases/object_id.hpp
    aliases/object_ref.hpp
    aliases/row.hpp
    aliases/shared.hpp
    aliases/tile_id.hpp
    aliases/tile_matrix.hpp
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
    core/viewport_info.hpp

    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp
    core/algorithms/invoke_n.hpp

    core/commands/command.hpp
    core/commands/command.cpp
    core/commands/command_id.hpp
    core/commands/command_stack.cpp
    core/commands/command_stack.hpp

    core/commands/maps/add_column_cmd.cpp
    core/commands/maps/add_column_cmd.hpp
    core/commands/maps/add_row_cmd.cpp
    core/commands/maps/add_row_cmd.hpp
    core/commands/maps/map_command.cpp
    core/commands/maps/map_command.hpp
    core/commands/maps/mergeable_map_command.cpp
    core/commands/maps/mergeable_map_command.hpp
    core/commands/maps/remove_column_cmd.cpp
    core/commands/maps/remove_column_cmd.hpp
    core/commands/maps/remove_row_cmd.cpp
    core/commands/maps/remove_row_cmd.hpp

    core/commands/maps/layers/add_layer_cmd.cpp
    core/commands/maps/layers/add_layer_cmd.hpp
    core/commands/maps/layers/duplicate_layer_cmd.cpp
    core/commands/maps/layers/duplicate_layer_cmd.hpp
    core/commands/maps/layers/move_layer_down_cmd.cpp
    core/commands/maps/layers/move_layer_down_cmd.hpp
    core/commands/maps/layers/move_layer_up_cmd.cpp
    core/commands/maps/layers/move_layer_up_cmd.hpp
    core/commands/maps/layers/remove_layer_cmd.cpp
    core/commands/maps/layers/remove_layer_cmd.hpp
    core/commands/maps/layers/select_layer_cmd.cpp
    core/commands/maps/layers/select_layer_cmd.hpp
    core/commands/maps/layers/set_layer_opacity_cmd.cpp
    core/commands/maps/layers/set_layer_opacity_cmd.hpp
    core/commands/maps/layers/set_layer_visible_cmd.cpp
    core/commands/maps/layers/set_layer_visible_cmd.hpp

    core/commands/properties/add_property_cmd.cpp
    core/commands/properties/add_property_cmd.hpp
    core/commands/properties/change_property_type_cmd.cpp
    core/commands/properties/change_property_type_cmd.hpp
    core/commands/properties/property_command.cpp
    core/commands/properties/property_command.hpp
    core/commands/properties/remove_property_cmd.cpp
    core/commands/properties/remove_property_cmd.hpp
    core/commands/properties/rename_property_cmd.cpp
    core/commands/properties/rename_property_cmd.hpp
    core/commands/properties/set_property_cmd.cpp
    core/commands/properties/set_property_cmd.hpp
    core/commands/properties/set_property_context_cmd.cpp
    core/commands/properties/set_property_context_cmd.hpp

    core/commands/tilesets/add_tileset_cmd.cpp
    core/commands/tilesets/add_tileset_cmd.hpp
    core/commands/tilesets/remove_tileset_cmd.cpp
    core/commands/tilesets/remove_tileset_cmd.hpp

    core/commands/tools/bucket_cmd.cpp
    core/commands/tools/bucket_cmd.hpp
    core/commands/tools/eraser_sequence_cmd.cpp
    core/commands/tools/eraser_sequence_cmd.hpp
    core/commands/tools/stamp_sequence_cmd.cpp
    core/commands/tools/stamp_sequence_cmd.hpp

    core/events/quit_event.hpp
    core/events/redo_event.hpp
    core/events/undo_event.hpp

    core/events/layers/add_layer_event.hpp
    core/events/layers/duplicate_layer_event.hpp
    core/events/layers/move_layer_down_event.hpp
    core/events/layers/move_layer_up_event.hpp
    core/events/layers/remove_layer_event.hpp
    core/events/layers/select_layer_event.hpp
    core/events/layers/set_layer_opacity_event.hpp
    core/events/layers/set_layer_visible_event.hpp
    core/events/layers/show_layer_properties_event.hpp

    core/events/maps/add_column_event.hpp
    core/events/maps/add_map_event.hpp
    core/events/maps/add_row_event.hpp
    core/events/maps/close_map_event.hpp
    core/events/maps/open_map_event.hpp
    core/events/maps/remove_column_event.hpp
    core/events/maps/remove_row_event.hpp
    core/events/maps/select_map_event.hpp
    core/events/maps/show_map_properties_event.hpp

    core/events/properties/add_property_event.hpp
    core/events/properties/change_property_type_event.hpp
    core/events/properties/remove_property_event.hpp
    core/events/properties/rename_property_event.hpp
    core/events/properties/set_property_value_event.hpp

    core/events/tilesets/add_tileset_event.hpp
    core/events/tilesets/remove_tileset_event.hpp
    core/events/tilesets/select_tileset_event.hpp
    core/events/tilesets/set_tileset_selection_event.hpp

    core/events/tools/mouse_drag_event.hpp
    core/events/tools/mouse_pressed_event.hpp
    core/events/tools/mouse_released_event.hpp
    core/events/tools/select_tool_event.hpp

    core/events/viewport/center_viewport_event.hpp
    core/events/viewport/decrease_viewport_zoom_event.hpp
    core/events/viewport/increase_viewport_zoom_event.hpp
    core/events/viewport/offset_viewport_event.hpp
    core/events/viewport/reset_viewport_zoom_event.hpp

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

    core/tools/bucket_tool.cpp
    core/tools/bucket_tool.hpp
    core/tools/eraser_tool.cpp
    core/tools/eraser_tool.hpp
    core/tools/mouse_info.hpp
    core/tools/mouse_tool.cpp
    core/tools/mouse_tool.hpp
    core/tools/mouse_tool_model.cpp
    core/tools/mouse_tool_model.hpp
    core/tools/mouse_tool_type.hpp
    core/tools/stamp_tool.cpp
    core/tools/stamp_tool.hpp

    gui/cursors.cpp
    gui/cursors.hpp
    gui/icons.cpp
    gui/icons.hpp
    gui/imgui_context.cpp
    gui/imgui_context.hpp
    gui/style.cpp
    gui/style.hpp
    gui/texture_utils.cpp
    gui/texture_utils.hpp
    gui/themes.cpp
    gui/themes.hpp
    gui/update_gui.cpp
    gui/update_gui.hpp

    gui/layout/dock_space.cpp
    gui/layout/dock_space.hpp
    gui/layout/load_default_layout.cpp
    gui/layout/load_default_layout.hpp

    gui/widgets/alignment.cpp
    gui/widgets/alignment.hpp

    gui/widgets/common/button_ex.cpp
    gui/widgets/common/button_ex.hpp
    gui/widgets/common/centered_button.cpp
    gui/widgets/common/centered_button.hpp
    gui/widgets/common/centered_text.cpp
    gui/widgets/common/centered_text.hpp
    gui/widgets/common/file_dialog.cpp
    gui/widgets/common/file_dialog.hpp
    gui/widgets/common/help_marker.cpp
    gui/widgets/common/help_marker.hpp
    gui/widgets/common/mouse_tracker.cpp
    gui/widgets/common/mouse_tracker.hpp
    gui/widgets/common/rubber_band.cpp
    gui/widgets/common/rubber_band.hpp

    gui/widgets/dialogs/about_dialog.cpp
    gui/widgets/dialogs/about_dialog.hpp
    gui/widgets/dialogs/credits_dialog.cpp
    gui/widgets/dialogs/credits_dialog.hpp
    gui/widgets/dialogs/map_import_error_dialog.cpp
    gui/widgets/dialogs/map_import_error_dialog.hpp
    gui/widgets/dialogs/settings_dialog.cpp
    gui/widgets/dialogs/settings_dialog.hpp

    gui/widgets/layers/layer_dock.cpp
    gui/widgets/layers/layer_dock.hpp

    gui/widgets/menus/edit_menu.cpp
    gui/widgets/menus/edit_menu.hpp
    gui/widgets/menus/file_menu.cpp
    gui/widgets/menus/file_menu.hpp
    gui/widgets/menus/help_menu.cpp
    gui/widgets/menus/help_menu.hpp
    gui/widgets/menus/menu_bar.cpp
    gui/widgets/menus/menu_bar.hpp
    gui/widgets/menus/view_menu.cpp
    gui/widgets/menus/view_menu.hpp

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
    gui/widgets/properties/properties_content_widget.cpp
    gui/widgets/properties/properties_content_widget.hpp
    gui/widgets/properties/properties_dock.cpp
    gui/widgets/properties/properties_dock.hpp
    gui/widgets/properties/string_property_widget.cpp
    gui/widgets/properties/string_property_widget.hpp

    gui/widgets/properties/dialogs/add_property_dialog.cpp
    gui/widgets/properties/dialogs/add_property_dialog.hpp
    gui/widgets/properties/dialogs/change_property_type_dialog.cpp
    gui/widgets/properties/dialogs/change_property_type_dialog.hpp
    gui/widgets/properties/dialogs/property_type_combo.cpp
    gui/widgets/properties/dialogs/property_type_combo.hpp
    gui/widgets/properties/dialogs/rename_property_dialog.cpp
    gui/widgets/properties/dialogs/rename_property_dialog.hpp

    gui/widgets/rendering/canvas.hpp
    gui/widgets/rendering/canvas.cpp
    gui/widgets/rendering/render_bounds.cpp
    gui/widgets/rendering/render_bounds.hpp
    gui/widgets/rendering/render_map.cpp
    gui/widgets/rendering/render_map.hpp
    gui/widgets/rendering/render_tile.cpp
    gui/widgets/rendering/render_tile.hpp
    gui/widgets/rendering/render_tile_layer.cpp
    gui/widgets/rendering/render_tile_layer.hpp
    gui/widgets/rendering/grid.cpp
    gui/widgets/rendering/grid.hpp
    gui/widgets/rendering/uv_tile_size.cpp
    gui/widgets/rendering/uv_tile_size.hpp

    gui/widgets/tilesets/tileset_content_widget.cpp
    gui/widgets/tilesets/tileset_content_widget.hpp
    gui/widgets/tilesets/tileset_dock.cpp
    gui/widgets/tilesets/tileset_dock.hpp
    gui/widgets/tilesets/tileset_viewport_widget.cpp
    gui/widgets/tilesets/tileset_viewport_widget.hpp

    gui/widgets/tilesets/dialogs/tileset_dialog.cpp
    gui/widgets/tilesets/dialogs/tileset_dialog.hpp

    gui/widgets/toolbar/tool_bar.cpp
    gui/widgets/toolbar/tool_bar.hpp
    gui/widgets/toolbar/tool_button.cpp
    gui/widgets/toolbar/tool_button.hpp

    gui/widgets/viewport/map_content_viewport.cpp
    gui/widgets/viewport/map_content_viewport.hpp
    gui/widgets/viewport/map_view.cpp
    gui/widgets/viewport/map_view.hpp
    gui/widgets/viewport/no_content_viewport_widget.cpp
    gui/widgets/viewport/no_content_viewport_widget.hpp
    gui/widgets/viewport/render_stamp_preview.cpp
    gui/widgets/viewport/render_stamp_preview.hpp
    gui/widgets/viewport/viewport_cursor_info.cpp
    gui/widgets/viewport/viewport_cursor_info.hpp
    gui/widgets/viewport/viewport_widget.cpp
    gui/widgets/viewport/viewport_widget.hpp

    io/directories.cpp
    io/directories.hpp
    io/preferences.cpp
    io/preferences.hpp

    io/parsing/map_attribute.hpp
    io/parsing/map_file.hpp
    io/parsing/map_object.hpp
    io/parsing/map_parser.cpp
    io/parsing/map_parser.hpp
    io/parsing/parse_error.cpp
    io/parsing/parse_error.hpp
    io/parsing/parse_ir.hpp
    io/parsing/parse_layers.cpp
    io/parsing/parse_layers.hpp
    io/parsing/parse_properties.cpp
    io/parsing/parse_properties.hpp
    io/parsing/parse_tilesets.cpp
    io/parsing/parse_tilesets.hpp
    io/parsing/to_map_document.cpp
    io/parsing/to_map_document.hpp

    io/parsing/json/json_map_file.cpp
    io/parsing/json/json_map_file.hpp
    io/parsing/json/json_map_object.cpp
    io/parsing/json/json_map_object.hpp

    io/parsing/xml/parse_csv.cpp
    io/parsing/xml/parse_csv.hpp
    io/parsing/xml/xml_map_file.cpp
    io/parsing/xml/xml_map_file.hpp
    io/parsing/xml/xml_map_object.cpp
    io/parsing/xml/xml_map_object.hpp

    utils/buffer_utils.hpp
    utils/color_utils.cpp
    utils/color_utils.hpp
    utils/from_string.hpp
    utils/load_texture.cpp
    utils/load_texture.hpp
    utils/scope_id.hpp
    utils/texture_info.hpp)

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})
list(APPEND TACTILE_SOURCE_FILES ${PUGIXML_SOURCES})

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
    SYSTEM PUBLIC ${LIBRARY_DIR}/pugixml
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