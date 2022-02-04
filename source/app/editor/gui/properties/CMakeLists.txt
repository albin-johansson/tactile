cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-properties CXX)

set(TACTILE_EDITOR_GUI_PROPERTIES_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/properties_dock.cpp
    ${PROJECT_SOURCE_DIR}/properties_dock.hpp
    ${PROJECT_SOURCE_DIR}/property_table.cpp
    ${PROJECT_SOURCE_DIR}/property_table.hpp

    ${PROJECT_SOURCE_DIR}/dialogs/add_property_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/add_property_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/change_property_type_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/change_property_type_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/property_type_combo.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/property_type_combo.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_property_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_property_dialog.hpp

    ${PROJECT_SOURCE_DIR}/items/property_item_context_menu.cpp
    ${PROJECT_SOURCE_DIR}/items/property_item_context_menu.hpp

    PARENT_SCOPE)