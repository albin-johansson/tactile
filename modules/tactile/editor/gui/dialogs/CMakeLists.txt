cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-dialogs CXX)

set(TACTILE_EDITOR_GUI_DIALOGS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/about_dialog.cpp
    ${PROJECT_SOURCE_DIR}/about_dialog.hpp
    ${PROJECT_SOURCE_DIR}/create_map_dialog.cpp
    ${PROJECT_SOURCE_DIR}/create_map_dialog.hpp
    ${PROJECT_SOURCE_DIR}/credits_dialog.cpp
    ${PROJECT_SOURCE_DIR}/credits_dialog.hpp
    ${PROJECT_SOURCE_DIR}/dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialog.hpp
    ${PROJECT_SOURCE_DIR}/map_import_error_dialog.cpp
    ${PROJECT_SOURCE_DIR}/map_import_error_dialog.hpp
    ${PROJECT_SOURCE_DIR}/resize_map_dialog.cpp
    ${PROJECT_SOURCE_DIR}/resize_map_dialog.hpp
    ${PROJECT_SOURCE_DIR}/save_as_dialog.cpp
    ${PROJECT_SOURCE_DIR}/save_as_dialog.hpp
    ${PROJECT_SOURCE_DIR}/settings_dialog.cpp
    ${PROJECT_SOURCE_DIR}/settings_dialog.hpp
    ${PROJECT_SOURCE_DIR}/string_input_dialog.cpp
    ${PROJECT_SOURCE_DIR}/string_input_dialog.hpp

    PARENT_SCOPE)