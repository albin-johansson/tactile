cmake_minimum_required(VERSION 3.18)

project(tactile-src CXX)

add_subdirectory(parsers)

set(SOURCE_FILES
    ${IMGUI_SOURCES}
    ${PUGIXML_SOURCES}

    application.cpp
    application.hpp
    application_events.cpp
    application_events.hpp
    build.hpp
    common.hpp

    cfg/gl_attributes.cpp
    cfg/gl_attributes.hpp

    common/context_id.hpp
    common/cstr.hpp
    common/expected.hpp
    common/hash_map.hpp
    common/ints.hpp
    io/json.hpp
    common/layer_id.hpp
    common/map_id.hpp
    common/maybe.hpp
    common/not_null.hpp
    common/object_id.hpp
    common/object_ref.hpp
    common/ref.hpp
    common/tile_cache.hpp
    common/tile_id.hpp
    common/tile_matrix.hpp
    common/tileset_id.hpp

    core/layer_type.hpp
    core/map.hpp
    core/map_position.hpp
    core/mouse.hpp
    core/mouse_info.hpp
    core/object_type.hpp
    core/property_type.hpp
    core/property_value.cpp
    core/property_value.hpp
    core/region.hpp
    core/tactile_error.hpp
    core/tool_type.hpp
    core/viewport.hpp

    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp
    core/algorithms/invoke_n.hpp

    core/components/animation.hpp
    core/components/fancy_tile.hpp
    core/components/group_layer.hpp
    core/components/layer.hpp
    core/components/layer_tree_node.hpp
    core/components/object.hpp
    core/components/object_layer.hpp
    core/components/parent.hpp
    core/components/property.hpp
    core/components/property_context.hpp
    core/components/texture.hpp
    core/components/tile_layer.hpp
    core/components/tileset.hpp
    core/components/tool.hpp
    core/components/uv_tile_size.hpp

    core/systems/animation_system.cpp
    core/systems/animation_system.hpp
    core/systems/duplicate_comp.hpp
    core/systems/map_system.cpp
    core/systems/map_system.hpp
    core/systems/property_context_snapshot.hpp
    core/systems/property_system.cpp
    core/systems/property_system.hpp
    core/systems/registry_factory_system.cpp
    core/systems/registry_factory_system.hpp
    core/systems/tileset_snapshot.hpp
    core/systems/tileset_system.cpp
    core/systems/tileset_system.hpp
    core/systems/viewport_system.cpp
    core/systems/viewport_system.hpp

    core/systems/layers/layer_snapshot.hpp
    core/systems/layers/layer_system.cpp
    core/systems/layers/layer_system.hpp
    core/systems/layers/layer_tree_system.cpp
    core/systems/layers/layer_tree_system.hpp
    core/systems/layers/object_layer_system.cpp
    core/systems/layers/object_layer_system.hpp
    core/systems/layers/object_snapshot.hpp
    core/systems/layers/tile_layer_system.cpp
    core/systems/layers/tile_layer_system.hpp

    core/systems/tools/bucket_tool_system.cpp
    core/systems/tools/bucket_tool_system.hpp
    core/systems/tools/eraser_tool_system.cpp
    core/systems/tools/eraser_tool_system.hpp
    core/systems/tools/stamp_tool_system.cpp
    core/systems/tools/stamp_tool_system.hpp
    core/systems/tools/tool_system.cpp
    core/systems/tools/tool_system.hpp

    core/utils/buffer_utils.hpp
    core/utils/color_utils.hpp
    core/utils/from_string.hpp
    core/utils/load_texture.cpp
    core/utils/load_texture.hpp
    core/utils/profile.hpp
    core/utils/scope_id.hpp
    core/utils/string_utils.hpp
    core/utils/string_utils.cpp

    editor/document.hpp
    editor/model.cpp
    editor/model.hpp

    editor/commands/command.cpp
    editor/commands/command.hpp
    editor/commands/command_id.hpp
    editor/commands/command_stack.cpp
    editor/commands/command_stack.hpp

    editor/commands/layers/add_layer_cmd.cpp
    editor/commands/layers/add_layer_cmd.hpp
    editor/commands/layers/duplicate_layer_cmd.cpp
    editor/commands/layers/duplicate_layer_cmd.hpp
    editor/commands/layers/move_layer_down_cmd.cpp
    editor/commands/layers/move_layer_down_cmd.hpp
    editor/commands/layers/move_layer_up_cmd.cpp
    editor/commands/layers/move_layer_up_cmd.hpp
    editor/commands/layers/remove_layer_cmd.cpp
    editor/commands/layers/remove_layer_cmd.hpp
    editor/commands/layers/rename_layer_cmd.cpp
    editor/commands/layers/rename_layer_cmd.hpp
    editor/commands/layers/set_layer_opacity_cmd.cpp
    editor/commands/layers/set_layer_opacity_cmd.hpp
    editor/commands/layers/set_layer_visibility_cmd.cpp
    editor/commands/layers/set_layer_visibility_cmd.hpp

    editor/commands/maps/add_column_cmd.cpp
    editor/commands/maps/add_column_cmd.hpp
    editor/commands/maps/add_row_cmd.cpp
    editor/commands/maps/add_row_cmd.hpp
    editor/commands/maps/map_command_cache.cpp
    editor/commands/maps/map_command_cache.hpp
    editor/commands/maps/remove_column_cmd.cpp
    editor/commands/maps/remove_column_cmd.hpp
    editor/commands/maps/remove_row_cmd.cpp
    editor/commands/maps/remove_row_cmd.hpp
    editor/commands/maps/resize_map_cmd.cpp
    editor/commands/maps/resize_map_cmd.hpp

    editor/commands/properties/add_property_cmd.cpp
    editor/commands/properties/add_property_cmd.hpp
    editor/commands/properties/change_property_type_cmd.cpp
    editor/commands/properties/change_property_type_cmd.hpp
    editor/commands/properties/remove_property_cmd.cpp
    editor/commands/properties/remove_property_cmd.hpp
    editor/commands/properties/rename_property_cmd.cpp
    editor/commands/properties/rename_property_cmd.hpp
    editor/commands/properties/update_property_cmd.cpp
    editor/commands/properties/update_property_cmd.hpp

    editor/commands/tilesets/add_tileset_cmd.cpp
    editor/commands/tilesets/add_tileset_cmd.hpp
    editor/commands/tilesets/remove_tileset_cmd.cpp
    editor/commands/tilesets/remove_tileset_cmd.hpp

    editor/commands/tools/bucket_cmd.cpp
    editor/commands/tools/bucket_cmd.hpp
    editor/commands/tools/eraser_sequence_cmd.cpp
    editor/commands/tools/eraser_sequence_cmd.hpp
    editor/commands/tools/stamp_sequence_cmd.cpp
    editor/commands/tools/stamp_sequence_cmd.hpp

    editor/events/command_events.hpp
    editor/events/layer_events.hpp
    editor/events/map_events.hpp
    editor/events/property_events.hpp
    editor/events/quit_event.hpp
    editor/events/save_events.hpp
    editor/events/tileset_events.hpp
    editor/events/tool_events.hpp
    editor/events/view_events.hpp
    editor/events/viewport_events.hpp

    editor/shortcuts/shortcut.cpp
    editor/shortcuts/shortcut.hpp
    editor/shortcuts/shortcuts.cpp
    editor/shortcuts/shortcuts.hpp

    editor/shortcuts/edit/add_column_shortcut.cpp
    editor/shortcuts/edit/add_column_shortcut.hpp
    editor/shortcuts/edit/add_row_shortcut.cpp
    editor/shortcuts/edit/add_row_shortcut.hpp
    editor/shortcuts/edit/add_tileset_shortcut.cpp
    editor/shortcuts/edit/add_tileset_shortcut.hpp
    editor/shortcuts/edit/enable_bucket_tool.cpp
    editor/shortcuts/edit/enable_bucket_tool.hpp
    editor/shortcuts/edit/enable_eraser_shortcut.cpp
    editor/shortcuts/edit/enable_eraser_shortcut.hpp
    editor/shortcuts/edit/enable_stamp_shortcut.cpp
    editor/shortcuts/edit/enable_stamp_shortcut.hpp
    editor/shortcuts/edit/redo_shortcut.cpp
    editor/shortcuts/edit/redo_shortcut.hpp
    editor/shortcuts/edit/remove_column_shortcut.cpp
    editor/shortcuts/edit/remove_column_shortcut.hpp
    editor/shortcuts/edit/remove_row_shortcut.cpp
    editor/shortcuts/edit/remove_row_shortcut.hpp
    editor/shortcuts/edit/undo_shortcut.cpp
    editor/shortcuts/edit/undo_shortcut.hpp

    editor/shortcuts/file/new_map_shortcut.cpp
    editor/shortcuts/file/new_map_shortcut.hpp
    editor/shortcuts/file/open_map_shortcut.cpp
    editor/shortcuts/file/open_map_shortcut.hpp
    editor/shortcuts/file/open_settings_shortcut.cpp
    editor/shortcuts/file/open_settings_shortcut.hpp
    editor/shortcuts/file/save_as_shortcut.cpp
    editor/shortcuts/file/save_as_shortcut.hpp
    editor/shortcuts/file/save_shortcut.cpp
    editor/shortcuts/file/save_shortcut.hpp

    editor/shortcuts/view/center_viewport_shortcut.cpp
    editor/shortcuts/view/center_viewport_shortcut.hpp
    editor/shortcuts/view/decrease_viewport_zoom_shortcut.cpp
    editor/shortcuts/view/decrease_viewport_zoom_shortcut.hpp
    editor/shortcuts/view/increase_viewport_zoom_shortcut.cpp
    editor/shortcuts/view/increase_viewport_zoom_shortcut.hpp
    editor/shortcuts/view/pan_down_shortcut.cpp
    editor/shortcuts/view/pan_down_shortcut.hpp
    editor/shortcuts/view/pan_left_shortcut.cpp
    editor/shortcuts/view/pan_left_shortcut.hpp
    editor/shortcuts/view/pan_right_shortcut.cpp
    editor/shortcuts/view/pan_right_shortcut.hpp
    editor/shortcuts/view/pan_up_shortcut.cpp
    editor/shortcuts/view/pan_up_shortcut.hpp
    editor/shortcuts/view/toggle_grid_shortcut.cpp
    editor/shortcuts/view/toggle_grid_shortcut.hpp
    editor/shortcuts/view/toggle_ui_shortcut.cpp
    editor/shortcuts/view/toggle_ui_shortcut.hpp

    editor/gui/icons.cpp
    editor/gui/icons.hpp
    editor/gui/imgui_context.cpp
    editor/gui/imgui_context.hpp
    editor/gui/style.cpp
    editor/gui/style.hpp
    editor/gui/texture_utils.cpp
    editor/gui/texture_utils.hpp
    editor/gui/themes.cpp
    editor/gui/themes.hpp
    editor/gui/update_gui.cpp
    editor/gui/update_gui.hpp

    editor/gui/layout/dock_space.cpp
    editor/gui/layout/dock_space.hpp
    editor/gui/layout/load_default_layout.cpp
    editor/gui/layout/load_default_layout.hpp

    editor/gui/rendering/canvas.hpp
    editor/gui/rendering/canvas.cpp
    editor/gui/rendering/common_rendering.cpp
    editor/gui/rendering/common_rendering.hpp
    editor/gui/rendering/grid.cpp
    editor/gui/rendering/grid.hpp
    editor/gui/rendering/render_bounds.cpp
    editor/gui/rendering/render_bounds.hpp
    editor/gui/rendering/render_info.cpp
    editor/gui/rendering/render_info.hpp
    editor/gui/rendering/render_map.cpp
    editor/gui/rendering/render_map.hpp
    editor/gui/rendering/render_object_layer.cpp
    editor/gui/rendering/render_object_layer.hpp
    editor/gui/rendering/render_stamp_preview.cpp
    editor/gui/rendering/render_stamp_preview.hpp
    editor/gui/rendering/render_tile.cpp
    editor/gui/rendering/render_tile.hpp
    editor/gui/rendering/render_tile_layer.cpp
    editor/gui/rendering/render_tile_layer.hpp

    editor/gui/rendering/objects/render_ellipse.cpp
    editor/gui/rendering/objects/render_ellipse.hpp
    editor/gui/rendering/objects/render_point.cpp
    editor/gui/rendering/objects/render_point.hpp
    editor/gui/rendering/objects/render_rect.cpp
    editor/gui/rendering/objects/render_rect.hpp

    editor/gui/widgets/alignment.cpp
    editor/gui/widgets/alignment.hpp
    editor/gui/widgets/focus.cpp
    editor/gui/widgets/focus.hpp

    editor/gui/widgets/common/button.cpp
    editor/gui/widgets/common/button.hpp
    editor/gui/widgets/common/centered_button.cpp
    editor/gui/widgets/common/centered_button.hpp
    editor/gui/widgets/common/centered_text.cpp
    editor/gui/widgets/common/centered_text.hpp
    editor/gui/widgets/common/checkbox.cpp
    editor/gui/widgets/common/checkbox.hpp
    editor/gui/widgets/common/combo.cpp
    editor/gui/widgets/common/combo.hpp
    editor/gui/widgets/common/docking_toolbar.cpp
    editor/gui/widgets/common/docking_toolbar.hpp
    editor/gui/widgets/common/help_marker.cpp
    editor/gui/widgets/common/help_marker.hpp
    editor/gui/widgets/common/menu.hpp
    editor/gui/widgets/common/modal.hpp
    editor/gui/widgets/common/mouse_tracker.cpp
    editor/gui/widgets/common/mouse_tracker.hpp
    editor/gui/widgets/common/rubber_band.cpp
    editor/gui/widgets/common/rubber_band.hpp
    editor/gui/widgets/common/window.hpp

    editor/gui/widgets/dialogs/about_dialog.cpp
    editor/gui/widgets/dialogs/about_dialog.hpp
    editor/gui/widgets/dialogs/add_map_dialog.cpp
    editor/gui/widgets/dialogs/add_map_dialog.hpp
    editor/gui/widgets/dialogs/credits_dialog.cpp
    editor/gui/widgets/dialogs/credits_dialog.hpp
    editor/gui/widgets/dialogs/map_import_error_dialog.cpp
    editor/gui/widgets/dialogs/map_import_error_dialog.hpp
    editor/gui/widgets/dialogs/rename_dialog.cpp
    editor/gui/widgets/dialogs/rename_dialog.hpp
    editor/gui/widgets/dialogs/resize_map_dialog.cpp
    editor/gui/widgets/dialogs/resize_map_dialog.hpp
    editor/gui/widgets/dialogs/save_as_dialog.cpp
    editor/gui/widgets/dialogs/save_as_dialog.hpp
    editor/gui/widgets/dialogs/settings_dialog.cpp
    editor/gui/widgets/dialogs/settings_dialog.hpp

    editor/gui/widgets/layers/add_layer_popup.cpp
    editor/gui/widgets/layers/add_layer_popup.hpp
    editor/gui/widgets/layers/layer_dock.cpp
    editor/gui/widgets/layers/layer_dock.hpp
    editor/gui/widgets/layers/layer_item.cpp
    editor/gui/widgets/layers/layer_item.hpp
    editor/gui/widgets/layers/layer_item_popup.cpp
    editor/gui/widgets/layers/layer_item_popup.hpp
    editor/gui/widgets/layers/rename_layer_dialog.cpp
    editor/gui/widgets/layers/rename_layer_dialog.hpp

    editor/gui/widgets/menus/debug_menu.cpp
    editor/gui/widgets/menus/debug_menu.hpp
    editor/gui/widgets/menus/edit_menu.cpp
    editor/gui/widgets/menus/edit_menu.hpp
    editor/gui/widgets/menus/file_menu.cpp
    editor/gui/widgets/menus/file_menu.hpp
    editor/gui/widgets/menus/help_menu.cpp
    editor/gui/widgets/menus/help_menu.hpp
    editor/gui/widgets/menus/map_menu.cpp
    editor/gui/widgets/menus/map_menu.hpp
    editor/gui/widgets/menus/menu_bar.cpp
    editor/gui/widgets/menus/menu_bar.hpp
    editor/gui/widgets/menus/view_menu.cpp
    editor/gui/widgets/menus/view_menu.hpp

    editor/gui/widgets/properties/properties_dock.cpp
    editor/gui/widgets/properties/properties_dock.hpp
    editor/gui/widgets/properties/property_table.cpp
    editor/gui/widgets/properties/property_table.hpp

    editor/gui/widgets/properties/items/bool_property_widget.cpp
    editor/gui/widgets/properties/items/bool_property_widget.hpp
    editor/gui/widgets/properties/items/color_property_widget.cpp
    editor/gui/widgets/properties/items/color_property_widget.hpp
    editor/gui/widgets/properties/items/file_property_widget.hpp
    editor/gui/widgets/properties/items/file_property_widget.cpp
    editor/gui/widgets/properties/items/float_property_widget.cpp
    editor/gui/widgets/properties/items/float_property_widget.hpp
    editor/gui/widgets/properties/items/int_property_widget.cpp
    editor/gui/widgets/properties/items/int_property_widget.hpp
    editor/gui/widgets/properties/items/object_property_widget.cpp
    editor/gui/widgets/properties/items/object_property_widget.hpp
    editor/gui/widgets/properties/items/property_item_context_menu.cpp
    editor/gui/widgets/properties/items/property_item_context_menu.hpp
    editor/gui/widgets/properties/items/string_property_widget.cpp
    editor/gui/widgets/properties/items/string_property_widget.hpp

    editor/gui/widgets/properties/dialogs/add_property_dialog.cpp
    editor/gui/widgets/properties/dialogs/add_property_dialog.hpp
    editor/gui/widgets/properties/dialogs/change_property_type_dialog.cpp
    editor/gui/widgets/properties/dialogs/change_property_type_dialog.hpp
    editor/gui/widgets/properties/dialogs/property_type_combo.cpp
    editor/gui/widgets/properties/dialogs/property_type_combo.hpp
    editor/gui/widgets/properties/dialogs/rename_property_dialog.cpp
    editor/gui/widgets/properties/dialogs/rename_property_dialog.hpp

    editor/gui/widgets/tilesets/tileset_dock.cpp
    editor/gui/widgets/tilesets/tileset_dock.hpp
    editor/gui/widgets/tilesets/tileset_tabs.cpp
    editor/gui/widgets/tilesets/tileset_tabs.hpp
    editor/gui/widgets/tilesets/tileset_view.cpp
    editor/gui/widgets/tilesets/tileset_view.hpp

    editor/gui/widgets/tilesets/dialogs/tileset_dialog.cpp
    editor/gui/widgets/tilesets/dialogs/tileset_dialog.hpp

    editor/gui/widgets/toolbar/toolbar.cpp
    editor/gui/widgets/toolbar/toolbar.hpp
    editor/gui/widgets/toolbar/tool_button.cpp
    editor/gui/widgets/toolbar/tool_button.hpp

    editor/gui/widgets/viewport/document_tabs.cpp
    editor/gui/widgets/viewport/document_tabs.hpp
    editor/gui/widgets/viewport/home_page_content.cpp
    editor/gui/widgets/viewport/home_page_content.hpp
    editor/gui/widgets/viewport/map_view.cpp
    editor/gui/widgets/viewport/map_view.hpp
    editor/gui/widgets/viewport/viewport_cursor_info.cpp
    editor/gui/widgets/viewport/viewport_cursor_info.hpp
    editor/gui/widgets/viewport/viewport_overlay.cpp
    editor/gui/widgets/viewport/viewport_overlay.hpp
    editor/gui/widgets/viewport/viewport_widget.cpp
    editor/gui/widgets/viewport/viewport_widget.hpp

    io/directories.cpp
    io/directories.hpp
    io/history.cpp
    io/history.hpp
    io/preferences.cpp
    io/preferences.hpp
    io/session.cpp
    io/session.hpp

    parsers/map_parser.cpp
    parsers/map_parser.hpp
    parsers/parse_error.cpp
    parsers/parse_error.hpp
    parsers/parse_ir.hpp
    parsers/to_map_document.cpp
    parsers/to_map_document.hpp

    parsers/json/parse_fancy_tiles.cpp
    parsers/json/parse_fancy_tiles.hpp
    parsers/json/parse_json_map.cpp
    parsers/json/parse_json_map.hpp
    parsers/json/parse_layers.cpp
    parsers/json/parse_layers.hpp
    parsers/json/parse_object_layer.cpp
    parsers/json/parse_object_layer.hpp
    parsers/json/parse_properties.cpp
    parsers/json/parse_properties.hpp
    parsers/json/parse_tile_data.cpp
    parsers/json/parse_tile_data.hpp
    parsers/json/parse_tile_layer.cpp
    parsers/json/parse_tile_layer.hpp
    parsers/json/parse_tileset.cpp
    parsers/json/parse_tileset.hpp
    parsers/json/parse_tilesets.cpp
    parsers/json/parse_tilesets.hpp
    parsers/json/read_json.cpp
    parsers/json/read_json.hpp

    parsers/xml/parse_csv.cpp
    parsers/xml/parse_csv.hpp
    parsers/xml/parse_fancy_tiles.cpp
    parsers/xml/parse_fancy_tiles.hpp
    parsers/xml/parse_layers.cpp
    parsers/xml/parse_layers.hpp
    parsers/xml/parse_object_layer.cpp
    parsers/xml/parse_object_layer.hpp
    parsers/xml/parse_properties.cpp
    parsers/xml/parse_properties.hpp
    parsers/xml/parse_tile_data.cpp
    parsers/xml/parse_tile_data.hpp
    parsers/xml/parse_tile_layer.cpp
    parsers/xml/parse_tile_layer.hpp
    parsers/xml/parse_tile_nodes.cpp
    parsers/xml/parse_tile_nodes.hpp
    parsers/xml/parse_tilesets.cpp
    parsers/xml/parse_tilesets.hpp
    parsers/xml/parse_xml_map.cpp
    parsers/xml/parse_xml_map.hpp
    parsers/xml/xml_utils.cpp
    parsers/xml/xml_utils.hpp

    io/saving/common_saving.cpp
    io/saving/common_saving.hpp
    io/saving/save_map_document.cpp
    io/saving/save_map_document.hpp

    io/saving/json/save_fancy_tiles.cpp
    io/saving/json/save_fancy_tiles.hpp
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

    io/saving/xml/append_fancy_tiles.cpp
    io/saving/xml/append_fancy_tiles.hpp
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

    io/saving/yaml/save_document_as_yaml.cpp
    io/saving/yaml/save_document_as_yaml.hpp
    io/saving/yaml/save_layers.cpp
    io/saving/yaml/save_layers.hpp
    io/saving/yaml/save_properties.cpp
    io/saving/yaml/save_properties.hpp
    io/saving/yaml/save_tilesets.cpp
    io/saving/yaml/save_tilesets.hpp
    )

add_library(${TACTILE_LIB} ${SOURCE_FILES})
add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})
add_dependencies(${TACTILE_LIB} TactileYAML)

target_include_directories(${TACTILE_LIB}
    PUBLIC
    ${ROOT_DIR}/src

    SYSTEM PUBLIC
    ${LIBRARY_DIR}/entt
    ${LIBRARY_DIR}/imgui
    ${LIBRARY_DIR}/glew
    ${LIBRARY_DIR}/nenya
    ${LIBRARY_DIR}/rune
    ${LIBRARY_DIR}/init
    ${LIBRARY_DIR}/pugixml
    ${LIBRARY_DIR}/json
    ${LIBRARY_DIR}/stbimage
    ${LIBRARY_DIR}/fontawesome
    ${LIBRARY_DIR}/pfd
    ${LIBRARY_DIR}/magic-enum
    ${LIBRARY_DIR}/tl
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${GENERATED_PROTO_DIR}
    )

target_link_libraries(${TACTILE_LIB}
    PUBLIC
    ${TACTILE_PROTO}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_LIBRARY}
    ${CENTURION_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    TactileYAML
    )

target_compile_definitions(${TACTILE_LIB}
    PUBLIC
    CENTURION_NO_SDL_MIXER
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    IMGUI_DEFINE_MATH_OPERATORS)

if (MSVC)
  target_compile_definitions(${TACTILE_LIB} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

if (MSVC)
  target_compile_options(${TACTILE_LIB}
      PUBLIC
      /EHsc
      /MP
      /W4
      /Zc:preprocessor
      /Zc:__cplusplus
      /wd5105  # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
      )
endif ()

target_precompile_headers(${TACTILE_LIB}
    PRIVATE
    <array>
    <algorithm>
    <string>
    <string_view>
    <format>
    <functional>
    <filesystem>
    <unordered_map>
    <map>
    <vector>
    <variant>
    <memory>
    <concepts>
    <type_traits>

    <centurion.hpp>
    <entt.hpp>
    <json.hpp>
    <rune/rune.hpp>
    <nenya.hpp>
    <optional.hpp>
    <expected.hpp>
    )

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} main.cpp)
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} main.cpp)
endif ()

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

target_link_libraries(${TACTILE_EXE} PUBLIC ${TACTILE_LIB})

if (WIN32)
  copy_directory_post_build(${TACTILE_EXE} ${ROOT_DIR}/bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()

copy_directory_post_build(${TACTILE_EXE} ${RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)