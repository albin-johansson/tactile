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
    assert.cpp
    assert.hpp
    build.hpp
    logging.cpp
    logging.hpp
    profile.hpp
    throw.hpp

    cfg/attributes.cpp
    cfg/attributes.hpp
    cfg/configuration.cpp
    cfg/configuration.hpp
    cfg/imgui_context.cpp
    cfg/imgui_context.hpp
    cfg/protobuf_context.cpp
    cfg/protobuf_context.hpp

    core/map.hpp
    core/mouse_info.hpp
    core/mouse_pos.hpp
    core/property_value.hpp
    core/region.hpp
    core/tile_cache.hpp
    core/tool_type.hpp
    core/viewport.hpp

    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp
    core/algorithms/invoke_n.hpp

    core/components/animation.hpp
    core/components/component.hpp
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
    core/systems/component_system.cpp
    core/systems/component_system.hpp
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
    editor/events/component_events.hpp
    editor/events/edit_events.hpp
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

    editor/shortcuts/edit_shortcuts.cpp
    editor/shortcuts/edit_shortcuts.hpp
    editor/shortcuts/file_shortcuts.cpp
    editor/shortcuts/file_shortcuts.hpp
    editor/shortcuts/mappings.hpp
    editor/shortcuts/shortcut.cpp
    editor/shortcuts/shortcut.hpp
    editor/shortcuts/shortcuts.cpp
    editor/shortcuts/shortcuts.hpp
    editor/shortcuts/view_shortcuts.cpp
    editor/shortcuts/view_shortcuts.hpp

    editor/gui/icons.cpp
    editor/gui/icons.hpp
    editor/gui/style.cpp
    editor/gui/style.hpp
    editor/gui/texture_utils.cpp
    editor/gui/texture_utils.hpp
    editor/gui/themes.cpp
    editor/gui/themes.hpp

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

    editor/gui/alignment.cpp
    editor/gui/alignment.hpp
    editor/gui/scoped.hpp
    editor/gui/widget_manager.cpp
    editor/gui/widget_manager.hpp

    editor/gui/common/button.cpp
    editor/gui/common/button.hpp
    editor/gui/common/centered_button.cpp
    editor/gui/common/centered_button.hpp
    editor/gui/common/centered_text.cpp
    editor/gui/common/centered_text.hpp
    editor/gui/common/checkbox.cpp
    editor/gui/common/checkbox.hpp
    editor/gui/common/docking_toolbar.cpp
    editor/gui/common/docking_toolbar.hpp
    editor/gui/common/help_marker.cpp
    editor/gui/common/help_marker.hpp
    editor/gui/common/input_widgets.cpp
    editor/gui/common/input_widgets.hpp
    editor/gui/common/mouse_tracker.cpp
    editor/gui/common/mouse_tracker.hpp
    editor/gui/common/rubber_band.cpp
    editor/gui/common/rubber_band.hpp

    editor/gui/components/component_editor.cpp
    editor/gui/components/component_editor.hpp
    editor/gui/components/component_name_dialog.cpp
    editor/gui/components/component_name_dialog.hpp
    editor/gui/components/component_widget.cpp
    editor/gui/components/component_widget.hpp
    editor/gui/components/create_component_attribute_dialog.cpp
    editor/gui/components/create_component_attribute_dialog.hpp
    editor/gui/components/create_component_dialog.cpp
    editor/gui/components/create_component_dialog.hpp
    editor/gui/components/rename_component_attribute_dialog.cpp
    editor/gui/components/rename_component_attribute_dialog.hpp
    editor/gui/components/rename_component_dialog.cpp
    editor/gui/components/rename_component_dialog.hpp

    editor/gui/dialogs/about_dialog.cpp
    editor/gui/dialogs/about_dialog.hpp
    editor/gui/dialogs/create_map_dialog.cpp
    editor/gui/dialogs/create_map_dialog.hpp
    editor/gui/dialogs/credits_dialog.cpp
    editor/gui/dialogs/credits_dialog.hpp
    editor/gui/dialogs/dialog.cpp
    editor/gui/dialogs/dialog.hpp
    editor/gui/dialogs/map_import_error_dialog.cpp
    editor/gui/dialogs/map_import_error_dialog.hpp
    editor/gui/dialogs/resize_map_dialog.cpp
    editor/gui/dialogs/resize_map_dialog.hpp
    editor/gui/dialogs/save_as_dialog.cpp
    editor/gui/dialogs/save_as_dialog.hpp
    editor/gui/dialogs/settings_dialog.cpp
    editor/gui/dialogs/settings_dialog.hpp
    editor/gui/dialogs/string_input_dialog.cpp
    editor/gui/dialogs/string_input_dialog.hpp

    editor/gui/layers/add_layer_popup.cpp
    editor/gui/layers/add_layer_popup.hpp
    editor/gui/layers/layer_dock.cpp
    editor/gui/layers/layer_dock.hpp
    editor/gui/layers/layer_item.cpp
    editor/gui/layers/layer_item.hpp
    editor/gui/layers/layer_item_popup.cpp
    editor/gui/layers/layer_item_popup.hpp
    editor/gui/layers/rename_layer_dialog.cpp
    editor/gui/layers/rename_layer_dialog.hpp

    editor/gui/log/log_dock.cpp
    editor/gui/log/log_dock.hpp

    editor/gui/menus/debug_menu.cpp
    editor/gui/menus/debug_menu.hpp
    editor/gui/menus/edit_menu.cpp
    editor/gui/menus/edit_menu.hpp
    editor/gui/menus/file_menu.cpp
    editor/gui/menus/file_menu.hpp
    editor/gui/menus/help_menu.cpp
    editor/gui/menus/help_menu.hpp
    editor/gui/menus/map_menu.cpp
    editor/gui/menus/map_menu.hpp
    editor/gui/menus/menu_bar.cpp
    editor/gui/menus/menu_bar.hpp
    editor/gui/menus/view_menu.cpp
    editor/gui/menus/view_menu.hpp

    editor/gui/properties/properties_dock.cpp
    editor/gui/properties/properties_dock.hpp
    editor/gui/properties/property_table.cpp
    editor/gui/properties/property_table.hpp

    editor/gui/properties/items/property_item_context_menu.cpp
    editor/gui/properties/items/property_item_context_menu.hpp

    editor/gui/properties/dialogs/add_property_dialog.cpp
    editor/gui/properties/dialogs/add_property_dialog.hpp
    editor/gui/properties/dialogs/change_property_type_dialog.cpp
    editor/gui/properties/dialogs/change_property_type_dialog.hpp
    editor/gui/properties/dialogs/property_type_combo.cpp
    editor/gui/properties/dialogs/property_type_combo.hpp
    editor/gui/properties/dialogs/rename_property_dialog.cpp
    editor/gui/properties/dialogs/rename_property_dialog.hpp

    editor/gui/tilesets/tileset_dock.cpp
    editor/gui/tilesets/tileset_dock.hpp
    editor/gui/tilesets/tileset_tabs.cpp
    editor/gui/tilesets/tileset_tabs.hpp
    editor/gui/tilesets/tileset_view.cpp
    editor/gui/tilesets/tileset_view.hpp

    editor/gui/tilesets/dialogs/tileset_dialog.cpp
    editor/gui/tilesets/dialogs/tileset_dialog.hpp

    editor/gui/toolbar/toolbar.cpp
    editor/gui/toolbar/toolbar.hpp
    editor/gui/toolbar/tool_button.cpp
    editor/gui/toolbar/tool_button.hpp

    editor/gui/viewport/document_tabs.cpp
    editor/gui/viewport/document_tabs.hpp
    editor/gui/viewport/home_page_content.cpp
    editor/gui/viewport/home_page_content.hpp
    editor/gui/viewport/map_view.cpp
    editor/gui/viewport/map_view.hpp
    editor/gui/viewport/viewport_cursor_info.cpp
    editor/gui/viewport/viewport_cursor_info.hpp
    editor/gui/viewport/viewport_overlay.cpp
    editor/gui/viewport/viewport_overlay.hpp
    editor/gui/viewport/viewport_widget.cpp
    editor/gui/viewport/viewport_widget.hpp

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
                           ${Boost_INCLUDE_DIRS}
                           ${TACTILE_PROTO_GENERATED_DIR}
                           )

target_link_libraries(${TACTILE_LIB}
                      PUBLIC
                      ${TACTILE_PROTO}
                      ${TACTILE_IO}
                      SDL2::SDL2-static
                      SDL2::SDL2_image
                      imgui::imgui
                      EnTT::EnTT
                      fmt::fmt
                      protobuf::libprotobuf
                      GLEW::GLEW
                      OpenGL::GL
                      ${Boost_LIBRARIES}
                      )

target_compile_definitions(${TACTILE_LIB}
                           PUBLIC
                           CENTURION_NO_SDL_MIXER
                           CENTURION_NO_SDL_TTF
                           IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                           IMGUI_DEFINE_MATH_OPERATORS
                           GLEW_NO_GLU
                           BOOST_ENABLE_ASSERT_DEBUG_HANDLER
                           BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
                           )

if (MSVC)
  target_compile_definitions(${TACTILE_LIB} PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${TACTILE_EXE} WIN32 ${SOURCE_FILES} main.cpp)
else ()
  add_executable(${TACTILE_EXE} ${SOURCE_FILES} main.cpp)
endif ()

add_dependencies(${TACTILE_EXE} ${TACTILE_LIB})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_LIB}
                         PRIVATE
                         /EHsc
                         /MP
                         /W4
                         /Zc:preprocessor
                         /Zc:__cplusplus
                         /wd5105  # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
                         )

  target_compile_options(${TACTILE_EXE}
                         PRIVATE
                         /EHsc
                         /MP
                         /W4
                         /Zc:preprocessor
                         /Zc:__cplusplus
                         /wd5105  # Disable C5105: "macro expansion producing 'defined' has undefined behavior"
                         )

elseif (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|GNU")
  target_compile_options(${TACTILE_LIB}
                         PRIVATE
                         -Wall
                         -Wextra
                         -Wpedantic
                         -Wconversion
                         )

  target_compile_options(${TACTILE_EXE}
                         PRIVATE
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
                          <imgui.h>
                          <fmt/format.h>
                          )

target_precompile_headers(${TACTILE_EXE} REUSE_FROM ${TACTILE_LIB})

target_link_libraries(${TACTILE_EXE}
                      PRIVATE
                      ${TACTILE_LIB}
                      SDL2::SDL2main
                      )

copy_directory_post_build(${TACTILE_EXE} ${RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)