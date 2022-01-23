cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui CXX)

set(TACTILE_EDITOR_GUI_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/icons.cpp
    ${PROJECT_SOURCE_DIR}/icons.hpp
    ${PROJECT_SOURCE_DIR}/style.cpp
    ${PROJECT_SOURCE_DIR}/style.hpp
    ${PROJECT_SOURCE_DIR}/texture_utils.cpp
    ${PROJECT_SOURCE_DIR}/texture_utils.hpp
    ${PROJECT_SOURCE_DIR}/themes.cpp
    ${PROJECT_SOURCE_DIR}/themes.hpp

    ${PROJECT_SOURCE_DIR}/layout/dock_space.cpp
    ${PROJECT_SOURCE_DIR}/layout/dock_space.hpp
    ${PROJECT_SOURCE_DIR}/layout/load_default_layout.cpp
    ${PROJECT_SOURCE_DIR}/layout/load_default_layout.hpp

    ${PROJECT_SOURCE_DIR}/rendering/graphics.cpp
    ${PROJECT_SOURCE_DIR}/rendering/graphics.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_bounds.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_bounds.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_info.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_info.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_map.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_map.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_object_layer.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_object_layer.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_stamp_preview.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_stamp_preview.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_tile.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_tile.hpp
    ${PROJECT_SOURCE_DIR}/rendering/render_tile_layer.cpp
    ${PROJECT_SOURCE_DIR}/rendering/render_tile_layer.hpp

    ${PROJECT_SOURCE_DIR}/alignment.cpp
    ${PROJECT_SOURCE_DIR}/alignment.hpp
    ${PROJECT_SOURCE_DIR}/scoped.hpp
    ${PROJECT_SOURCE_DIR}/widget_manager.cpp
    ${PROJECT_SOURCE_DIR}/widget_manager.hpp

    ${PROJECT_SOURCE_DIR}/common/button.cpp
    ${PROJECT_SOURCE_DIR}/common/button.hpp
    ${PROJECT_SOURCE_DIR}/common/centered_button.cpp
    ${PROJECT_SOURCE_DIR}/common/centered_button.hpp
    ${PROJECT_SOURCE_DIR}/common/centered_text.cpp
    ${PROJECT_SOURCE_DIR}/common/centered_text.hpp
    ${PROJECT_SOURCE_DIR}/common/checkbox.cpp
    ${PROJECT_SOURCE_DIR}/common/checkbox.hpp
    ${PROJECT_SOURCE_DIR}/common/docking_toolbar.cpp
    ${PROJECT_SOURCE_DIR}/common/docking_toolbar.hpp
    ${PROJECT_SOURCE_DIR}/common/help_marker.cpp
    ${PROJECT_SOURCE_DIR}/common/help_marker.hpp
    ${PROJECT_SOURCE_DIR}/common/input_widgets.cpp
    ${PROJECT_SOURCE_DIR}/common/input_widgets.hpp
    ${PROJECT_SOURCE_DIR}/common/mouse_tracker.cpp
    ${PROJECT_SOURCE_DIR}/common/mouse_tracker.hpp
    ${PROJECT_SOURCE_DIR}/common/rubber_band.cpp
    ${PROJECT_SOURCE_DIR}/common/rubber_band.hpp

    ${PROJECT_SOURCE_DIR}/components/component_dock.cpp
    ${PROJECT_SOURCE_DIR}/components/component_dock.hpp
    ${PROJECT_SOURCE_DIR}/components/component_editor.cpp
    ${PROJECT_SOURCE_DIR}/components/component_editor.hpp

    ${PROJECT_SOURCE_DIR}/components/dialogs/component_name_dialog.cpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/component_name_dialog.hpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/create_component_attribute_dialog.cpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/create_component_attribute_dialog.hpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/create_component_dialog.cpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/create_component_dialog.hpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/rename_component_attribute_dialog.cpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/rename_component_attribute_dialog.hpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/rename_component_dialog.cpp
    ${PROJECT_SOURCE_DIR}/components/dialogs/rename_component_dialog.hpp

    ${PROJECT_SOURCE_DIR}/dialogs/about_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/about_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_map_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_map_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/credits_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/credits_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/map_import_error_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/map_import_error_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/resize_map_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/resize_map_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/save_as_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/save_as_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/settings_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/settings_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/string_input_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/string_input_dialog.hpp

    ${PROJECT_SOURCE_DIR}/layers/add_layer_popup.cpp
    ${PROJECT_SOURCE_DIR}/layers/add_layer_popup.hpp
    ${PROJECT_SOURCE_DIR}/layers/layer_dock.cpp
    ${PROJECT_SOURCE_DIR}/layers/layer_dock.hpp
    ${PROJECT_SOURCE_DIR}/layers/layer_item.cpp
    ${PROJECT_SOURCE_DIR}/layers/layer_item.hpp
    ${PROJECT_SOURCE_DIR}/layers/layer_item_popup.cpp
    ${PROJECT_SOURCE_DIR}/layers/layer_item_popup.hpp

    ${PROJECT_SOURCE_DIR}/layers/dialogs/rename_layer_dialog.cpp
    ${PROJECT_SOURCE_DIR}/layers/dialogs/rename_layer_dialog.hpp

    ${PROJECT_SOURCE_DIR}/log/log_dock.cpp
    ${PROJECT_SOURCE_DIR}/log/log_dock.hpp

    ${PROJECT_SOURCE_DIR}/menus/debug_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/debug_menu.hpp
    ${PROJECT_SOURCE_DIR}/menus/edit_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/edit_menu.hpp
    ${PROJECT_SOURCE_DIR}/menus/file_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/file_menu.hpp
    ${PROJECT_SOURCE_DIR}/menus/help_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/help_menu.hpp
    ${PROJECT_SOURCE_DIR}/menus/map_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/map_menu.hpp
    ${PROJECT_SOURCE_DIR}/menus/menu_bar.cpp
    ${PROJECT_SOURCE_DIR}/menus/menu_bar.hpp
    ${PROJECT_SOURCE_DIR}/menus/view_menu.cpp
    ${PROJECT_SOURCE_DIR}/menus/view_menu.hpp

    ${PROJECT_SOURCE_DIR}/properties/properties_dock.cpp
    ${PROJECT_SOURCE_DIR}/properties/properties_dock.hpp
    ${PROJECT_SOURCE_DIR}/properties/property_table.cpp
    ${PROJECT_SOURCE_DIR}/properties/property_table.hpp

    ${PROJECT_SOURCE_DIR}/properties/items/property_item_context_menu.cpp
    ${PROJECT_SOURCE_DIR}/properties/items/property_item_context_menu.hpp

    ${PROJECT_SOURCE_DIR}/properties/dialogs/add_property_dialog.cpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/add_property_dialog.hpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/change_property_type_dialog.cpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/change_property_type_dialog.hpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/property_type_combo.cpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/property_type_combo.hpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/rename_property_dialog.cpp
    ${PROJECT_SOURCE_DIR}/properties/dialogs/rename_property_dialog.hpp

    ${PROJECT_SOURCE_DIR}/tilesets/tileset_dock.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/tileset_dock.hpp
    ${PROJECT_SOURCE_DIR}/tilesets/tileset_tabs.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/tileset_tabs.hpp
    ${PROJECT_SOURCE_DIR}/tilesets/tileset_view.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/tileset_view.hpp

    ${PROJECT_SOURCE_DIR}/tilesets/dialogs/create_tileset_dialog.cpp
    ${PROJECT_SOURCE_DIR}/tilesets/dialogs/create_tileset_dialog.hpp

    ${PROJECT_SOURCE_DIR}/toolbar/toolbar.cpp
    ${PROJECT_SOURCE_DIR}/toolbar/toolbar.hpp
    ${PROJECT_SOURCE_DIR}/toolbar/tool_button.cpp
    ${PROJECT_SOURCE_DIR}/toolbar/tool_button.hpp

    ${PROJECT_SOURCE_DIR}/viewport/document_tabs.cpp
    ${PROJECT_SOURCE_DIR}/viewport/document_tabs.hpp
    ${PROJECT_SOURCE_DIR}/viewport/home_page_content.cpp
    ${PROJECT_SOURCE_DIR}/viewport/home_page_content.hpp
    ${PROJECT_SOURCE_DIR}/viewport/map_view.cpp
    ${PROJECT_SOURCE_DIR}/viewport/map_view.hpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_cursor_info.cpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_cursor_info.hpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_overlay.cpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_overlay.hpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_widget.cpp
    ${PROJECT_SOURCE_DIR}/viewport/viewport_widget.hpp

    PARENT_SCOPE)