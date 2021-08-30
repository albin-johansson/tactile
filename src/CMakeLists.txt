cmake_minimum_required(VERSION 3.18)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    application.cpp
    application.hpp
    application_events.cpp
    application_events.hpp
    init_open_gl_attributes.cpp
    init_open_gl_attributes.hpp

    aliases/col.hpp
    aliases/czstring.hpp
    aliases/hash_map.hpp
    aliases/ints.hpp
    aliases/json.hpp
    aliases/layer_id.hpp
    aliases/layer_map.hpp
    aliases/layer_stack_resource.hpp
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
    core/layer_type.hpp
    core/map_position.hpp
    core/model.cpp
    core/model.hpp
    core/mouse_info.hpp
    core/object_type.hpp
    core/region.hpp
    core/tactile_error.hpp

    core/algorithms/invoke_n.hpp

    core/commands/command.cpp
    core/commands/command.hpp
    core/commands/command_id.hpp
    core/commands/command_stack.cpp
    core/commands/command_stack.hpp

    core/components/fancy_tile.hpp
    core/components/group_layer.hpp
    core/components/layer.hpp
    core/components/object.hpp
    core/components/object_layer.hpp
    core/components/parent.hpp
    core/components/property.hpp
    core/components/property_context.hpp
    core/components/texture.hpp
    core/components/tile_animation.hpp
    core/components/tile_layer.hpp
    core/components/tileset.hpp
    core/components/uv_tile_size.hpp

    core/properties/property_value.cpp
    core/properties/property_value.hpp
    core/properties/property_type.hpp

    core/systems/viewport_system.cpp
    core/systems/viewport_system.hpp

    core/tools/mouse_tool_type.hpp

    events/change_command_capacity_event.hpp
    events/quit_event.hpp
    events/redo_event.hpp
    events/save_as_event.hpp
    events/save_as_request_event.hpp
    events/save_event.hpp
    events/undo_event.hpp

    events/layers/add_layer_event.hpp
    events/layers/duplicate_layer_event.hpp
    events/layers/move_layer_down_event.hpp
    events/layers/move_layer_up_event.hpp
    events/layers/remove_layer_event.hpp
    events/layers/select_layer_event.hpp
    events/layers/set_layer_opacity_event.hpp
    events/layers/set_layer_visible_event.hpp
    events/layers/show_layer_properties_event.hpp

    events/maps/add_column_event.hpp
    events/maps/add_map_event.hpp
    events/maps/add_row_event.hpp
    events/maps/close_map_event.hpp
    events/maps/open_map_event.hpp
    events/maps/remove_column_event.hpp
    events/maps/remove_row_event.hpp
    events/maps/select_map_event.hpp
    events/maps/show_map_properties_event.hpp

    events/properties/add_property_event.hpp
    events/properties/change_property_type_event.hpp
    events/properties/remove_property_event.hpp
    events/properties/rename_property_event.hpp
    events/properties/set_property_value_event.hpp

    events/tilesets/add_tileset_event.hpp
    events/tilesets/remove_tileset_event.hpp
    events/tilesets/select_tileset_event.hpp
    events/tilesets/set_tileset_selection_event.hpp

    events/tools/mouse_drag_event.hpp
    events/tools/mouse_pressed_event.hpp
    events/tools/mouse_released_event.hpp
    events/tools/select_tool_event.hpp

    events/viewport/center_viewport_event.hpp
    events/viewport/decrease_viewport_zoom_event.hpp
    events/viewport/increase_viewport_zoom_event.hpp
    events/viewport/offset_viewport_event.hpp
    events/viewport/pan_down_event.hpp
    events/viewport/pan_left_event.hpp
    events/viewport/pan_right_event.hpp
    events/viewport/pan_up_event.hpp
    events/viewport/reset_viewport_zoom_event.hpp

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

    gui/rendering/canvas.hpp
    gui/rendering/canvas.cpp
    gui/rendering/common.cpp
    gui/rendering/common.hpp
    gui/rendering/grid.cpp
    gui/rendering/grid.hpp
    gui/rendering/render_bounds.cpp
    gui/rendering/render_bounds.hpp
    gui/rendering/render_info.cpp
    gui/rendering/render_info.hpp
    gui/rendering/render_map.cpp
    gui/rendering/render_map.hpp
    gui/rendering/render_object_layer.cpp
    gui/rendering/render_object_layer.hpp
    gui/rendering/render_tile.cpp
    gui/rendering/render_tile.hpp
    gui/rendering/render_tile_layer.cpp
    gui/rendering/render_tile_layer.hpp

    gui/rendering/objects/render_ellipse.cpp
    gui/rendering/objects/render_ellipse.hpp
    gui/rendering/objects/render_point.cpp
    gui/rendering/objects/render_point.hpp
    gui/rendering/objects/render_rect.cpp
    gui/rendering/objects/render_rect.hpp

    gui/widgets/alignment.cpp
    gui/widgets/alignment.hpp

    gui/widgets/common/button.cpp
    gui/widgets/common/button.hpp
    gui/widgets/common/centered_button.cpp
    gui/widgets/common/centered_button.hpp
    gui/widgets/common/centered_text.cpp
    gui/widgets/common/centered_text.hpp
    gui/widgets/common/checkbox.cpp
    gui/widgets/common/checkbox.hpp
    gui/widgets/common/combo.cpp
    gui/widgets/common/combo.hpp
    gui/widgets/common/docking_toolbar.cpp
    gui/widgets/common/docking_toolbar.hpp
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
    gui/widgets/dialogs/add_map_dialog.cpp
    gui/widgets/dialogs/add_map_dialog.hpp
    gui/widgets/dialogs/credits_dialog.cpp
    gui/widgets/dialogs/credits_dialog.hpp
    gui/widgets/dialogs/map_import_error_dialog.cpp
    gui/widgets/dialogs/map_import_error_dialog.hpp
    gui/widgets/dialogs/save_as_dialog.cpp
    gui/widgets/dialogs/save_as_dialog.hpp
    gui/widgets/dialogs/settings_dialog.cpp
    gui/widgets/dialogs/settings_dialog.hpp

    gui/widgets/layers/add_layer_popup.cpp
    gui/widgets/layers/add_layer_popup.hpp
    gui/widgets/layers/layer_dock.cpp
    gui/widgets/layers/layer_dock.hpp
    gui/widgets/layers/layer_item.cpp
    gui/widgets/layers/layer_item.hpp
    gui/widgets/layers/layer_item_popup.cpp
    gui/widgets/layers/layer_item_popup.hpp

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

    gui/widgets/viewport/map_content_overlay.cpp
    gui/widgets/viewport/map_content_overlay.hpp
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
    io/session.cpp
    io/session.hpp

    io/parsing/map_parser.cpp
    io/parsing/map_parser.hpp
    io/parsing/parse_error.cpp
    io/parsing/parse_error.hpp
    io/parsing/parse_ir.hpp
    io/parsing/to_map_document.cpp
    io/parsing/to_map_document.hpp

    io/parsing/json/json_utils.hpp
    io/parsing/json/parse_json_map.cpp
    io/parsing/json/parse_json_map.hpp
    io/parsing/json/parse_layers.cpp
    io/parsing/json/parse_layers.hpp
    io/parsing/json/parse_object_layer.cpp
    io/parsing/json/parse_object_layer.hpp
    io/parsing/json/parse_properties.cpp
    io/parsing/json/parse_properties.hpp
    io/parsing/json/parse_tile_data.cpp
    io/parsing/json/parse_tile_data.hpp
    io/parsing/json/parse_tile_layer.cpp
    io/parsing/json/parse_tile_layer.hpp
    io/parsing/json/parse_tilesets.cpp
    io/parsing/json/parse_tilesets.hpp
    io/parsing/json/read_json.cpp
    io/parsing/json/read_json.hpp

    io/parsing/xml/parse_csv.cpp
    io/parsing/xml/parse_csv.hpp
    io/parsing/xml/parse_layers.cpp
    io/parsing/xml/parse_layers.hpp
    io/parsing/xml/parse_object_layer.cpp
    io/parsing/xml/parse_object_layer.hpp
    io/parsing/xml/parse_properties.cpp
    io/parsing/xml/parse_properties.hpp
    io/parsing/xml/parse_tile_data.cpp
    io/parsing/xml/parse_tile_data.hpp
    io/parsing/xml/parse_tile_layer.cpp
    io/parsing/xml/parse_tile_layer.hpp
    io/parsing/xml/parse_tile_nodes.cpp
    io/parsing/xml/parse_tile_nodes.hpp
    io/parsing/xml/parse_tilesets.cpp
    io/parsing/xml/parse_tilesets.hpp
    io/parsing/xml/parse_xml_map.cpp
    io/parsing/xml/parse_xml_map.hpp
    io/parsing/xml/xml_utils.cpp
    io/parsing/xml/xml_utils.hpp

    io/saving/common.cpp
    io/saving/common.hpp
    io/saving/save_map_document.cpp
    io/saving/save_map_document.hpp

    io/saving/json/save_json.cpp
    io/saving/json/save_json.hpp
    io/saving/json/save_layers.cpp
    io/saving/json/save_layers.hpp
    io/saving/json/save_map_document_as_json.cpp
    io/saving/json/save_map_document_as_json.hpp
    io/saving/json/save_properties.cpp
    io/saving/json/save_properties.hpp
    io/saving/json/save_tilesets.cpp
    io/saving/json/save_tilesets.hpp

    io/saving/xml/append_layer.cpp
    io/saving/xml/append_layer.hpp
    io/saving/xml/append_properties.cpp
    io/saving/xml/append_properties.hpp
    io/saving/xml/append_tileset.cpp
    io/saving/xml/append_tileset.hpp
    io/saving/xml/save_map_document_as_xml.cpp
    io/saving/xml/save_map_document_as_xml.hpp
    io/saving/xml/save_xml.cpp
    io/saving/xml/save_xml.hpp

    utils/buffer_utils.hpp
    utils/color_utils.hpp
    utils/from_string.hpp
    utils/load_texture.cpp
    utils/load_texture.hpp
    utils/profile.hpp
    utils/scope_id.hpp
    utils/texture_info.hpp

    shortcuts/shortcut.cpp
    shortcuts/shortcut.hpp
    shortcuts/shortcuts.cpp
    shortcuts/shortcuts.hpp

    shortcuts/edit/add_column_shortcut.cpp
    shortcuts/edit/add_column_shortcut.hpp
    shortcuts/edit/add_row_shortcut.cpp
    shortcuts/edit/add_row_shortcut.hpp
    shortcuts/edit/add_tileset_shortcut.cpp
    shortcuts/edit/add_tileset_shortcut.hpp
    shortcuts/edit/enable_bucket_tool.cpp
    shortcuts/edit/enable_bucket_tool.hpp
    shortcuts/edit/enable_eraser_shortcut.cpp
    shortcuts/edit/enable_eraser_shortcut.hpp
    shortcuts/edit/enable_stamp_shortcut.cpp
    shortcuts/edit/enable_stamp_shortcut.hpp
    shortcuts/edit/redo_shortcut.cpp
    shortcuts/edit/redo_shortcut.hpp
    shortcuts/edit/remove_column_shortcut.cpp
    shortcuts/edit/remove_column_shortcut.hpp
    shortcuts/edit/remove_row_shortcut.cpp
    shortcuts/edit/remove_row_shortcut.hpp
    shortcuts/edit/undo_shortcut.cpp
    shortcuts/edit/undo_shortcut.hpp

    shortcuts/file/new_map_shortcut.cpp
    shortcuts/file/new_map_shortcut.hpp
    shortcuts/file/open_map_shortcut.cpp
    shortcuts/file/open_map_shortcut.hpp
    shortcuts/file/open_settings_shortcut.cpp
    shortcuts/file/open_settings_shortcut.hpp
    shortcuts/file/save_as_shortcut.cpp
    shortcuts/file/save_as_shortcut.hpp
    shortcuts/file/save_shortcut.cpp
    shortcuts/file/save_shortcut.hpp

    shortcuts/view/center_viewport_shortcut.cpp
    shortcuts/view/center_viewport_shortcut.hpp
    shortcuts/view/decrease_viewport_zoom_shortcut.cpp
    shortcuts/view/decrease_viewport_zoom_shortcut.hpp
    shortcuts/view/increase_viewport_zoom_shortcut.cpp
    shortcuts/view/increase_viewport_zoom_shortcut.hpp
    shortcuts/view/pan_down_shortcut.cpp
    shortcuts/view/pan_down_shortcut.hpp
    shortcuts/view/pan_left_shortcut.cpp
    shortcuts/view/pan_left_shortcut.hpp
    shortcuts/view/pan_right_shortcut.cpp
    shortcuts/view/pan_right_shortcut.hpp
    shortcuts/view/pan_up_shortcut.cpp
    shortcuts/view/pan_up_shortcut.hpp
    shortcuts/view/toggle_grid_shortcut.cpp
    shortcuts/view/toggle_grid_shortcut.hpp
    )

list(APPEND TACTILE_SOURCE_FILES ${IMGUI_SOURCES})
list(APPEND TACTILE_SOURCE_FILES ${PUGIXML_SOURCES})

add_library(libtactile ${TACTILE_SOURCE_FILES})

target_compile_definitions(libtactile
    PUBLIC CENTURION_NO_SDL_MIXER
    PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # ImGuiFileDialog needs this to avoid unsafe C-functions
  target_compile_definitions(libtactile PUBLIC MSVC)
endif ()

target_include_directories(libtactile
    PUBLIC ${TACTILE_ROOT_DIR}/src
    SYSTEM PUBLIC ${LIBRARY_DIR}/glew
    SYSTEM PUBLIC ${LIBRARY_DIR}/imgui
    SYSTEM PUBLIC ${LIBRARY_DIR}/pugixml
    SYSTEM PUBLIC ${LIBRARY_DIR}/rune
    SYSTEM PUBLIC ${LIBRARY_DIR}/init
    SYSTEM PUBLIC ${SDL2_INCLUDE_DIR}
    SYSTEM PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS}
    SYSTEM PUBLIC ${SDL2_TTF_INCLUDE_DIRS}
    )

target_link_libraries(libtactile
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${SDL2_IMAGE_LIBRARIES}
    PUBLIC ${SDL2_TTF_LIBRARIES}
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC libCenturion
    PUBLIC libNenya
    PUBLIC libJson
    PUBLIC libEntt
    PUBLIC libFontAwesome
    PUBLIC libStbImage)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(libtactile PRIVATE
      /EHsc
      /MP
      /W3
      /Zc:preprocessor
      /Zc:__cplusplus
      /wd5105  # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
      )
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

add_dependencies(Tactile libtactile)

target_link_libraries(Tactile PUBLIC libtactile)

if (WIN32)
  copy_directory_post_build(Tactile
      ${TACTILE_ROOT_DIR}/bin
      ${CMAKE_CURRENT_BINARY_DIR})
endif ()

copy_directory_post_build(Tactile
    ${RESOURCES_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/resources)