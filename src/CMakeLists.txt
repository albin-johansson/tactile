cmake_minimum_required(VERSION 3.12)

project(tactile-src CXX)

set(TACTILE_SOURCE_FILES
    tactile_app.cpp
    tactile_app.hpp
    version.hpp
    tactile.qrc

    command/add_col.cpp
    command/add_col.hpp
    command/add_layer.cpp
    command/add_layer.hpp
    command/add_property.cpp
    command/add_property.hpp
    command/add_row.cpp
    command/add_row.hpp
    command/add_tileset.cpp
    command/add_tileset.hpp
    command/bucket_fill.cpp
    command/bucket_fill.hpp
    command/command_id.hpp
    command/command_stack.hpp
    command/erase_sequence.cpp
    command/erase_sequence.hpp
    command/map_command.cpp
    command/map_command.hpp
    command/remove_col.cpp
    command/remove_col.hpp
    command/remove_layer.cpp
    command/remove_layer.hpp
    command/remove_property.cpp
    command/remove_property.hpp
    command/remove_row.cpp
    command/remove_row.hpp
    command/remove_tileset.cpp
    command/remove_tileset.hpp
    command/rename_property.cpp
    command/rename_property.hpp
    command/repeated_command.cpp
    command/repeated_command.hpp
    command/repeated_map_command.hpp
    command/repeated_map_command.cpp
    command/resize_map.cpp
    command/resize_map.hpp
    command/set_property.cpp
    command/set_property.hpp
    command/stamp_sequence.cpp
    command/stamp_sequence.hpp

    core/document.hpp
    core/document_delegate.cpp
    core/document_delegate.hpp
    core/model.cpp
    core/model.hpp
    core/position.hpp
    core/tactile_error.hpp

    core/algorithm/algorithm.hpp
    core/algorithm/flood_fill.cpp
    core/algorithm/flood_fill.hpp

    core/aliases/czstring.hpp
    core/aliases/layer_id.hpp
    core/aliases/map_id.hpp
    core/aliases/maybe.hpp
    core/aliases/not_null.hpp
    core/aliases/object_id.hpp
    core/aliases/smart_pointers.hpp
    core/aliases/tile_height.hpp
    core/aliases/tile_id.hpp
    core/aliases/tile_width.hpp
    core/aliases/tileset_id.hpp

    core/map/layer.hpp
    core/map/layer_delegate.cpp
    core/map/layer_delegate.hpp
    core/map/layer_type.hpp
    core/map/map.cpp
    core/map/map.hpp
    core/map/map_document.cpp
    core/map/map_document.hpp
    core/map/map_document_manager.cpp
    core/map/map_document_manager.hpp
    core/map/object_layer.cpp
    core/map/object_layer.hpp
    core/map/tile_layer.cpp
    core/map/tile_layer.hpp

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

    core/viewmodel/item_model_utils.hpp

    core/viewmodel/layer/layer_item.cpp
    core/viewmodel/layer/layer_item.hpp
    core/viewmodel/layer/layer_item_role.hpp
    core/viewmodel/layer/layer_model.cpp
    core/viewmodel/layer/layer_model.hpp

    core/viewmodel/property/make_property_item.cpp
    core/viewmodel/property/make_property_item.hpp
    core/viewmodel/property/property_item_role.hpp
    core/viewmodel/property/property_item_type.hpp
    core/viewmodel/property/property_items.cpp
    core/viewmodel/property/property_items.hpp
    core/viewmodel/property/property_model.cpp
    core/viewmodel/property/property_model.hpp

    gui/dock_widget.cpp
    gui/dock_widget.hpp
    gui/icons.cpp
    gui/icons.hpp
    gui/init_ui.hpp
    gui/tab_bar.cpp
    gui/tab_bar.hpp
    gui/tab_widget.cpp
    gui/tab_widget.hpp
    gui/tree_widget_item.cpp
    gui/tree_widget_item.hpp

    gui/dialog/about/about_dialog.cpp
    gui/dialog/about/about_dialog.hpp
    gui/dialog/about/about_dialog.ui

    gui/dialog/add_property/add_property_dialog.cpp
    gui/dialog/add_property/add_property_dialog.hpp
    gui/dialog/add_property/add_property_dialog.ui

    gui/dialog/change_property_name/change_property_name_dialog.cpp
    gui/dialog/change_property_name/change_property_name_dialog.hpp
    gui/dialog/change_property_name/change_property_name_dialog.ui

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
    gui/layer/layer_item.cpp
    gui/layer/layer_item.hpp
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
    gui/properties/property_value_widget.hpp
    gui/properties/file_value_widget.ui
    gui/properties/properties_widget.ui

    gui/settings/settings_dialog.cpp
    gui/settings/settings_dialog.hpp
    gui/settings/settings_dialog.ui

    gui/startup/startup_widget.cpp
    gui/startup/startup_widget.hpp
    gui/startup/startup_widget.ui

    gui/status/status_bar.cpp
    gui/status/status_bar.hpp

    gui/tileset/tileset_content_page.cpp
    gui/tileset/tileset_content_page.hpp
    gui/tileset/tileset_dock.cpp
    gui/tileset/tileset_dock.hpp
    gui/tileset/tileset_empty_page.cpp
    gui/tileset/tileset_empty_page.hpp
    gui/tileset/tileset_image_label.cpp
    gui/tileset/tileset_image_label.hpp
    gui/tileset/tileset_image_widget.cpp
    gui/tileset/tileset_image_widget.hpp
    gui/tileset/tileset_rubber_band.cpp
    gui/tileset/tileset_rubber_band.hpp
    gui/tileset/tileset_tab.cpp
    gui/tileset/tileset_tab.hpp
    gui/tileset/tileset_tab_context_menu.cpp
    gui/tileset/tileset_tab_context_menu.hpp
    gui/tileset/tileset_tab_manager.cpp
    gui/tileset/tileset_tab_manager.hpp
    gui/tileset/tileset_widget.cpp
    gui/tileset/tileset_widget.hpp
    gui/tileset/tileset_content_page.ui
    gui/tileset/tileset_empty_page.ui
    gui/tileset/tileset_widget.ui

    gui/tools/tool_dock.cpp
    gui/tools/tool_dock.hpp
    gui/tools/tool_widget.cpp
    gui/tools/tool_widget.hpp
    gui/tools/tool_widget.ui

    gui/window/window.cpp
    gui/window/window.hpp
    gui/window/window.ui

    parse/json/json_utils.cpp
    parse/json/json_utils.hpp

    parse/map/element_id.hpp
    parse/map/json_element.cpp
    parse/map/json_element.hpp
    parse/map/json_engine.cpp
    parse/map/json_engine.hpp
    parse/map/map_file_type.hpp
    parse/map/map_parser.hpp
    parse/map/parse_error.cpp
    parse/map/parse_error.hpp
    parse/map/xml_element.cpp
    parse/map/xml_element.hpp
    parse/map/xml_engine.cpp
    parse/map/xml_engine.hpp

    parse/palette/parse_palette.cpp
    parse/palette/parse_palette.hpp

    parse/xml/xml_utils.cpp
    parse/xml/xml_utils.hpp

    prefs/preferences.cpp
    prefs/preferences.hpp
    prefs/setting.hpp
    prefs/theme.cpp
    prefs/theme.hpp

    service/open/open_map.cpp
    service/open/open_map.hpp

    service/save/export_options.hpp
    service/save/save_json.cpp
    service/save/save_json.hpp
    service/save/save_tmx.cpp
    service/save/save_tmx.hpp
    service/save/save_service.cpp
    service/save/save_service.hpp
    service/save/tiled_version.hpp

    tool/abstract_tool.cpp
    tool/abstract_tool.hpp
    tool/bucket_tool.cpp
    tool/bucket_tool.hpp
    tool/eraser_tool.cpp
    tool/eraser_tool.hpp
    tool/stamp_tool.cpp
    tool/stamp_tool.hpp
    tool/tool_id.hpp
    tool/tool_model.cpp
    tool/tool_model.hpp

    util/nenya.hpp
    util/set_style_sheet.cpp
    util/set_style_sheet.hpp
    util/setup_app.cpp
    util/setup_app.hpp
    util/signal_blocker.hpp
    util/tactile_qstring.hpp
    util/vector_map.hpp)

add_library(libTactile ${TACTILE_SOURCE_FILES})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

target_include_directories(libTactile
    PUBLIC ${TACTILE_ROOT_DIR}/src
    PUBLIC command
    PUBLIC prefs
    PUBLIC parse
    PUBLIC parse/map
    PUBLIC parse/xml
    PUBLIC parse/json
    PUBLIC parse/palette
    PUBLIC tool
    PUBLIC util
    PUBLIC core
    PUBLIC core/util
    PUBLIC core/property
    PUBLIC core/tile
    PUBLIC core/map
    PUBLIC core/tileset
    PUBLIC core/algorithm
    PUBLIC core/aliases
    PUBLIC core/viewmodel
    PUBLIC core/viewmodel/property
    PUBLIC core/viewmodel/layer
    PUBLIC gui
    PUBLIC gui/startup
    PUBLIC gui/editor
    PUBLIC gui/theme
    PUBLIC gui/tileset
    PUBLIC gui/layer
    PUBLIC gui/window
    PUBLIC gui/util
    PUBLIC gui/tools
    PUBLIC gui/settings
    PUBLIC gui/status
    PUBLIC gui/properties
    PUBLIC gui/properties/item
    PUBLIC gui/properties/view
    PUBLIC gui/dialog
    PUBLIC gui/dialog/about
    PUBLIC gui/dialog/resize_map
    PUBLIC gui/dialog/save
    PUBLIC gui/dialog/open_map
    PUBLIC gui/dialog/open_tileset
    PUBLIC gui/dialog/add_property
    PUBLIC gui/dialog/change_property_name
    PUBLIC gui/dialog/select_file
    PUBLIC service
    PUBLIC service/save
    PUBLIC service/open
    PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(libTactile
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Gui
    PUBLIC Qt5::Xml)

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