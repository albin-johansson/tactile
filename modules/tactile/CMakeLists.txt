cmake_minimum_required(VERSION 3.16)

project(tactile-app CXX)

find_package(OpenGL REQUIRED)  # Not acquired using vcpkg

find_package(GLEW CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")

set(SOURCE_FILES
    application.cpp
    application.hpp
    application_events.cpp
    application_events.hpp
    build.hpp

    cfg/gl_attributes.cpp
    cfg/gl_attributes.hpp
    cfg/logging.cpp
    cfg/logging.hpp
    cfg/protobuf_context.cpp
    cfg/protobuf_context.hpp

    common/tile_cache.hpp

    core/map.hpp
    core/mouse_info.hpp
    core/mouse_pos.hpp
    core/property_value.hpp
    core/region.hpp
    core/tool_type.hpp
    core/viewport.hpp

    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp
    core/algorithms/invoke_n.hpp

    core/components/animation.hpp
    core/components/fancy_tile.hpp
    core/components/layer.hpp
    core/components/object.hpp
    core/components/parent.hpp
    core/components/property.hpp
    core/components/property_context.hpp
    core/components/texture.hpp
    core/components/tileset.hpp
    core/components/tool.hpp
    core/components/uv_tile_size.hpp

    core/systems/animation_system.cpp
    core/systems/animation_system.hpp
    core/systems/duplicate_comp.hpp
    core/systems/map_system.cpp
    core/systems/map_system.hpp
    core/systems/object_system.cpp
    core/systems/object_system.hpp
    core/systems/property_system.cpp
    core/systems/property_system.hpp
    core/systems/registry_system.cpp
    core/systems/registry_system.hpp
    core/systems/snapshot.hpp
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
    core/systems/layers/tile_layer_system.cpp
    core/systems/layers/tile_layer_system.hpp

    core/systems/tools/bucket_tool_system.cpp
    core/systems/tools/bucket_tool_system.hpp
    core/systems/tools/eraser_tool_system.cpp
    core/systems/tools/eraser_tool_system.hpp
    core/systems/tools/object_selection_tool_system.cpp
    core/systems/tools/object_selection_tool_system.hpp
    core/systems/tools/stamp_tool_system.cpp
    core/systems/tools/stamp_tool_system.hpp
    core/systems/tools/tool_system.cpp
    core/systems/tools/tool_system.hpp

    core/utils/buffer_utils.hpp
    core/utils/color_utils.hpp
    core/utils/formatted_string.hpp
    core/utils/texture_manager.cpp
    core/utils/texture_manager.hpp
    core/utils/profile.hpp
    core/utils/scope_id.hpp
    core/utils/vector_map.hpp

    editor/document.hpp
    editor/model.cpp
    editor/model.hpp

    editor/commands/command.cpp
    editor/commands/command.hpp
    editor/commands/command_id.hpp
    editor/commands/command_stack.cpp
    editor/commands/command_stack.hpp
    editor/commands/commands.hpp

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

    editor/commands/objects/move_object_cmd.cpp
    editor/commands/objects/move_object_cmd.hpp
    editor/commands/objects/object_cmd.cpp
    editor/commands/objects/object_cmd.hpp
    editor/commands/objects/set_object_name_cmd.cpp
    editor/commands/objects/set_object_name_cmd.hpp
    editor/commands/objects/set_object_tag_cmd.cpp
    editor/commands/objects/set_object_tag_cmd.hpp
    editor/commands/objects/set_object_visibility_cmd.cpp
    editor/commands/objects/set_object_visibility_cmd.hpp

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
    editor/commands/tilesets/set_tileset_name_cmd.cpp
    editor/commands/tilesets/set_tileset_name_cmd.hpp

    editor/commands/tools/bucket_cmd.cpp
    editor/commands/tools/bucket_cmd.hpp
    editor/commands/tools/eraser_sequence_cmd.cpp
    editor/commands/tools/eraser_sequence_cmd.hpp
    editor/commands/tools/stamp_sequence_cmd.cpp
    editor/commands/tools/stamp_sequence_cmd.hpp

    editor/events/command_events.hpp
    editor/events/layer_events.hpp
    editor/events/map_events.hpp
    editor/events/object_events.hpp
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
    editor/gui/rendering/objects/render_object.cpp
    editor/gui/rendering/objects/render_object.hpp
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

    editor/gui/widgets/log/log_dock.cpp
    editor/gui/widgets/log/log_dock.hpp

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

    io/convert_document_to_ir.cpp
    io/convert_document_to_ir.hpp
    io/directories.cpp
    io/directories.hpp
    io/history.cpp
    io/history.hpp
    io/map_parser.cpp
    io/map_parser.hpp
    io/preferences.cpp
    io/preferences.hpp
    io/save_document.cpp
    io/save_document.hpp
    io/session.cpp
    io/session.hpp
    io/create_document_from_ir.cpp
    io/create_document_from_ir.hpp
    )

add_library(${TACTILE_LIB} ${SOURCE_FILES})

add_dependencies(${TACTILE_LIB} ${TACTILE_PROTO})
add_dependencies(${TACTILE_LIB} ${TACTILE_IO})

target_include_directories(${TACTILE_LIB}
                           PUBLIC
                           ${PROJECT_SOURCE_DIR}
                           ${ROOT_DIR}/modules/tactile-base
                           ${ROOT_DIR}/modules/tactile-io/include

                           SYSTEM PUBLIC
                           ${LIBRARY_DIR}/centurion
                           ${LIBRARY_DIR}/fontawesome
                           ${LIBRARY_DIR}/pfd
                           ${STB_INCLUDE_DIRS}
                           ${TACTILE_PROTO_GENERATED_DIR}
                           )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      OpenGL::GL
                      GLEW::GLEW
                      SDL2::SDL2
                      SDL2::SDL2main
                      imgui::imgui
                      EnTT::EnTT
                      fmt::fmt
                      protobuf::libprotobuf
                      protobuf::libprotobuf-lite
                      ${TACTILE_PROTO}
                      ${TACTILE_IO_IMPORT_LIBRARY}
                      ${CMAKE_DL_LIBS}
                      )

target_compile_definitions(${TACTILE_LIB}
                           PUBLIC
                           CENTURION_NO_SDL_IMAGE
                           CENTURION_NO_SDL_MIXER
                           CENTURION_NO_SDL_TTF
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
                           GLEW_NO_GLU
                           )

if (MSVC)
  target_compile_definitions(${TACTILE_LIB} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_LIB}
                         PUBLIC
                         /EHsc
                         /MP
                         /W4
                         /Zc:preprocessor
                         /Zc:__cplusplus
                         /wd5105  # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
                         )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${TACTILE_LIB}
                         PUBLIC
                         -Wall
                         -Wextra
                         -Wpedantic
                         -Wconversion
                         )
endif ()

target_precompile_headers(${TACTILE_LIB} PRIVATE
                          <cstdint>
                          <cstddef>
                          <cassert>
                          <memory>
                          <utility>
                          <array>
                          <vector>
                          <map>
                          <unordered_map>
                          <variant>
                          <string>
                          <string_view>
                          <concepts>
                          <functional>

                          <centurion.hpp>
                          <entt/entt.hpp>
                          <fmt/format.h>
                          )

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} main.cpp)
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} main.cpp)
endif ()

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

target_link_libraries(${TACTILE_EXE} PUBLIC ${TACTILE_LIB})

copy_directory_post_build(${TACTILE_EXE} ${RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)