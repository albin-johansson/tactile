cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-components CXX)

set(TACTILE_EDITOR_GUI_COMPONENTS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/component_dock.cpp
    ${PROJECT_SOURCE_DIR}/component_dock.hpp
    ${PROJECT_SOURCE_DIR}/component_editor.cpp
    ${PROJECT_SOURCE_DIR}/component_editor.hpp

    ${PROJECT_SOURCE_DIR}/dialogs/component_name_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/component_name_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_component_attribute_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_component_attribute_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_component_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_component_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_component_attribute_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_component_attribute_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_component_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_component_dialog.hpp

    PARENT_SCOPE)