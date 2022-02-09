cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-layers CXX)

set(TACTILE_EDITOR_GUI_LAYERS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/add_layer_context_menu.cpp
    ${PROJECT_SOURCE_DIR}/add_layer_context_menu.hpp
    ${PROJECT_SOURCE_DIR}/layer_dock.cpp
    ${PROJECT_SOURCE_DIR}/layer_dock.hpp
    ${PROJECT_SOURCE_DIR}/layer_item.cpp
    ${PROJECT_SOURCE_DIR}/layer_item.hpp

    ${PROJECT_SOURCE_DIR}/dialogs/rename_layer_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/rename_layer_dialog.hpp

    PARENT_SCOPE)