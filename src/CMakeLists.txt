cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    tactile_app.cpp
    tactile_app.hpp
    version.hpp
    tactile.qrc

    commands/layers/add_layer.cpp
    commands/layers/add_layer.hpp
    commands/layers/change_opacity.cpp
    commands/layers/change_opacity.hpp
    commands/layers/duplicate_layer.cpp
    commands/layers/duplicate_layer.hpp
    commands/layers/move_layer_back.cpp
    commands/layers/move_layer_back.hpp
    commands/layers/move_layer_forward.cpp
    commands/layers/move_layer_forward.hpp
    commands/layers/remove_layer.cpp
    commands/layers/remove_layer.hpp
    commands/layers/select_layer.cpp
    commands/layers/select_layer.hpp
    commands/layers/set_layer_name.cpp
    commands/layers/set_layer_name.hpp
    commands/layers/set_layer_visibility.cpp
    commands/layers/set_layer_visibility.hpp

    commands/maps/add_column.cpp
    commands/maps/add_column.hpp
    commands/maps/add_row.cpp
    commands/maps/add_row.hpp
    commands/maps/map_command.cpp
    commands/maps/map_command.hpp
    commands/maps/remove_column.cpp
    commands/maps/remove_column.hpp
    commands/maps/remove_row.cpp
    commands/maps/remove_row.hpp
    commands/maps/repeated_map_command.hpp
    commands/maps/repeated_map_command.cpp
    commands/maps/resize_map.cpp
    commands/maps/resize_map.hpp

    commands/properties/add_property.cpp
    commands/properties/add_property.hpp
    commands/properties/change_property_type.cpp
    commands/properties/change_property_type.hpp
    commands/properties/remove_property.cpp
    commands/properties/remove_property.hpp
    commands/properties/rename_property.cpp
    commands/properties/rename_property.hpp
    commands/properties/update_property.cpp
    commands/properties/update_property.hpp

    commands/tilesets/add_tileset.cpp
    commands/tilesets/add_tileset.hpp
    commands/tilesets/remove_tileset.cpp
    commands/tilesets/remove_tileset.hpp
    commands/tilesets/set_tileset_name.cpp
    commands/tilesets/set_tileset_name.hpp

    commands/tools/bucket_fill.cpp
    commands/tools/bucket_fill.hpp
    commands/tools/erase_sequence.cpp
    commands/tools/erase_sequence.hpp
    commands/tools/stamp_sequence.cpp
    commands/tools/stamp_sequence.hpp

    commands/command_id.hpp
    commands/command_stack.hpp
    commands/repeated_command.cpp
    commands/repeated_command.hpp

    core/document.hpp
    core/document_delegate.cpp
    core/document_delegate.hpp
    core/model.cpp
    core/model.hpp
    core/position.hpp
    core/tactile_error.hpp

    core/algorithms/algorithm.hpp
    core/algorithms/flood_fill.cpp
    core/algorithms/flood_fill.hpp

    core/aliases/czstring.hpp
    core/aliases/ints.hpp
    core/aliases/layer_id.hpp
    core/aliases/map_id.hpp
    core/aliases/maybe.hpp
    core/aliases/not_null.hpp
    core/aliases/object_id.hpp
    core/aliases/property_id.hpp
    core/aliases/smart_pointers.hpp
    core/aliases/tile_height.hpp
    core/aliases/tile_id.hpp
    core/aliases/tile_width.hpp
    core/aliases/tileset_id.hpp

    core/layers/layer.hpp
    core/layers/layer_delegate.cpp
    core/layers/layer_delegate.hpp
    core/layers/layer_type.hpp
    core/layers/layer_utils.cpp
    core/layers/layer_utils.hpp
    core/layers/object.cpp
    core/layers/object.hpp
    core/layers/object_layer.cpp
    core/layers/object_layer.hpp
    core/layers/object_type.hpp
    core/layers/tile_layer.cpp
    core/layers/tile_layer.hpp

    core/map/map.cpp
    core/map/map.hpp
    core/map/map_document.cpp
    core/map/map_document.hpp

    core/property/property.cpp
    core/property/property.hpp
    core/property/property_delegate.cpp
    core/property/property_delegate.hpp
    core/property/property_manager.hpp

    core/tile/tile_size.cpp
    core/tile/tile_size.hpp

    core/tileset/tileset.cpp
    core/tileset/tileset.hpp
    core/tileset/tileset_manager.cpp
    core/tileset/tileset_manager.hpp
    core/tileset/tileset_selection.hpp

    core/viewmodels/item_model_utils.hpp

    core/viewmodels/layer/layer_item.cpp
    core/viewmodels/layer/layer_item.hpp
    core/viewmodels/layer/layer_item_role.hpp
    core/viewmodels/layer/layer_model.cpp
    core/viewmodels/layer/layer_model.hpp

    core/viewmodels/property/make_property_item.cpp
    core/viewmodels/property/make_property_item.hpp
    core/viewmodels/property/property_item_role.hpp
    core/viewmodels/property/property_item_type.hpp
    core/viewmodels/property/property_items.cpp
    core/viewmodels/property/property_items.hpp
    core/viewmodels/property/property_model.cpp
    core/viewmodels/property/property_model.hpp

    export/map/export_options.cpp
    export/map/export_options.hpp
    export/map/save_map_document.cpp
    export/map/save_map_document.hpp
    export/map/save_map_document_as_json.cpp
    export/map/save_map_document_as_json.hpp
    export/map/save_map_document_as_xml.cpp
    export/map/save_map_document_as_xml.hpp
    export/map/tiled_version.hpp

    export/theme/save_theme.cpp
    export/theme/save_theme.hpp

    gui/icons.cpp
    gui/icons.hpp
    gui/init_ui.hpp

    gui/base/dock_widget.cpp
    gui/base/dock_widget.hpp
    gui/base/tab_bar.cpp
    gui/base/tab_bar.hpp
    gui/base/tab_widget.cpp
    gui/base/tab_widget.hpp

    gui/dialog/about/about_dialog.cpp
    gui/dialog/about/about_dialog.hpp
    gui/dialog/about/about_dialog.ui

    gui/dialog/add_property/add_property_dialog.cpp
    gui/dialog/add_property/add_property_dialog.hpp
    gui/dialog/add_property/add_property_dialog.ui

    gui/dialog/change_property_name/change_property_name_dialog.cpp
    gui/dialog/change_property_name/change_property_name_dialog.hpp
    gui/dialog/change_property_name/change_property_name_dialog.ui

    gui/dialog/export_theme/export_theme_dialog.cpp
    gui/dialog/export_theme/export_theme_dialog.hpp

    gui/dialog/import_theme/import_theme_dialog.cpp
    gui/dialog/import_theme/import_theme_dialog.hpp

    gui/dialog/open_map/open_map_dialog.cpp
    gui/dialog/open_map/open_map_dialog.hpp
    gui/dialog/open_map/open_map_error_dialog.cpp
    gui/dialog/open_map/open_map_error_dialog.hpp
    gui/dialog/open_map/open_map_error_dialog.ui

    gui/dialog/open_tileset/open_tileset_image.cpp
    gui/dialog/open_tileset/open_tileset_image.hpp
    gui/dialog/open_tileset/tileset_dialog.cpp
    gui/dialog/open_tileset/tileset_dialog.hpp
    gui/dialog/open_tileset/tileset_image_file_dialog.hpp
    gui/dialog/open_tileset/tileset_image_file_dialog.cpp
    gui/dialog/open_tileset/tileset_dialog.ui

    gui/dialog/resize_map/resize_dialog.cpp
    gui/dialog/resize_map/resize_dialog.hpp
    gui/dialog/resize_map/resize_dialog.ui

    gui/dialog/save/save_as_dialog.cpp
    gui/dialog/save/save_as_dialog.hpp

    gui/dialog/select_file/select_file_dialog.cpp
    gui/dialog/select_file/select_file_dialog.hpp

    gui/editor/map_editor.cpp
    gui/editor/map_editor.hpp
    gui/editor/map_editor_context_menu.cpp
    gui/editor/map_editor_context_menu.hpp
    gui/editor/map_item.cpp
    gui/editor/map_item.hpp
    gui/editor/map_scene.cpp
    gui/editor/map_scene.hpp
    gui/editor/map_tab_widget.cpp
    gui/editor/map_tab_widget.hpp
    gui/editor/map_view.cpp
    gui/editor/map_view.hpp
    gui/editor/map_editor.ui

    gui/layer/layer_dock.cpp
    gui/layer/layer_dock.hpp
    gui/layer/layer_item_context_menu.cpp
    gui/layer/layer_item_context_menu.hpp
    gui/layer/layer_list_view.cpp
    gui/layer/layer_list_view.hpp
    gui/layer/layer_widget.cpp
    gui/layer/layer_widget.hpp
    gui/layer/layer_widget_context_menu.cpp
    gui/layer/layer_widget_context_menu.hpp
    gui/layer/layer_widget.ui

    gui/properties/color_preview_button.cpp
    gui/properties/color_preview_button.hpp
    gui/properties/color_validator.cpp
    gui/properties/color_validator.hpp
    gui/properties/file_value_widget.cpp
    gui/properties/file_value_widget.hpp
    gui/properties/properties_dock.cpp
    gui/properties/properties_dock.hpp
    gui/properties/properties_widget.cpp
    gui/properties/properties_widget.hpp
    gui/properties/property_context_menu.cpp
    gui/properties/property_context_menu.hpp
    gui/properties/property_name_validator.cpp
    gui/properties/property_name_validator.hpp
    gui/properties/property_tree_view.cpp
    gui/properties/property_tree_view.hpp
    gui/properties/file_value_widget.ui
    gui/properties/properties_widget.ui

    gui/rendering/render_map.cpp
    gui/rendering/render_map.hpp

    gui/settings/color_preview_manager.cpp
    gui/settings/color_preview_manager.hpp
    gui/settings/settings_dialog.cpp
    gui/settings/settings_dialog.hpp
    gui/settings/theme_options_context_menu.cpp
    gui/settings/theme_options_context_menu.hpp
    gui/settings/settings_dialog.ui

    gui/startup/startup_widget.cpp
    gui/startup/startup_widget.hpp
    gui/startup/startup_widget.ui

    gui/status/status_bar.cpp
    gui/status/status_bar.hpp

    gui/tileset/tileset_dock.cpp
    gui/tileset/tileset_dock.hpp
    gui/tileset/tileset_widget.cpp
    gui/tileset/tileset_widget.hpp
    gui/tileset/tileset_widget.ui

    gui/tileset/tabs/tileset_tab.cpp
    gui/tileset/tabs/tileset_tab.hpp
    gui/tileset/tabs/tileset_tab_context_menu.cpp
    gui/tileset/tabs/tileset_tab_context_menu.hpp
    gui/tileset/tabs/tileset_tab_manager.cpp
    gui/tileset/tabs/tileset_tab_manager.hpp

    gui/tileset/tabs/contents/tileset_content_page.cpp
    gui/tileset/tabs/contents/tileset_content_page.hpp
    gui/tileset/tabs/contents/tileset_empty_page.cpp
    gui/tileset/tabs/contents/tileset_empty_page.hpp
    gui/tileset/tabs/contents/tileset_image_label.cpp
    gui/tileset/tabs/contents/tileset_image_label.hpp
    gui/tileset/tabs/contents/tileset_image_widget.cpp
    gui/tileset/tabs/contents/tileset_image_widget.hpp
    gui/tileset/tabs/contents/tileset_rubber_band.cpp
    gui/tileset/tabs/contents/tileset_rubber_band.hpp
    gui/tileset/tabs/contents/tileset_content_page.ui
    gui/tileset/tabs/contents/tileset_empty_page.ui

    gui/tools/tool_dock.cpp
    gui/tools/tool_dock.hpp
    gui/tools/tool_widget.cpp
    gui/tools/tool_widget.hpp
    gui/tools/tool_widget.ui

    gui/window/window.cpp
    gui/window/window.hpp
    gui/window/window_connections.cpp
    gui/window/window_connections.hpp
    gui/window/window.ui

    mouse-tools/bucket_tool.cpp
    mouse-tools/bucket_tool.hpp
    mouse-tools/eraser_tool.cpp
    mouse-tools/eraser_tool.hpp
    mouse-tools/mouse_tool.cpp
    mouse-tools/mouse_tool.hpp
    mouse-tools/mouse_tool_model.cpp
    mouse-tools/mouse_tool_model.hpp
    mouse-tools/mouse_tool_type.hpp
    mouse-tools/stamp_tool.cpp
    mouse-tools/stamp_tool.hpp

    parsing/json_utils.cpp
    parsing/json_utils.hpp
    parsing/parse_palette.cpp
    parsing/parse_palette.hpp
    parsing/xml_utils.cpp
    parsing/xml_utils.hpp

    parsing/map/element_id.hpp
    parsing/map/json_element.cpp
    parsing/map/json_element.hpp
    parsing/map/json_engine.cpp
    parsing/map/json_engine.hpp
    parsing/map/map_parse_concepts.hpp
    parsing/map/map_parse_data.hpp
    parsing/map/map_parser.hpp
    parsing/map/open_map_document.cpp
    parsing/map/open_map_document.hpp
    parsing/map/parse_error.cpp
    parsing/map/parse_error.hpp
    parsing/map/to_map_document.cpp
    parsing/map/to_map_document.hpp
    parsing/map/to_property.hpp
    parsing/map/xml_element.cpp
    parsing/map/xml_element.hpp
    parsing/map/xml_engine.cpp
    parsing/map/xml_engine.hpp

    preferences/preferences.cpp
    preferences/preferences.hpp
    preferences/setting.hpp
    preferences/theme.cpp
    preferences/theme.hpp

    util/color_utils.cpp
    util/color_utils.hpp
    util/file_handle.cpp
    util/file_handle.hpp
    util/forward_declare.hpp
    util/index_to_position.hpp
    util/prepare_application.cpp
    util/prepare_application.hpp
    util/standard_paths.cpp
    util/standard_paths.hpp
    util/string_utils.cpp
    util/string_utils.hpp
    util/tactile_qstring.hpp
    util/vector_map.hpp
    )

add_library(libTactile ${TACTILE_SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

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
    PUBLIC gui/dialog/add_property
    PUBLIC gui/dialog/change_property_name
    PUBLIC gui/dialog/export_theme
    PUBLIC gui/dialog/import_theme
    PUBLIC gui/dialog/open_map
    PUBLIC gui/dialog/open_tileset
    PUBLIC gui/dialog/resize_map
    PUBLIC gui/dialog/save
    PUBLIC gui/dialog/select_file
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
    PUBLIC util)

target_link_libraries(libTactile
    PUBLIC Qt6::Core
    PUBLIC Qt6::Widgets
    PUBLIC Qt6::OpenGLWidgets
    PUBLIC Qt6::Gui
    PUBLIC Qt6::Xml
    PUBLIC libNenya)

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